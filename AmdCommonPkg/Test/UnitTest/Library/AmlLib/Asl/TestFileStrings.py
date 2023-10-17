# *****************************************************************************
# *
# * Copyright (C) 2020-2023 Advanced Micro Devices, Inc. All rights reserved.
# *
# ******************************************************************************
#
#  This file defines string constants that will be used while generating the
#  skeleton code for the ASL test cases.
#
# ******************************************************************************

amd_header = """
/*****************************************************************************
 *
 * Copyright (C) 2020-2023 Advanced Micro Devices, Inc. All rights reserved.
 *
 *****************************************************************************/

#include <Library/AmlLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/DebugLib.h>
#include <Library/BaseLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/UnitTestLib.h>
#include <Uefi.h>
#include "AmlTestConstants.h"
#include "AmlTests.h"


#define {caps_name:s}_UNIT_TEST_NAME        "Def{suite_name:s} Test Application"
#define {caps_name:s}_UNIT_TEST_VERSION     "0.1"
"""

test_case_result_start = """UINT8   {test_results_name:s}[] = {{"""

test_case_result_data = """
            {test_hex:s}"""

test_case_result_end = """};
"""

amd_input_validation = """
/***************************************************************/
/* AML Def{suite_name} Input Validation Tests                    */
/***************************************************************/
UNIT_TEST_STATUS
EFIAPI
{suite_name:s}TestInputValidation(
  IN  UNIT_TEST_CONTEXT   Context
)
{{
  EFI_STATUS           Status;
  LIST_ENTRY           *ListHead;
  LIST_ENTRY           AmlListHead;

  ListHead = &AmlListHead;

  InitializeListHead (ListHead);


 // INSERT INPUT VALIDATION TESTS HERE


  if (!EFI_ERROR (Status)) {{
    AmlDebugPrintLinkedObjects (ListHead);
  }}
  AmlFreeObjectList (ListHead);

  return UNIT_TEST_PASSED;
}}

/***************************************************************/
/* AML Def{suite_name:s} Happy Path Tests                        */
/***************************************************************/
"""

test_def = """
/***************************************************************/
// {test_comment:s}
/***************************************************************/
UNIT_TEST_STATUS
EFIAPI
Test{test_name:s}(
  IN  UNIT_TEST_CONTEXT   Context
)
{{
  EFI_STATUS           Status;
  LIST_ENTRY           *ListHead;
  LIST_ENTRY           AmlListHead;
  LIST_ENTRY           *Node;
  AML_OBJECT_INSTANCE  *MainObject;

  ListHead = &AmlListHead;

  InitializeListHead (ListHead);

  // INSERT TEST CODE HERE!

  // Get first Child Node
  Node = GetFirstNode(ListHead);
  MainObject = AML_OBJECT_INSTANCE_FROM_LINK (Node);
  UT_ASSERT_NOT_NULL(MainObject->Data);
  UT_ASSERT_MEM_EQUAL({test_results_name:s}, MainObject->Data, sizeof({test_results_name:s}));
  UT_ASSERT_EQUAL(MainObject->DataSize, sizeof({test_results_name:s}));

  if (!EFI_ERROR (Status)) {{
    AmlDebugPrintLinkedObjects (ListHead);
  }}
  AmlFreeObjectList (ListHead);

  return UNIT_TEST_PASSED;
}}
"""

test_main_start = """
/***************************************************************/
// Creates and runs the {suite_name:s} Test Suite
/***************************************************************/
EFI_STATUS
EFIAPI
{suite_name:s}TestMain (
  VOID
  )
{{
  EFI_STATUS                  Status;
  UNIT_TEST_FRAMEWORK_HANDLE  Framework;
  UNIT_TEST_SUITE_HANDLE      {suite_name:s}TestSuite;

  DEBUG((DEBUG_INFO, "%a v%a\\n", {caps_name:s}_UNIT_TEST_NAME, {caps_name:s}_UNIT_TEST_VERSION));

  //
  // Start setting up the test framework for running the tests.
  //
  Status = InitUnitTestFramework (&Framework, {caps_name:s}_UNIT_TEST_NAME, gEfiCallerBaseName, {caps_name:s}_UNIT_TEST_VERSION);
  if (EFI_ERROR(Status)) {{
    DEBUG((DEBUG_ERROR, "Failed in InitUnitTestFramework. Status = %r\\n", Status));
    goto Done;
  }}

  ///
  // Test the {suite_name:s} function
  //
  Status = CreateUnitTestSuite (&{suite_name:s}TestSuite, Framework, "{suite_name:s} Test Suite", "AmdEdk2Common.Lib.AmlLib", NULL, NULL);
  if (EFI_ERROR(Status)) {{
    DEBUG((DEBUG_ERROR, "Failed in CreateUnitTestSuite for {suite_name:s} Test Suite\\n"));
    Status = EFI_OUT_OF_RESOURCES;
    goto Done;
  }}
  AddTestCase({suite_name:s}TestSuite, "{suite_name:s} Test Input Validation",  "{suite_name:s}TestInputValidation",  {suite_name:s}TestInputValidation, NULL, NULL, NULL);

"""

add_test = """\
  AddTestCase({suite_name:s}TestSuite, "{test_comment:s}",  "Test{test_name:s}",  Test{test_name:s}, NULL, NULL, NULL);
"""

test_main_close = """

  //
  // Execute the tests.
  //
  Status = RunAllTestSuites(Framework);

  Done:
  if (Framework != NULL) {
    FreeUnitTestFramework(Framework);
  }
  return Status;
}
"""

suite_const_header = """
/***************************************************************/
/* AML {suite_name:s} Test Constants                             */
/***************************************************************/
"""

test_case_extern = """
extern UINT8   {test_results_name:s}[];"""


test_call_main =  """
  Status = {suite_name:s}TestMain();
  if(EFI_ERROR(Status)) {{
    DEBUG ((DEBUG_ERROR, "%a: ERROR in {suite_name:s} Unit Test\\n", __FUNCTION__));
  }}
"""

main_test_header = """
/***************************************************************/
/* AML {suite_name:s} Test Suite                        */
/***************************************************************/
EFI_STATUS
EFIAPI
{suite_name:s}TestMain (
  VOID
);
"""

test_file = """  {test_file_name:s}
"""