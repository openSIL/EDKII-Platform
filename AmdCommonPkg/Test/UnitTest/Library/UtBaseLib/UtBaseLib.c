/* Copyright (C) 2021 - 2024 Advanced Micro Devices, Inc. All rights reserved. */
/**
 * @file  UtBaseLib.c
 * @brief
 *
 */

#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#include <stddef.h>
#include <setjmp.h>
#include <stdlib.h>
#include <cmocka.h>
#include <malloc.h>
#include <UtBaseLib.h>
#include <UtLogLib.h>
#include "Log.h"

static AMD_UNIT_TEST_FRAMEWORK_HANDLE ActiveFramework = NULL;

extern AMD_UNIT_TEST_STATUS TestPrerequisite (AMD_UNIT_TEST_CONTEXT Context);
extern void                 TestBody (AMD_UNIT_TEST_CONTEXT Context);
extern AMD_UNIT_TEST_STATUS TestCleanUp (AMD_UNIT_TEST_CONTEXT Context);

static
void UtSetActiveFrameworkHandle (
  AMD_UNIT_TEST_FRAMEWORK_HANDLE Handle
  )
{
  ActiveFramework = Handle;
}

static
void
UtUsage (
  const char *TestName
  )
{
  printf ("USAGE:\n");
  printf ("  %s -i \"Test Iteration Name\" -o \"Absolute Path to Test Output Directory\"", TestName);
  printf (" -c \"Absolute Path to Test Configuration File\"");
  printf ("\nOPTIONS:\n");
  printf ("  -h, --help   Print This Help Message.\n");
}

static
void
UtWrite2ResultFile (
  AMD_UNIT_TEST_FRAMEWORK *Ut
  )
{
  char *result_str = cJSON_Print(Ut->TestResultRoot);
  fputs(result_str, Ut->ResultFile);
  cJSON_free(result_str);
}

static
AMD_UNIT_TEST_STATUS
UtSetTestNameFromArgs (
  AMD_UNIT_TEST_FRAMEWORK *Ut,
  int  argc,
  char *argv[]
  )
{
  char *LastBackSlash;
  char *Extension;
  Ut->TestName = (char*) malloc (AMD_UNIT_TEST_MAX_FILENAME_LENGTH);
  if (Ut->TestName == NULL) {
    printf ("UtInitFromArgs failed to allocate memory dynamically.\n");
    return AMD_UNIT_TEST_ABORTED;
  }
  LastBackSlash = strrchr(argv[0], '\\');
  if (LastBackSlash == NULL) {
    if (strcpy_s (Ut->TestName, AMD_UNIT_TEST_MAX_FILENAME_LENGTH, argv[0]) != 0x00) {
      printf ("UtInitFromArgs failed to set test name.\n");
      free (Ut->TestName);
      return AMD_UNIT_TEST_ABORTED;
    }
  } else {
    if (strcpy_s (Ut->TestName, AMD_UNIT_TEST_MAX_FILENAME_LENGTH, LastBackSlash+1) != 0x00) {
      printf ("UtInitFromArgs failed to set test name.\n");
      free (Ut->TestName);
      return AMD_UNIT_TEST_ABORTED;
    }
  }
  Extension = strrchr (Ut->TestName, '.');
  if (Extension != NULL) {
    *Extension = '\0';
  }
  return AMD_UNIT_TEST_PASSED;
}

static
AMD_UNIT_TEST_STATUS
UtInitTestLogger (
  AMD_UNIT_TEST_FRAMEWORK *Ut
  )
{
  char *LogFilePath = NULL;
  uint32_t LogFilePathLength;
  LogFilePathLength = strlen(Ut->TestOutpath)+strlen(Ut->TestName)+5;
  if (LogFilePathLength > AMD_UNIT_TEST_MAX_PATH_LENGTH) {
    printf ("Test log file path length (%d) exceeds the maximum path length allowed (%d).\n",
      LogFilePathLength, AMD_UNIT_TEST_MAX_PATH_LENGTH);
    return AMD_UNIT_TEST_ABORTED;
  }
  LogFilePath = (char*) malloc (AMD_UNIT_TEST_MAX_PATH_LENGTH);
  strcpy_s (LogFilePath, AMD_UNIT_TEST_MAX_PATH_LENGTH, Ut->TestOutpath);
  strcat_s (LogFilePath, AMD_UNIT_TEST_MAX_PATH_LENGTH, "\\");
  strcat_s (LogFilePath, AMD_UNIT_TEST_MAX_PATH_LENGTH, Ut->TestName);
  strcat_s (LogFilePath, AMD_UNIT_TEST_MAX_PATH_LENGTH, ".log");
  if (fopen_s (&Ut->LogFile, LogFilePath, "w") != 0) {
    printf ("Failed to open %s file for logging.\n", LogFilePath);
    free (LogFilePath);
    return AMD_UNIT_TEST_ABORTED;
  }
  Ut->Log = log_log;
  log_add_fp(Ut->LogFile, AMD_UNIT_TEST_LOG_TRACE);
  free (LogFilePath);
  return AMD_UNIT_TEST_PASSED;
}

static
AMD_UNIT_TEST_STATUS
UtInitTestResult (
  AMD_UNIT_TEST_FRAMEWORK *Ut
  )
{
  char *ResFilePath = NULL;
  uint32_t ResFilePathLength;
  ResFilePathLength = strlen(Ut->TestOutpath)+strlen(Ut->TestName)+5;
  if (ResFilePathLength > AMD_UNIT_TEST_MAX_PATH_LENGTH) {
    printf ("Test result file path length (%d) exceeds the maximum path length allowed (%d).\n",
      ResFilePathLength, AMD_UNIT_TEST_MAX_PATH_LENGTH);
    return AMD_UNIT_TEST_ABORTED;
  }
  ResFilePath = (char*) malloc (AMD_UNIT_TEST_MAX_PATH_LENGTH);
  strcpy_s (ResFilePath, AMD_UNIT_TEST_MAX_PATH_LENGTH, Ut->TestOutpath);
  strcat_s (ResFilePath, AMD_UNIT_TEST_MAX_PATH_LENGTH, "\\");
  strcat_s (ResFilePath, AMD_UNIT_TEST_MAX_PATH_LENGTH, Ut->TestName);
  strcat_s (ResFilePath, AMD_UNIT_TEST_MAX_PATH_LENGTH, ".json");
  if (fopen_s (&Ut->ResultFile, ResFilePath, "w") != 0) {
    printf ("Failed to open %s file for logging.\n", ResFilePath);
    free (ResFilePath);
    return AMD_UNIT_TEST_ABORTED;
  }
  free (ResFilePath);
  Ut->TestResultRoot = cJSON_CreateObject();
  UtAddElementToResult (Ut, "TestName", Ut->TestName);
  UtAddElementToResult (Ut, "TestIteration", Ut->TestIteration);
  return AMD_UNIT_TEST_PASSED;
}

static
AMD_UNIT_TEST_STATUS
UtInitTestConfigs  (
  AMD_UNIT_TEST_FRAMEWORK *Ut
  )
{
  bool IterationFound = false;
  if (fopen_s (&Ut->ConfigFile, Ut->TestConfigFile, "r") != 0) {
    printf ("Failed to open test configuration file (i.e., %s).\n", Ut->TestConfigFile);
    Ut->Log(AMD_UNIT_TEST_LOG_ERROR, __FUNCTION__, __LINE__,
      "Failed to open test configuration file (i.e., %s).", Ut->TestConfigFile);
    return AMD_UNIT_TEST_ABORTED;
  }

  // Read config file to the buffer
  char buffer[AMD_UNIT_TEST_MAX_CONFIG_FILE_LENGTH];
  fread(buffer, 0x1, sizeof(buffer), Ut->ConfigFile);
  fclose (Ut->ConfigFile);

  // Get root node
  cJSON *root = cJSON_Parse(buffer);
  if (root == NULL) {
    const char *JsonErr = cJSON_GetErrorPtr();
    if (JsonErr != NULL) {
      Ut->Log(AMD_UNIT_TEST_LOG_ERROR, __FUNCTION__, __LINE__,
        "Failed to parse test configuration file (JSON Error: %s).", JsonErr);
    }
    cJSON_Delete(root);
    return AMD_UNIT_TEST_ABORTED;
  }

  if (cJSON_IsArray(root) == false) {
    Ut->Log(AMD_UNIT_TEST_LOG_ERROR, __FUNCTION__, __LINE__,
      "Unexpected test configuration format (root must be an array object).");
    cJSON_Delete(root);
  } else if (cJSON_GetArraySize(root) == 0) {
    Ut->Log(AMD_UNIT_TEST_LOG_ERROR, __FUNCTION__, __LINE__,
      "Test configuration file must have at least one iteration (current size is 0).");
    cJSON_Delete(root);
    return AMD_UNIT_TEST_ABORTED;
  }

  cJSON *child = root->child;
  while (child != NULL) {
    cJSON *Iteration = cJSON_GetObjectItemCaseSensitive(child, "Iteration");
    if (Iteration == NULL) {
      Ut->Log(AMD_UNIT_TEST_LOG_ERROR, __FUNCTION__, __LINE__,
        "Unexpected test configuration format (root with empty child found).");
      cJSON_Delete(root);
      return AMD_UNIT_TEST_ABORTED;
    } else if (cJSON_IsString(Iteration) == false) {
      Ut->Log(AMD_UNIT_TEST_LOG_ERROR, __FUNCTION__, __LINE__,
        "Unexpected test configuration format ('Iteration' key is not a string object).");
      break;
    } else if (Iteration->valuestring == NULL) {
      Ut->Log(AMD_UNIT_TEST_LOG_ERROR, __FUNCTION__, __LINE__,
        "Unexpected test configuration format ('Iteration' key valuestring is NULL).");
      break;
    } else if (strcmp (Iteration->valuestring, Ut->TestIteration) == 0) {
      Ut->Log(AMD_UNIT_TEST_LOG_INFO, __FUNCTION__, __LINE__,
        "Iteration %s found.", Ut->TestIteration);
      IterationFound = true;
      break;
    }
    child = child->next;
  }

  if (IterationFound == false) {
    Ut->Log(AMD_UNIT_TEST_LOG_ERROR, __FUNCTION__, __LINE__,
      "Failed to find iteration '%s' in test configuration file.", Ut->TestIteration);
    cJSON_Delete(root);
    return AMD_UNIT_TEST_ABORTED;
  }

  Ut->TestConfigRoot = root;
  Ut->TestConfigIteration = child;

  return AMD_UNIT_TEST_PASSED;
}

void*
UtGetTestConfig  (
  AMD_UNIT_TEST_FRAMEWORK *Ut,
  const char* key
  )
{
  cJSON *Item = cJSON_GetObjectItemCaseSensitive(Ut->TestConfigIteration, key);
  if (Item == NULL) {
    Ut->Log(AMD_UNIT_TEST_LOG_WARN, __FUNCTION__, __LINE__,
      "Parameter '%s' was not found in iteration '%s' parameters.", key, Ut->TestIteration);
    return NULL;
  }
  if (Item->type == cJSON_String) {
    return (void*)(Item->valuestring);
  } else if ((Item->type == cJSON_Number) ||
    (Item->type == cJSON_False) || (Item->type == cJSON_True)) {
    return (void*)(&Item->valueint);
  } else if (Item->type == cJSON_NULL) {
    return NULL;
  }
  return NULL;
}

static
AMD_UNIT_TEST_STATUS
UtParseArgs (
  int   argc,
  char  *argv[],
  char  **TestIteration,
  char  **TestConfigFile,
  char  **TestOutpath
  )
{
  int32_t Index;
  if (argc<7) {
    printf ("Insufficient command line arguments.\n");
    UtUsage (argv[0]);
    exit (AMD_UNIT_TEST_ABORTED);
  }
  for (Index=1; Index < argc; Index++) {
    if (!strcmp(argv[Index], "-o")) {
      *TestOutpath = argv[++Index];
    } else if (!strcmp(argv[Index], "-i")) {
      *TestIteration = argv[++Index];
    } else if (!strcmp(argv[Index], "-c")) {
      *TestConfigFile = argv[++Index];
    } else if (!strcmp(argv[Index], "?") || !strcmp(argv[Index], "-h") ||
      !strcmp(argv[Index], "/?") || !strcmp(argv[Index], "--help")) {
      UtUsage (argv[0]);
      exit (AMD_UNIT_TEST_ABORTED);
    } else {
      printf ("Extra/Invalid command line argument (i.e., %s).\n", argv[Index]);
      UtUsage (argv[0]);
      exit (AMD_UNIT_TEST_ABORTED);
    }
  }
  return AMD_UNIT_TEST_PASSED;
}

int
AmdTestSetupFunctionRunner (
  void  **state
  )
{
  AMD_UNIT_TEST_WRAPPER *UnitTest;
  UnitTest = (AMD_UNIT_TEST_WRAPPER *)(*state);
  if (UnitTest->PrereqFunc == NULL) {
    return AMD_UNIT_TEST_PASSED;
  }
  return UnitTest->PrereqFunc (UnitTest->Context);
}

void
AmdTestFunctionRunner (
  void  **state
  )
{
  AMD_UNIT_TEST_WRAPPER *UnitTest;
  UnitTest = (AMD_UNIT_TEST_WRAPPER *)(*state);
  if (UnitTest->TestFunc != NULL) {
    UnitTest->TestFunc (UnitTest->Context);
  }
}

int
AmdTestTeardownFunctionRunner (
  void  **state
  )
{
  AMD_UNIT_TEST_WRAPPER *UnitTest;
  UnitTest = (AMD_UNIT_TEST_WRAPPER *)(*state);
  if (UnitTest->CleanUpFunc != NULL) {
    return UnitTest->CleanUpFunc (UnitTest->Context);
  }
  return AMD_UNIT_TEST_PASSED;
}

AMD_UNIT_TEST_FRAMEWORK_HANDLE
UtGetActiveFrameworkHandle (
  void
  )
{
  return ActiveFramework;
}

uint32_t
UtGetTestStatus (
  AMD_UNIT_TEST_FRAMEWORK *Ut
  )
{
  return Ut->TestStatus;
}

const char *
UtGetTestStatusString (
  AMD_UNIT_TEST_FRAMEWORK *Ut
  )
{
  if (Ut->TestStatus == AMD_UNIT_TEST_PASSED) {
    return "PASSED";
  } else if (Ut->TestStatus == AMD_UNIT_TEST_FAILED) {
    return "FAILED";
  } else if (Ut->TestStatus == AMD_UNIT_TEST_ABORTED) {
    return "ABORTED";
  } else {
    return "NOT_SET";
  }
}

const char *
UtGetTestName (
  AMD_UNIT_TEST_FRAMEWORK *Ut
  )
{
  return Ut->TestName;
}

const char *
UtGetTestIteration (
  AMD_UNIT_TEST_FRAMEWORK *Ut
  )
{
  return Ut->TestIteration;
}

void
UtSetTestStatus (
  AMD_UNIT_TEST_FRAMEWORK *Ut,
  AMD_UNIT_TEST_STATUS    Status
  )
{
  if (Ut->TestStatus == AMD_UNIT_TEST_STATUS_NOT_SET) {
    Ut->TestStatus = Status;
  } else if (Ut->TestStatus < Status) {
    Ut->TestStatus = Status;
  } else {
    ;
  }
}

void
UtAddElementToResult (
  AMD_UNIT_TEST_FRAMEWORK *Ut,
  const char              *Key,
  const char              *Value
  )
{
  cJSON_AddStringToObject(Ut->TestResultRoot, Key, Value);
}

AMD_UNIT_TEST_STATUS
UtInitFromArgs (
  AMD_UNIT_TEST_FRAMEWORK *Ut,
  int  argc,
  char *argv[]
  )
{
  AMD_UNIT_TEST_STATUS Status;

  memset ((void*)Ut, 0x00, sizeof(AMD_UNIT_TEST_FRAMEWORK));
  UtSetTestStatus (Ut, AMD_UNIT_TEST_STATUS_NOT_SET);

  Status = UtParseArgs (argc, argv, &Ut->TestIteration, &Ut->TestConfigFile, &Ut->TestOutpath);
  if (Status != AMD_UNIT_TEST_PASSED) {
    printf ("UtParseArgs failed (Status=0x%x).\n", Status);
    UtSetTestStatus (Ut, AMD_UNIT_TEST_ABORTED);
    UtDeinit (Ut);
    return Status;
  }

  Status = UtSetTestNameFromArgs (Ut, argc, argv);
  if (Status != AMD_UNIT_TEST_PASSED) {
    printf ("UtSetTestName failed (Status=0x%x).\n", Status);
    UtSetTestStatus (Ut, AMD_UNIT_TEST_ABORTED);
    UtDeinit (Ut);
    return Status;
  }

  Status = UtInitTestLogger (Ut);
  if (Status != AMD_UNIT_TEST_PASSED) {
    printf ("UtInitTestLogger failed (Status=0x%x).\n", Status);
    UtSetTestStatus (Ut, AMD_UNIT_TEST_ABORTED);
    UtDeinit (Ut);
    return Status;
  }

  Status = UtInitTestResult (Ut);
  if (Status != AMD_UNIT_TEST_PASSED) {
    printf ("UtInitTestResult failed (Status=0x%x).\n", Status);
    UtSetTestStatus (Ut, AMD_UNIT_TEST_ABORTED);
    Ut->Log(AMD_UNIT_TEST_LOG_ERROR, __FUNCTION__, __LINE__,
      "UtInitTestResult returned a non-zero value (%d). Test status was set to ABORTED.", Status);
    UtDeinit (Ut);
    return Status;
  }

  Status = UtInitTestConfigs (Ut);
  if (Status != AMD_UNIT_TEST_PASSED) {
    UtSetTestStatus (Ut, AMD_UNIT_TEST_ABORTED);
    Ut->Log(AMD_UNIT_TEST_LOG_ERROR, __FUNCTION__, __LINE__,
      "UtInitTestConfigs returned a non-zero value (%d). Test status was set to ABORTED.", Status);
    UtDeinit (Ut);
    return Status;
  }

  UtSetActiveFrameworkHandle ((AMD_UNIT_TEST_FRAMEWORK_HANDLE)Ut);

  return AMD_UNIT_TEST_PASSED;
}

void
UtSetTestContext (
  AMD_UNIT_TEST_FRAMEWORK  *Ut,
  AMD_UNIT_TEST_CONTEXT    Context
  )
{
  Ut->TestContext = Context;
}

int
UtRunTest (
  AMD_UNIT_TEST_FRAMEWORK *Ut
  )
{
  int                    ReturnCode;
  struct CMUnitTest      *Tests;
  AMD_UNIT_TEST_WRAPPER  UnitTest;

  UnitTest.Context       = Ut->TestContext;
  UnitTest.PrereqFunc    = TestPrerequisite;
  UnitTest.TestFunc      = TestBody;
  UnitTest.CleanUpFunc   = TestCleanUp;

  Tests                  = (struct CMUnitTest*) malloc (sizeof (struct CMUnitTest));
  Tests[0].name          = Ut->TestName;
  Tests[0].test_func     = AmdTestFunctionRunner;
  Tests[0].setup_func    = AmdTestSetupFunctionRunner;
  Tests[0].teardown_func = AmdTestTeardownFunctionRunner;
  Tests[0].initial_state = &UnitTest;

  ReturnCode = _cmocka_run_group_tests(Ut->TestName, Tests, 0x1, NULL, NULL);
  if (ReturnCode != 0) {
    Ut->Log(AMD_UNIT_TEST_LOG_ERROR, __FUNCTION__, __LINE__,
      "UtRunTest returned a non-zero value (%d). Test status was set to ABORTED.", ReturnCode);
    UtSetTestStatus (Ut, AMD_UNIT_TEST_ABORTED);
  }
  free (Tests);

  return ReturnCode;
}

void
UtDeinit (
  AMD_UNIT_TEST_FRAMEWORK *Ut
  )
{
  if (Ut->TestName != NULL) {
    free (Ut->TestName);
    Ut->TestName = NULL;
  }
  if (Ut->TestConfigRoot != NULL) {
    cJSON_Delete(Ut->TestConfigRoot);
    Ut->TestConfigRoot = NULL;
    Ut->TestConfigIteration = NULL;
  }
  if (Ut->ResultFile != NULL) {
    UtAddElementToResult (Ut, "Status", UtGetTestStatusString (Ut));
    UtWrite2ResultFile (Ut);
    fclose (Ut->ResultFile);
    cJSON_Delete(Ut->TestResultRoot);
    Ut->ResultFile = NULL;
    Ut->TestResultRoot = NULL;
  }
  if (Ut->LogFile != NULL) {
    Ut->Log(AMD_UNIT_TEST_LOG_INFO, __FUNCTION__, __LINE__, "Final Test Status was %s.", UtGetTestStatusString (Ut));
    fclose (Ut->LogFile);
    Ut->LogFile = NULL;
  }
}
