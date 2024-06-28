===========================
openSIL Unit Test Framework
===========================

Draft Version
-------------

----------------
1.0 Introduction
----------------
The openSIL unit test (UT) framework is a wrapper around the open-source
`CMocka <https://cmocka.org/>`_ library. The wrapper adds desired functionality
that the standalone library does not provide like test configurations,
unified tost logging, and status tracking.

The openSIL unit test framework has the following goals:

1. Independent and repeatable testing - tests are run in isolation and do not depend on the 
   results of other tests which makes debugging more easy.

2. The framework should allow developers to focus on the test content.

3. When a test fails, it should provide as much information as possible. Additionally, a single
   test failing will not prevent the rest of a batch of tests from running, so that you can
   detect and fix multiple bugs in a single run-edit-compile cycle.

4. Tests should be well organized. The framework provides a simple template and pattern to makes
   unit tests easy to maintain.

5. Tests should be portable and reusable.

---------
2.0 Usage
---------

`````````````````````````````````
2.1 Adding a new unit test module
`````````````````````````````````

A unit test consists of a test target file, which is a C file containing
the implementation of the unit test, and a unit test module (UTM), which is a collection
of C and C header files, including the code to be put under test, with an EDKII
.inf file.

Unit tests should be structured in folders like so: 
UnitTest/Source/(Component Name)/(Sub-component name)/TestThisFeatureUt.c.

The recommended naming convention is: **Test**\ ThisFeature\ **Ut**.c.

''''''''''''''''''''''''''''''
Adding a new UTM on (PHX POC):
''''''''''''''''''''''''''''''

1. Check out the repo (insert command)

2. (Optional) Duplicate UnitTest/Source/Examples/HelloWorldUt as a template for your new UTM

3. Add your UTM .inf file to AmdOpenSilUtPkgPhx.dsc.in under AmdOpenSilPkg/opensil-uefi-interface/UnitTest

4. Run ae.cmd 2M unit-test

5. Upon successful build, the unit test executables will be located under Build/AgesaModulePkg/HostTest/NOOPT_VS2019/IA32

'''''''''''''''''''''''
Executing a single test
'''''''''''''''''''''''

The unit test executables can be run individually at the Windows command prompt. Each executable
takes 3 arguments: 

1. -i: specify the iteration

2. -c: specify the test configuration file

3. -o: test output path

Example:

.. code-block:: 

    .\HelloWorldUt.exe -i Default -o C:/Users/Username/Desktop/Output
    -c REPO_PATH/AGESA/AgesaModulePkg/Test/UnitTest/Source/Examples/HelloWorldUt/HelloWorldUt.json

``````````````````````````````````````
2.2 openSIL unit test source structure
``````````````````````````````````````

A unit test source file has 4 functions: main, TestPrerequisite, TestBody, and TestCleanUp.

''''
Main
''''

The main function

1. parses the command line parameters passed to the test,

2. initiates the openSIL UT framework,

3. sets the *Context* parameter, which will later be available to the "test function trio,"

4. calls the *UtTestRun* function to run the test, and

5. cleans up the framework when the test is done.

''''''''''''''''''
Test function trio
''''''''''''''''''

*UtTestRun* exceutes the "test function trio" consisting of TestPrerequisite, TestBody, and TestCleanUp.
*UtTestRun* sets up the appropriate exception/signal handlers and other data structures prior to running
each of these test functions. When the test is complete, it performs various checks to determine the
outcome of the test.

Within the test functions, the test parameters including the test name, test iteration, test
configuration, and test status can be accessed by calling *UtGetActiveFrameworkHandle* to get the UT
framework structure. Then, by calling the appropriate framework methods, you can access the desired
parameters.

.. code-block::

    AMD_UNIT_TEST_FRAMEWORK* Ut = (AMD_UNIT_TEST_FRAMEWORK*) UtGetActiveFrameworkHandle ();
    const char* TestName        = UtGetTestName (Ut);
    const char* IterationName   = UtGetTestIteration (Ut);

**TestPrerequisite** is called immediately before TestBody. If this function returns any error,
TestBody will not be run and the test status will be set to AMD_UNIT_TEST_ABORTED.

**TestBody** contains the main logic of the test. The unit under test (UUT) should be called within
this function and the test status updated according to the outcome of the UUT. The default test status
is AMD_UNIT_TEST_STATUS_NOT_SET. Failing to update the test status will result in a final test status
of AMD_UNIT_TEST_ABORTED. Use *UtSetTestStatus* to update the test status.

**TestCleanUp** is called immediately after TestBody to provide the opportunity to free any
dynamically allocated memory. TestCleanUp is skipped if TestPrerequisite fails.

The *Context* parameter is an optional parameter passed to each of these 3 functions. This parameter
is test specific and could be a simple value, a string, or a complex structure. *Context* is NULL
by default. To change it, call *UtSetTestContext* in the main function prior to calling *UtTestRun*.

.. code-block::
    
    char* SimpleCharContext = "Hello world";
    UtSetTestContext (&Ut, (AMD_UNIT_TEST_CONTEXT) SimpleCharContext);

````````````````````````````
2.3 openSIL UT framework API
````````````````````````````

- const char\* UtGetTestName (AMD_UNIT_TEST_FRAMEWORK\* Ut): Returns a pointer to a null-terminated
  string containing the name of the test. AUT will take care of the memory clean up for this function.

- const char\* UtGetTestIteration (AMD_UNIT_TEST_FRAMEWORK\* Ut): Returns a pointer to a
  null-terminated string containing the name of the test iteration. AUT will take care of
  the memory clean up for this function.

- AMD_UNIT_TEST_STATUS UtGetTestStatus (AMD_UNIT_TEST_FRAMEWORK\* Ut): Returns the current
  status of the test. 

- const char\* UtGetTestStatusString (AMD_UNIT_TEST_FRAMEWORK\* Ut): Returns the current status
  of the test as a string. AUT will take care of the memory clean up for this function.

- void UtSetTestStatus (AMD_UNIT_TEST_FRAMEWORK\* Ut, AMD_UNIT_TEST_STATUS Status): Updates the
  test status to "Status". Note that the test Status have a priority built into it. Once a higher
  priority status is set, it cannot be changed to a lower priority status. For instance, if the
  test status is being set to FAILED at one point during the test execution, any further attempts
  to update the test status to PASSED will be ignored. Highest priority test status is ABORTED,
  followed by FAILED, PASSED and NOT_SET.

- void UtSetTestContext (AMD_UNIT_TEST_FRAMEWORK\* Ut, AMD_UNIT_TEST_CONTEXT Context): Sets *Context*
  to be passed to the test function trio.

- int UtRunTest (AMD_UNIT_TEST_FRAMEWORK\* Ut): Executes the test function trio.

- AMD_UNIT_TEST_FRAMEWORK_HANDLE UtGetActiveFrameworkHandle (void): Returns a handle to the
  active AUT framework structure.

- void\* UtGetTestConfig  (AMD_UNIT_TEST_FRAMEWORK\* Ut, const char\* key):

- void UtAddElementToResult (AMD_UNIT_TEST_FRAMEWORK\* Ut, const char\* Key, const char\* Value): 

- AMD_UNIT_TEST_STATUS UtInitFromArgs (AMD_UNIT_TEST_FRAMEWORK\* Ut, int  argc, char\* argv[]):
  Initializes the AMD unit test framework.

- void UtDeinit (AMD_UNIT_TEST_FRAMEWORK\* Ut): Deinitialize the AMD unit test framework.

``````````````````
2.4 Best practices
``````````````````

Below are some best practices to follow when writing unit tests:

- Each TestBody within a unit test module should ideally test a single feature of the module.

- Each test should be quick. A few milliseconds is ideal, and one second is the worst case run time.

- Each unit test should ideally include one real implementation of a module. The rest should be stubbed or fake versions of the modules not under test.

- Those stubbed and fake versions of modules should be written early then reused and shared.

```````````````````````````
2.5 Stubs, fakes, and mocks
```````````````````````````

When writing unit tests, it is common to have dependencies that will not be available when running
them locally on your host machine, like a piece of hardware. We write alternate implementations of
these modules to work around this. These alternate implementations can be classified as stubs,
fakes, or mocks.

**Fakes** are working implementations of the module, but will substitute their dependencies with
something simpler and easier to work with in a test environment.

**Stubs** are trivial implementations of a module that just return canned values. They will return
either valid or invalid values for the sake of testing.

**Mocks** are implementations that are controlled by the unit test. They can be programmed with
specific return values, check values of arguments, and help verify that functions are called.
However, unlike a fake, they are not complete models/implementations of the module.

'''''''''''''''''
When to use mocks
'''''''''''''''''

- When you you need something more than a stub, but don't want to go through the trouble of
  modeling the subsystem behavior perfectly in a fake.

- When you have complex retry logic in a system and want to trigger every retry path.

- When you find yourself manually pre-programming return values while using a stub or a fake. 

'''''''''''''''''''''''
Standard test structure
'''''''''''''''''''''''

The industry standard practice for unit testing is the AAA pattern. The openSIL UT framework aims
to follow this pattern.

"The AAA (Arrange-Act-Assert) pattern has become almost a standard across the industry. It
suggests that you should divide your test method into three sections: arrange, act and assert.
Each one of them only responsible for the part in which they are named after. So the arrange
section you only have code required to setup that specific test. Here objects would be created,
mocks setup (if you are using one) and potentially expectations would be set. Then there is the Act,
which should be the invocation of the method being tested. And on Assert you would simply check
whether the expectations were met."

-------------------
3.0 Coverage Report
-------------------

````````````````````````
3.1 Test dispatcher tool
````````````````````````

The test dispatcher tool is a python script which automates the execution of multiple tests.
The tool is located under UnitTest/Scripts/Dispatcher. It requires the following:

- Python3 (any version should work)

- Python mako package (https://www.makotemplates.org/)

  - pip install mako

- DynamoRio application (https://dynamorio.org/). You can download v10.0.0 from
  https://dynamorio.org/page_releases.html. 

The tool takes as input a JSON file for configuration formatted like so:

.. code-block::

    {
      "InPath"                : "", // Absolute path to the folder containing the unit test executables
      "OutPath"               : "", // Absolute path to the folder where the output test logs,
                                       coverage reports, etc. are expected
      "RepoPath"              : "", // Absolute path to the folder where the repo has been checked out
      "DynamoRioPath"         : "", // Absolute path to the DynamoRio installation folder
      "TestProfile"           : "", // Absolute path to the Json file containing the list of test
                                       to be executed
      "PerlPath"              : ""  // Absolute path to the Perl installation bin folder
    }

Completed, this config file should look like this, replacing REPO_PATH (i.e., *workspace*)
with the absolute path to the repo:

.. code-block::

    {
      "InPath"                : "REPO_PATH\\Build\\AmdCommonPkg\\HostTest\\NOOPT_VS2019\\IA32",
      "OutPath"               : "REPO_PATH\\Results\\June26",
      "RepoPath"              : "REPO_PATH",
      "DynamoRioPath"         : "C:\\DynamoRIO-Windows-10.0.0",
      "TestProfile"           : "REPO_PATH\\Platform\\AmdCommonPkg\\Test\\UnitTest\\SoC\\Gn\\GnUtMainProfile.json",
      "PerlPath"              : "C:\\Strawberry\\perl\\bin"
    }

The *TestProfile* parameter above in the config is a JSON file listing all the UTMs to be executed.
Generally, each platform has its own test profile to include all UTMs which are specific to that
platform. If you create a new UTM and want it to be executed by the dispatcher with a given profile,
you must add it to the profile. Below is an example of a test profile for the hello world UTM:

.. code-block::
  
    [
      {
        "Include": []
      },
      {
        "Component": "Examples",
        "Tests" : [
          {
            "Name"       : "HelloWorldUt",
            "ConfigFile" : "AmdOpenSilPkg\\opensil-uefi-interface\\UnitTest\\Source\\Xmp\\HelloWorldUt\\HelloWorldUt.json",
            "Iterations" : ["Default"],
            "Target"     : "AmdOpenSilPkg\\opensil-uefi-interface\\UnitTest\\Source\\Xmp\\HelloWorldUt\\HelloWorldUt.c",
            "Timeout"    : 15
          }
        ]
      }
    ]

Execute the test dispatcher tool by providing it with the config JSON like so:

.. code-block::

    python dispatcher.py dispatcher_configs.json

````````````````````````
3.1 Coverage report tool
````````````````````````

The coverage report tool generates a code coverage report for the test results previously obtained
by running the test dispatcher tool. The tool is located under UnitTest/Scripts/Coverage.
It requires the following:

- Python3 (any version should work)

- Python mako package (https://www.makotemplates.org/)

  - pip install mako

- Python pygount package (https://pypi.org/project/pygount/)

  - pip install pygount

- DynamoRio application (https://dynamorio.org/). You can download v10.0.0 from
  https://dynamorio.org/page_releases.html.

The tool takes as input a JSON file for configuration formatted like so:

.. code-block::

    {
      "InPath"                : "", // Absolute path to the folder containing the unit test
                                       results previously generated by running the dispatcher
      "OutPath"               : "", // Absolute path to the folder where the generated
                                       coverage report will be placed
      "RepoPath"              : "", // Absolute path to the folder where the repo has been
                                       checked out
      "DynamoRioPath"         : "", // Absolute path to the DynamoRio installation folder
      "SrcFileList"           : "", // Absolute path to the Json file containing the list of
                                       source files used in building the platform bios
      "PerlPath"              : ""  // Absolute path to the Perl installation bin folder
    }

Completed, this config file should look like this, replacing REPO_PATH (i.e., *workspace*)
with the absolute path to the repo:

.. code-block::

    {
      "InPath"                : "REPO_PATH\\Results\\June26",
      "OutPath"               : "REPO_PATH\\Results\\June26\\report",
      "RepoPath"              : "REPO_PATH",
      "DynamoRioPath"         : "C:\\DynamoRIO-Windows-10.0.0",
      "SrcFileList"           : "REPO_PATH\\Platform\\AmdCommonPkg\\Test\\UnitTest\\SoC\\Gn\\GnSrcFileList.json",
      "PerlPath"              : "C:\\Strawberry\\perl\\bin"
    }

The *SrcFileList* parameter above is a JSON file listing all the source files used in the build,
i.e., for a particular platform.

Execute the coverage report tool by providing it with the config JSON like so:

.. code-block::

    python report.py report_configs.json

------------------------------------
4.0 Setting up the build environment
------------------------------------

Follow the directions at https://github.com/openSIL/EDKII-Platform to set up your environment
for building the Genoa openSIL POC. You will require Python3.x and git installed on your system.

``````````````````````````````````````````````````
4.1 Directions to build the hello world UT example
``````````````````````````````````````````````````

- Create a new folder for the project. Below, this folder will be referred to as *workspace*

- Cd into *workspace*

- Clone the opensil-uefi-interface into *workspace*/AmdOpenSilPkg and checkout unit test branch

  - (All one command) git clone --recurse-submodules
    https://github.com/openSIL/opensil-uefi-interface.git AmdOpenSilPkg/opensil-uefi-interface

  - cd AmdOpenSilPkg/opensil-uefi-interface

  - git checkout unit_test_framework

  - cd ../.. (i.e., cd back out into *workspace*)

- Clone AGCL-R into *workspace*

  - git clone https://github.com/openSIL/AGCL-R.git

- Clone EDKII-Platform into platform and checkout unit test branch

  - git clone https://github.com/openSIL/EDKII-Platform.git Platform

  - cd Platform

  - git checkout unit_test_framework

  - cd .. (i.e., cd back out into *workspace*)

- Acquire the EDK2 firmware components

  - git clone -b edk2-stable202205 https://github.com/tianocore/edk2.git

    cd edk2

    git submodule update --init

  - cd *workspace* (i.e., cd back out of the edk2 folder)
  
    git clone https://github.com/tianocore/edk2-platforms.git

    cd edk2-platforms

    git checkout b8ffb76b471dae5e24badcd9e04033e8c9439ce3

- Clone amd_firmwares and move them into the needed location

  - cd *workspace* (i.e., cd back out of the edk2-platforms folder)

    git clone https://github.com/openSIL/amd_firmwares.git

    Move all contents of amd_firmwares/Firmwares/Genoa into AGCL-R/AgesaModulePkg/Firmwares/Genoa

- Move the dbuild utility to *workspace*

  - Copy Platform/PlatformTools/root_dbuild.cmd to *workspace*

  - Rename it to dbuild.cmd

- Configure the needed build tools. Refer to the file 
  *workspace*/Platform/PlatformTools/dbuild_support/buildtools.json for inspiration on the paths
  for the build tools

  - Microsoft Visual Studio (MSVC) 2019 - ensure visual studio and the SDK are properly configured
    for your environment

  - Microsoft SDK (match chosen version of Microsoft Visual Studio)

  - Perl (tested 5.32.1.1)

    Download URL: https://strawberryperl.com (tested)

    Set environment variable PERL_PATH i.e., PERL_PATH=C:\\Strawberry\\perl\\bin
    (wherever you installed Perl)

  - NASM (tested 2.15.05)

    Environment variable NASM_PREFIX

  - ASL compiler (tested 20200110)

    Environment variable ASL_PREFIX

- Build the unit tests

  - cd *workspace*

  - dbuild.cmd ut UnitTest\\AgesaModuleUtPkgGn.dsc

- After a successful build of the unit tests, head to
  *workspace*/Platform/AmdCommonPkg/Test/UnitTest/Scripts
  to use the dispatcher and coverage report tools discussed in earlier sections.
  After using the coverage report tool, you will find the generated report (index.html)
  in the OutPath you specified in the config JSON for it.