import os
import sys
import json
import glob
import shutil
import logging
import argparse
import subprocess

from pygount import SourceAnalysis
from mako.lookup import TemplateLookup

REPORT_INDEX_HTML="report.html"

script_dir = os.path.dirname(os.path.abspath(sys.argv[0]))

class CoverageInfo():
  """
  """
  def __init__ (self):
    self.target_file = None
    self.paths       = []

  def __eq__ (self, other):
    if self.target_file == other.target_file:
      return True
    return False

class SrcFile():
  """
  """
  def __init__ (self):
    self.name       = None
    self.path       = None
    self.doc_cnt    = 0
    self.code_cnt   = 0
    self.coverage   = 0
    self.empty_cnt  = 0
    self.string_cnt = 0

class Component():
  """
  """
  def __init__ (self):
    self.name   = None
    self.files  = []

  def __eq__(self, other):
    return self.name == other.name

def ut_get_platform(src_file):
  """
  """
  with open(src_file) as fp:
    items = json.load(fp)
  if "Platform" not in items[0]:
    logging.error("Unknown Src File 'Platform'. Top Src file must indicate the 'Platform' field.")
    sys.exit(1)
  return items[0]["Platform"]

def ut_get_components(src_file):
  components      = []
  nested_files    = []
  with open(src_file) as fp:
    items = json.load(fp)
  if "Include" in items[0]:
    nested_files = items[0]["Include"]
  components = items[1:]
  return components, nested_files

def ut_get_all_components(top_src_file):
  """
  """
  src_files = []
  all_components = []
  repo_path = configs["RepoPath"]

  src_files.append(top_src_file)

  for src_file in src_files:
    _components, _nested_files = ut_get_components(src_file)
    for item in _nested_files:
      src_files.append(os.path.join(repo_path, item))
    for _component in _components:
      component = Component()
      component.name = _component["Component"]
      files = []
      if 'CommonFiles' in _component:
        files += _component["CommonFiles"]
      if 'PlatformFiles' in _component:
        files += _component["PlatformFiles"]
      if 'Files' in _component:
        files += _component["Files"]
      for file in files:
        file_abs_path = os.path.join(repo_path, file["Name"])
        if not os.path.isfile (file_abs_path):
          logging.warning("Source file path {} does not exists. File skipped.".format(file_abs_path))
          continue
        src_file = SrcFile()
        src_file.path = file["Name"]
        src_file.name = os.path.basename(file["Name"]).replace(".c", "").replace(".C", "")
        component.files.append(src_file)

      if component in all_components:
        existing_component = all_components[all_components.index(component)] # Get the existing component
        existing_component.files.extend(component.files) # Add the files to the existing component
      else:
        all_components.append(component)

  return all_components

def ut_collect_coverage_infos(configs):
  """
  """
  coverage_infos = []
  files = glob.glob (configs["InPath"]+ "\**\*.coverage.info", recursive=True)

  for file in files:
    first_line = ""
    with open(file) as fp:
      first_line = fp.readline()
    if first_line.strip() == "":
      continue
    elif first_line[0:3] != "SF:":
      continue
    else:
      pass
    coverage_info = CoverageInfo()
    _name = os.path.basename(first_line[3:].strip())
    _name = _name.replace(".c", "").replace(".C", "")
    coverage_info.target_file = _name

    if coverage_info not in coverage_infos:
      coverage_info.paths.append(file)
      coverage_infos.append(coverage_info)
    else:
      idx = coverage_infos.index(coverage_info)
      coverage_infos[idx].paths.append(file)

  return coverage_infos

def ut_combine_coverage_info_files (files):
  """
  """
  # This function is a bit complicated.

  _all_lines = []
  combined_cov_info_lines = []

  for file in files:
    with open (file, 'r') as fp:
      lines = fp.readlines()
    _all_lines.append(lines)

  line_cnt = len(_all_lines[0])
  for lines in _all_lines:
    if len(lines) != line_cnt:
      # This is a fatal issue
      logging.error("Different # of lines found in .coverage.info files of the same src file.")
      logging.error ("{} != {}".format(len(lines), line_cnt))
      sys.exit(1)

  line0 = _all_lines[0][0]
  for lines in _all_lines:
    if lines[0] != line0:
      # This is a fatal issue
      logging.error("Different opening line found in .coverage.info files of the same src file.")
      logging.error ("{} != {}".format(lines[0], line0))
      sys.exit(1)

  last_line = _all_lines[0][-1]
  for lines in _all_lines:
    if lines[-1] != last_line:
      # This is a fatal issue
      logging.error("Different closing line found in .coverage.info files of the same src file.")
      logging.error ("{} != {}".format(lines[-1], last_line))
      sys.exit(1)

  combined_cov_info_lines.append(line0)

  for line_index in range (1, line_cnt-1):
    _str0 = []
    _str1 = []
    for lines in _all_lines:
      a, b = lines[line_index].split(",")
      _str0.append(a)
      _str1.append(int(b))
    combined_cov_info_lines.append ("{},{}\n".format(_str0[0], str(max(_str1))))

  combined_cov_info_lines.append(last_line)

  return combined_cov_info_lines

def ut_create_file_report (configs, coverage_infos, component_outpath, src_file):
  """
  """
  perl = os.path.join(configs["PerlPath"], "perl.exe")
  genhtml = os.path.join(configs["DynamoRioPath"], "tools\\bin32\genhtml")

  # Get file line and src code line counts
  src_file_abs_path = os.path.join(configs["RepoPath"], src_file.path)

  analysis = SourceAnalysis.from_file(src_file_abs_path, 'pygount')
  src_file.code_cnt   = analysis.code_count
  src_file.doc_cnt    = analysis.documentation_count
  src_file.empty_cnt  = analysis.empty_count
  src_file.string_cnt = analysis.string_count

  # Make src file report folder
  report_dir = os.path.join(component_outpath, src_file.name)
  if not os.path.isdir(report_dir):
    os.mkdir(report_dir)

  coverage_info = None
  for _coverage_info in coverage_infos:
    if _coverage_info.target_file == src_file.name:
      coverage_info = _coverage_info
      break

  if coverage_info is None:
    return

  combined_cov_info_file =  os.path.join(report_dir, "{}.combined.coverage.info".format(src_file.name))
  if (len(coverage_info.paths) == 1):
    shutil.copy (coverage_info.paths[0], combined_cov_info_file)
  else:
    combined_cov_info_str = ut_combine_coverage_info_files (coverage_info.paths)
    with open (combined_cov_info_file, "w") as fp:
      for line in combined_cov_info_str:
        fp.write (line)

  logging.debug("Running {} {} {} -o {}".format(perl, genhtml, combined_cov_info_file, report_dir))
  ret = subprocess.run([perl, genhtml, combined_cov_info_file, "-o", report_dir], stdout=subprocess.PIPE)
  if ret.returncode != 0:
    logging.error("genhtml for {} failed (returncode: {})".format(combined_cov_info_file, ret.returncode))

  stdout_str = str(ret.stdout)
  index0 = stdout_str.find("lines......: ")
  if (index0 == -1):
    logging.error("Failed to parse coverage information from generated html file. This should not happen!")
    return
  index1 = stdout_str[index0+13:].find("%")
  if (index1 == -1):
    logging.error("Failed to parse coverage information from generated html file. This should not happen!")
    return

  src_file.coverage = stdout_str[index0+13:index0+13+index1]


def ut_create_component_report(configs, coverage_infos, components):
  """
  """
  out_path = configs["OutPath"]
  for component in components:
    component_outpath = os.path.join(out_path, component.name)
    if not os.path.isdir(component_outpath):
      os.mkdir(component_outpath)
    for src_file in component.files:
      ut_create_file_report (configs, coverage_infos, component_outpath, src_file)

def ut_get_test_info(configs):
  """
  """
  test_info_file = os.path.join(configs["InPath"], "test_info.json")
  if not os.path.isfile(test_info_file):
    logging.error("Failed to read test_info.json (i.e., {}) file. test_info.json file is required for generating the report.")
    sys.exit(1)

  with open(test_info_file, 'r') as fp:
    test_info = json.load(fp)

  return test_info

def ut_get_configs(config_file):
  """
  """
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
    logging.error("Output directory (i.e., {}) does not exist.".format(configs["OutPath"]))
    sys.exit(1)

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

  if 'SrcFileList' not in configs:
    logging.error("Configuration parameter 'SrcFileList' not found. 'UnitTestSrcFileList' parameter is mandatory.")
    sys.exit(1)
  if not os.path.isfile(configs["SrcFileList"]):
    logging.error("SrcFileList (i.e., {}) does not exist.".format(configs["SrcFileList"]))
    sys.exit(1)

  return configs

if __name__ == "__main__":
  """
  """

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
  report_template = mylookup.get_template(REPORT_INDEX_HTML)

  # Get script configurations
  configs = ut_get_configs(config_file)

  # Retrieve test info
  test_info = ut_get_test_info(configs)

  # Get unit test platform
  platform = ut_get_platform(configs["SrcFileList"])

  # Get all src components/files
  components = ut_get_all_components(configs["SrcFileList"])

  # Collect all available coverage info files
  coverage_infos = ut_collect_coverage_infos(configs)

  #
  ut_create_component_report(configs, coverage_infos, components)

  report_page = os.path.join(configs["OutPath"], 'index.html')
  with open(report_page, 'w') as fp:
    fp.write(report_template.render(configs = configs, test_info = test_info, platform = platform, components = components))
