import os
import sys
import json
import logging
import argparse
import traceback
import subprocess

from datetime import datetime
from mako.lookup import TemplateLookup

AGESA="AGCL-R"
JSON_EXTENSION=".json"
BINARY_EXTENSION=".exe"
DISPATCHER_INDEX_HTML="dispatcher.html"
OUI="AmdOpenSilPkg/opensil-uefi-interface"
OPENSIL="{}/openSIL".format(OUI)

script_dir = os.path.dirname(os.path.abspath(sys.argv[0]))

class Ut():
  """
  """
  def __init__(self):
    self.name         = None
    self.bin_path     = None
    self.out_path     = None
    self.timeout      = None
    self.target_file  = None
    self.status       = []
    self.coverage     = []
    self.iterations   = []

class UtComponent():
  """
  """
  def __init__ (self):
    self.name   = None
    self.tests  = []

  def __eq__(self, other):
    return self.name == other.name

def get_git_revision_hash(path):
    return subprocess.check_output(['git', '-C', path, 'rev-parse', '--short', 'HEAD']).decode('ascii').strip()

def find_drcov_log(path):

  files_full_path = [os.path.join(path, _file) for _file in os.listdir(path)]
  lst = sorted(
    filter(lambda x: (os.path.basename(x).startswith("drcov") and os.path.basename(x).endswith(".log")), files_full_path),
    key=os.path.getmtime,
  )
  if not lst:
    logging.error("Could not locate drcov output log in {}.".format(path))
    sys.exit(1)
  return lst[-1]

def get_test_status(result_file):
  status = None
  with open(result_file) as fp:
    results = json.load(fp)
    if "Status" in results:
      status = results["Status"]
  return status

def ut_dispatch(configs, components):

  drrun = os.path.join(configs["DynamoRioPath"], "bin32\\drrun.exe")
  drcov2lcov = os.path.join(configs["DynamoRioPath"], "tools\\bin32\\drcov2lcov.exe")
  perl = os.path.join(configs["PerlPath"], "perl.exe")
  genhtml = os.path.join(configs["DynamoRioPath"], "tools\\bin32\genhtml")

  for component in components:
    for test in component.tests:
      if not os.path.isdir(test.out_path):
         os.mkdir (test.out_path)
      for iteration in test.iterations:
        test_iter_out_path = os.path.join (test.out_path, iteration)
        if not os.path.isdir (test_iter_out_path):
          os.mkdir (test_iter_out_path)
        test.status.append (None)
        test.coverage.append ("NA")
        try:
          logging.debug ("Running {} -t drcov -- {} -i {} -o {} -c {}".format(drrun, test.bin_path, iteration, test_iter_out_path, test.cfg_path))
          ret = subprocess.run([drrun, "-t", "drcov", "-logdir", test_iter_out_path,
            "--", test.bin_path, "-i", iteration, "-o", test_iter_out_path, "-c", test.cfg_path], timeout=test.timeout)
          if ret.returncode != 0:
            logging.error("Test {} drrun failed (returncode: {})".format(test.name, ret.returncode))
            continue
        except subprocess.TimeoutExpired as err:
          logging.error("Test {} drrun execution time expired (this is considered as a failure).")
          continue
        except Exception as err:
          logging.error("Test {} drrun execution threw an exception (this is considered as a failure).")
          logging.error(traceback.format_exc())
          continue

        result_file = os.path.join(test_iter_out_path, test.name + JSON_EXTENSION)
        if os.path.isfile (result_file):
          test.status[-1] = get_test_status (result_file)
        logging.debug("Test {} execution is over. Reported status is {}.".format(test.name, test.status[-1]))

        drcov_logfile = find_drcov_log(test_iter_out_path)
        lcov_outfile = os.path.join(test_iter_out_path, "{}.coverage.info".format(test.name))
        logging.debug("Running {} -input {} -output {} -src_filter {}".format(drcov2lcov, drcov_logfile, lcov_outfile, test.target_file))
        ret = subprocess.run([drcov2lcov, "-input", drcov_logfile, "-output", lcov_outfile, "-src_filter", test.target_file])
        if ret.returncode != 0:
          logging.error("Test {} drcov2lcov failed (returncode: {})".format(test.name, ret.returncode))
          continue
        logging.debug("Running {} {} {} -o {}".format(perl, genhtml, lcov_outfile, test_iter_out_path))
        ret = subprocess.run([perl, genhtml, lcov_outfile, "-o", test_iter_out_path], stdout=subprocess.PIPE)
        if ret.returncode != 0:
          logging.error("genhtml for {} failed (returncode: {})".format(test.name, ret.returncode))
          continue
        # Very hacky way to get the coverage percentage
        # FIX ME!
        stdout_str = str(ret.stdout)
        index0 = stdout_str.find("lines......: ")
        if (index0 == -1):
          continue
        index1 = stdout_str[index0+13:].find("%")
        if (index1 == -1):
          continue
        test.coverage[-1] = stdout_str[index0+13:index0+13+index1]

def ut_get_platform (profile):
  with open(profile) as fp:
    items = json.load(fp)
  if "Platform" not in items[0]:
    logging.error("Unknown test 'Platform'. Test profile header must indicate the 'Platform' field.")
    sys.exit(1)
  return items[0]["Platform"]

def ut_get_components(profile):
  components      = []
  nested_profiles = []
  with open(profile) as fp:
    items = json.load(fp)
  if "Include" in items[0]:
    nested_profiles = items[0]["Include"]
  components = items[1:]
  return components, nested_profiles

def ut_get_all_components(configs):

  inpath         = configs["InPath"]
  outpath        = configs["OutPath"]
  profiles       = []
  repo_path      = configs["RepoPath"]
  all_components = []
  top_profile    = configs["TestProfile"]

  profiles.append(top_profile)

  for profile in profiles:
    _components, _nested_profiles = ut_get_components(profile)
    for item in _nested_profiles:
      profiles.append(os.path.join(repo_path, item))
    for _component in _components:
      component = UtComponent()
      component.name = _component["Component"]
      for test in _component["Tests"]:
        ut = Ut()
        ut.name     = test["Name"]
        ut.bin_path = os.path.join(inpath, ut.name + BINARY_EXTENSION)
        ut.cfg_path = os.path.join(repo_path, test["ConfigFile"])
        ut.out_path = os.path.join(outpath, ut.name)
        ut.timeout  = test["Timeout"]
        ut.target_file = test["Target"]
        ut.iterations = test["Iterations"]
        logging.debug("ut.name: {}.".format(ut.name))
        logging.debug("ut.bin_path: {}".format(ut.bin_path))
        logging.debug("ut.out_path: {}".format(ut.out_path))
        logging.debug("ut.cfg_path: {}".format(ut.cfg_path))
        logging.debug("ut.target_file: {}".format(ut.target_file))
        component.tests.append(ut)

      if component in all_components:
        existing_component = all_components[all_components.index(component)] # Get the existing component
        existing_component.tests.extend(component.tests) # Add the tests to the existing component
      else:
        all_components.append(component)

  return all_components

def ut_get_configs(config_file):

  configs = {}

  with open(config_file) as fp:
    configs = json.load(fp)

  if 'InPath' not in configs:
    logging.error("Configuration parameter 'InPath' not found. 'InPath' parameter is mandatory.")
    sys.exit(1)
  if not os.path.isdir(configs["InPath"]):
    logging.error("Input directory (i.e., {}) does not exist.".format(configs["InPath"]))
    sys.exit(1)

  if 'OutPath' not in configs:
    logging.error("Configuration parameter 'OutPath' not found. 'OutPath' parameter is mandatory.")
    sys.exit(1)
  if not os.path.isdir(configs["OutPath"]):
    #logging.error("Output directory (i.e., {}) does not exist.".format(configs["OutPath"]))
    #sys.exit(1)
    os.mkdir(configs["OutPath"]);

  if 'RepoPath' not in configs:
    logging.error("Configuration parameter 'RepoPath' not found. 'RepoPath' parameter is mandatory.")
    sys.exit(1)
  if not os.path.isdir(configs["RepoPath"]):
    logging.error("Repo directory (i.e., {}) does not exist.".format(configs["RepoPath"]))
    sys.exit(1)

  if 'PerlPath' not in configs:
    logging.error("Configuration parameter 'PerlPath' not found. 'PerlPath' parameter is mandatory.")
    sys.exit(1)
  if not os.path.isdir(configs["PerlPath"]):
    logging.error("Perl directory (i.e., {}) does not exist.".format(configs["PerlPath"]))
    sys.exit(1)

  if 'DynamoRioPath' not in configs:
    logging.error("Configuration parameter 'DynamoRioPath' not found. 'DynamoRioPath' parameter is mandatory.")
    sys.exit(1)
  if not os.path.isdir(configs["DynamoRioPath"]):
    logging.error("DynamoRio directory (i.e., {}) does not exist.".format(configs["DynamoRioPath"]))
    sys.exit(1)

  if 'TestProfile' not in configs:
    logging.error("Configuration parameter 'TestProfile' not found. 'TestProfile' parameter is mandatory.")
    sys.exit(1)
  if not os.path.isfile(configs["TestProfile"]):
    logging.error("TestProfile (i.e., {}) does not exist.".format(configs["TestProfile"]))
    sys.exit(1)

  return configs

if __name__ == "__main__":

  GitHash = {}

  # Set logging format, default level, etc.
  logging.basicConfig(format="%(levelname)s: %(message)s", level=logging.DEBUG)

  parser = argparse.ArgumentParser()

  parser.add_argument(
    "ConfigFile",
    help="Relative path to the JSON configuration file for this script"
  )

  args = parser.parse_args()

  config_file = os.path.join(script_dir, args.ConfigFile)
  if not os.path.isfile(config_file):
    logging.error("Configuration file (i.e., {}) does not exist.".format(config_file))
    sys.exit(1)

  # Get the report template file
  mylookup = TemplateLookup(directories = script_dir)
  report_template = mylookup.get_template(DISPATCHER_INDEX_HTML)

  # Get script configurations
  configs = ut_get_configs(config_file)

  # Get test profile platform
  platform = ut_get_platform(configs["TestProfile"])

  GitHash["AGESA"]  = get_git_revision_hash (os.path.join(configs["RepoPath"], AGESA))
  GitHash["OUI"]    = get_git_revision_hash (os.path.join(configs["RepoPath"], OUI))
  GitHash["OpenSIL"]= get_git_revision_hash (os.path.join(configs["RepoPath"], OPENSIL))

  completion_time = datetime.now().strftime("%d/%m/%Y %H:%M:%S")

  # Get all test componenets
  all_components = ut_get_all_components(configs)

  # Run the tests
  ut_dispatch(configs, all_components)

  # Save test information
  test_info = {}
  test_info["AgesaGitHash"]       = GitHash["AGESA"]
  test_info["OuiGitHash"]         = GitHash["OUI"]
  test_info["OpenSilGitHash"]     = GitHash["OpenSIL"]
  test_info["TestProfile"]        = os.path.basename(configs["TestProfile"])
  test_info["TestBinariesPath"]   = configs["InPath"]
  test_info["TestCompletionTime"] = completion_time
  test_info_file = os.path.join(configs["OutPath"], "test_info.json")
  with open(test_info_file, 'w') as fp:
    json.dump(test_info, fp)

  # Create report page for dispatched tests
  report_page = os.path.join(configs["OutPath"], 'index.html')
  with open(report_page, 'w') as fp:
    fp.write(report_template.render(
      GitHash=GitHash,
      configs=configs,
      platform=platform,
      components=all_components,
      completion_time=completion_time
      )
    )
