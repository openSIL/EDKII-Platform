
/*****************************************************************************
 *
 * Copyright (C) 2021-2022 Advanced Micro Devices, Inc. All rights reserved.
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

#define EISAID_UNIT_TEST_NAME        "EisaId Test Application"
#define EISAID_UNIT_TEST_VERSION     "0.1"

UINT8   AslEisaIdPnp0C02[] = { 0x0C,0x41,0xD0,0x0C,0x02 };


/***************************************************************/
/* AML EisaId Input Validation Tests                        */
/***************************************************************/
UNIT_TEST_STATUS
EFIAPI
EisaIdTestInputValidation(
  IN  UNIT_TEST_CONTEXT   Context
)
{
  EFI_STATUS           Status;
  LIST_ENTRY           *ListHead;
  LIST_ENTRY           AmlListHead;

  ListHead = &AmlListHead;

  InitializeListHead (ListHead);

  // NULL List Head
  DEBUG((DEBUG_ERROR,"NULL List Head\n"));
  Status = AmlOPEisaId ("PNP0000", NULL);
  UT_ASSERT_EQUAL(EFI_INVALID_PARAMETER, Status);

  // NULL String Input
  DEBUG((DEBUG_ERROR,"NULL String\n"));
  Status = AmlOPEisaId (NULL, ListHead);
  UT_ASSERT_EQUAL(EFI_INVALID_PARAMETER, Status);

  // Invalid String Input Bad Prefix
  DEBUG((DEBUG_ERROR,"0NP0000\n"));
  Status = AmlOPEisaId ("0NP0000", ListHead);
  UT_ASSERT_EQUAL(EFI_INVALID_PARAMETER, Status);

  // Invalid String Input Bad Suffix
  DEBUG((DEBUG_ERROR,"PNPDEFG\n"));
  Status = AmlOPEisaId ("PNPDEFG", ListHead);
  UT_ASSERT_EQUAL(EFI_INVALID_PARAMETER, Status);

  // Invalid String Input lowercase
  DEBUG((DEBUG_ERROR,"PnP0000\n"));
  Status = AmlOPEisaId ("PnP0000", ListHead);
  UT_ASSERT_EQUAL(EFI_INVALID_PARAMETER, Status);

  // Invalid String Input too short
  DEBUG((DEBUG_ERROR,"PN0000\n"));
  Status = AmlOPEisaId ("PN0000", ListHead);
  UT_ASSERT_EQUAL(EFI_INVALID_PARAMETER, Status);

  // Invalid String Input too long
  DEBUG((DEBUG_ERROR,"PNP00001\n"));
  Status = AmlOPEisaId ("PNP00001", ListHead);
  UT_ASSERT_EQUAL(EFI_INVALID_PARAMETER, Status);

  if (!EFI_ERROR (Status)) {
    AmlDebugPrintLinkedObjects (ListHead);
  }
  AmlFreeObjectList (ListHead);

  return UNIT_TEST_PASSED;
}

/***************************************************************/
/* AML DefEisaId Happy Path Tests                        */
/***************************************************************/

/***************************************************************/
// EISAID ("PNP0C02")
/***************************************************************/
UNIT_TEST_STATUS
EFIAPI
TestEisaIdPnp0C02(
  IN  UNIT_TEST_CONTEXT   Context
)
{
  EFI_STATUS           Status;
  LIST_ENTRY           *ListHead;
  LIST_ENTRY           AmlListHead;
  LIST_ENTRY           *Node;
  AML_OBJECT_INSTANCE  *MainObject;

  ListHead = &AmlListHead;

  InitializeListHead (ListHead);

  // INSERT TEST CODE HERE!

  // Test EISAID("PNP0C02")
  Status = AmlOPEisaId ("PNP0C02", ListHead);
  UT_ASSERT_NOT_EFI_ERROR(Status);

  // Get first Child Node
  Node = GetFirstNode(ListHead);
  MainObject = AML_OBJECT_INSTANCE_FROM_LINK (Node);
  UT_ASSERT_NOT_NULL(MainObject->Data);
  UT_ASSERT_MEM_EQUAL(AslEisaIdPnp0C02, MainObject->Data, sizeof(AslEisaIdPnp0C02));
  UT_ASSERT_EQUAL(MainObject->DataSize, sizeof(AslEisaIdPnp0C02));

  if (!EFI_ERROR (Status)) {
    AmlDebugPrintLinkedObjects (ListHead);
  }
  AmlFreeObjectList (ListHead);

  return UNIT_TEST_PASSED;
}

/***************************************************************/
// Creates and runs the EisaId Test Suite
/***************************************************************/
EFI_STATUS
EFIAPI
EisaIdTestMain (
  VOID
  )
{
  EFI_STATUS                  Status;
  UNIT_TEST_FRAMEWORK_HANDLE  Framework;
  UNIT_TEST_SUITE_HANDLE      EisaIdTestSuite;

  DEBUG((DEBUG_INFO, "%a v%a\n", EISAID_UNIT_TEST_NAME, EISAID_UNIT_TEST_VERSION));

  //
  // Start setting up the test framework for running the tests.
  //
  Status = InitUnitTestFramework (&Framework, EISAID_UNIT_TEST_NAME, gEfiCallerBaseName, EISAID_UNIT_TEST_VERSION);
  if (EFI_ERROR(Status)) {
    DEBUG((DEBUG_ERROR, "Failed in InitUnitTestFramework. Status = %r\n", Status));
    goto Done;
  }

  ///
  // Test the EisaId function
  //
  Status = CreateUnitTestSuite (&EisaIdTestSuite, Framework, "EisaId Test Suite", "AmdEdk2Common.Lib.AmlLib", NULL, NULL);
  if (EFI_ERROR(Status)) {
    DEBUG((DEBUG_ERROR, "Failed in CreateUnitTestSuite for EisaId Test Suite\n"));
    Status = EFI_OUT_OF_RESOURCES;
    goto Done;
  }
  AddTestCase(EisaIdTestSuite, "EisaId Test Input Validation",  "EisaIdTestInputValidation",  EisaIdTestInputValidation, NULL, NULL, NULL);

  AddTestCase(EisaIdTestSuite, "EISAID (\"PNP0C02\")",  "TestEisaIdPnp0C02",  TestEisaIdPnp0C02, NULL, NULL, NULL);

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
