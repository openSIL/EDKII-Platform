
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


#define ALIAS_UNIT_TEST_NAME        "DefAlias Test Application"
#define ALIAS_UNIT_TEST_VERSION     "0.1"
UINT8   AslAliasSb[] = {
            0x06, 0x5C, 0x5F, 0x53, 0x42, 0x5F, 0x54, 0x53, 0x54, 0x41};
UINT8   AslAliasLongNameStrings[] = {
            0x06, 0x5C, 0x2F, 0x03, 0x5F, 0x53, 0x42, 0x5F, 0x50, 0x43, 0x49, 0x30, 0x54,
            0x53, 0x54, 0x30, 0x5C, 0x2F, 0x03, 0x5F, 0x53, 0x42, 0x5F, 0x50, 0x43, 0x49,
            0x30, 0x58, 0x58, 0x58, 0x5F};
UINT8   AslAliasLongToShort[] = {
            0x06, 0x5C, 0x2F, 0x05, 0x5F, 0x53, 0x42, 0x5F, 0x50, 0x43, 0x49, 0x30, 0x4C,
            0x50, 0x43, 0x42, 0x45, 0x43, 0x30, 0x5F, 0x4D, 0x54, 0x48, 0x31, 0x4D, 0x54,
            0x48, 0x31};
UINT8   AslAliasShortToShort[] = {
            0x06, 0x4D, 0x54, 0x48, 0x31, 0x46, 0x4F, 0x52, 0x4B};
UINT8   AslAliasSameName[] = {
            0x06, 0x5C, 0x2F, 0x03, 0x5F, 0x53, 0x42, 0x5F, 0x50, 0x43, 0x49, 0x30, 0x54,
            0x53, 0x54, 0x30, 0x5C, 0x2F, 0x03, 0x5F, 0x53, 0x42, 0x5F, 0x50, 0x43, 0x49,
            0x30, 0x54, 0x53, 0x54, 0x30};
UINT8   AslAliasFilledName[] = {
            0x06, 0x5C, 0x2F, 0x03, 0x5F, 0x53, 0x42, 0x5F, 0x50, 0x43, 0x49, 0x30, 0x54,
            0x53, 0x5F, 0x5F, 0x54, 0x53, 0x5F, 0x5F};

/***************************************************************/
/* AML DefAlias Input Validation Tests                    */
/***************************************************************/
UNIT_TEST_STATUS
EFIAPI
AliasTestInputValidation(
  IN  UNIT_TEST_CONTEXT   Context
)
{
  EFI_STATUS           Status;
  LIST_ENTRY           *ListHead;
  LIST_ENTRY           AmlListHead;
  CHAR8                Identifier[20];

  ListHead = &AmlListHead;

  InitializeListHead (ListHead);

  // NULL Source Name
  Status = AmlOPAlias(NULL, Identifier, ListHead);
  UT_ASSERT_EQUAL(EFI_INVALID_PARAMETER, Status);

  // NULL Alias Name
  Status = AmlOPAlias(Identifier, NULL, ListHead);
  UT_ASSERT_EQUAL(EFI_INVALID_PARAMETER, Status);

  // NULL List Head
  Status = AmlOPAlias(Identifier, Identifier, NULL);
  UT_ASSERT_EQUAL(EFI_INVALID_PARAMETER, Status);

  if (!EFI_ERROR (Status)) {
    AmlDebugPrintLinkedObjects (ListHead);
  }
  AmlFreeObjectList (ListHead);

  return UNIT_TEST_PASSED;
}

/***************************************************************/
/* AML DefAlias Happy Path Tests                        */
/***************************************************************/

/***************************************************************/
// Alias(\_SB, TSTA)
/***************************************************************/
UNIT_TEST_STATUS
EFIAPI
TestAliasSb(
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

  Status = AmlOPAlias("\\_SB", "TSTA", ListHead);
  UT_ASSERT_NOT_EFI_ERROR(Status);

  // Get first Child Node
  Node = GetFirstNode(ListHead);
  MainObject = AML_OBJECT_INSTANCE_FROM_LINK (Node);
  UT_ASSERT_NOT_NULL(MainObject->Data);
  UT_ASSERT_MEM_EQUAL(AslAliasSb, MainObject->Data, sizeof(AslAliasSb));
  UT_ASSERT_EQUAL(MainObject->DataSize, sizeof(AslAliasSb));

  if (!EFI_ERROR (Status)) {
    AmlDebugPrintLinkedObjects (ListHead);
  }
  AmlFreeObjectList (ListHead);

  return UNIT_TEST_PASSED;
}

/***************************************************************/
// Alias(\_SB.PCI0.TST0, \_SB.PCI0.XXX)
/***************************************************************/
UNIT_TEST_STATUS
EFIAPI
TestAliasLongNameStrings(
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

  Status = AmlOPAlias("\\_SB.PCI0.TST0", "\\_SB.PCI0.XXX", ListHead);
  UT_ASSERT_NOT_EFI_ERROR(Status);

  // Get first Child Node
  Node = GetFirstNode(ListHead);
  MainObject = AML_OBJECT_INSTANCE_FROM_LINK (Node);
  UT_ASSERT_NOT_NULL(MainObject->Data);
  UT_ASSERT_MEM_EQUAL(AslAliasLongNameStrings, MainObject->Data, sizeof(AslAliasLongNameStrings));
  UT_ASSERT_EQUAL(MainObject->DataSize, sizeof(AslAliasLongNameStrings));

  if (!EFI_ERROR (Status)) {
    AmlDebugPrintLinkedObjects (ListHead);
  }
  AmlFreeObjectList (ListHead);

  return UNIT_TEST_PASSED;
}

/***************************************************************/
// Alias(\_SB.PCI0.LPCB.EC0.MTH1, MTH1)
/***************************************************************/
UNIT_TEST_STATUS
EFIAPI
TestAliasLongToShort(
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

  Status = AmlOPAlias("\\_SB.PCI0.LPCB.EC0.MTH1", "MTH1", ListHead);
  UT_ASSERT_NOT_EFI_ERROR(Status);

  // Get first Child Node
  Node = GetFirstNode(ListHead);
  MainObject = AML_OBJECT_INSTANCE_FROM_LINK (Node);
  UT_ASSERT_NOT_NULL(MainObject->Data);
  UT_ASSERT_MEM_EQUAL(AslAliasLongToShort, MainObject->Data, sizeof(AslAliasLongToShort));
  UT_ASSERT_EQUAL(MainObject->DataSize, sizeof(AslAliasLongToShort));

  if (!EFI_ERROR (Status)) {
    AmlDebugPrintLinkedObjects (ListHead);
  }
  AmlFreeObjectList (ListHead);

  return UNIT_TEST_PASSED;
}

/***************************************************************/
// Alias(MTH1, FORK)
/***************************************************************/
UNIT_TEST_STATUS
EFIAPI
TestAliasShortToShort(
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

  Status = AmlOPAlias("MTH1", "FORK", ListHead);
  UT_ASSERT_NOT_EFI_ERROR(Status);

  // Get first Child Node
  Node = GetFirstNode(ListHead);
  MainObject = AML_OBJECT_INSTANCE_FROM_LINK (Node);
  UT_ASSERT_NOT_NULL(MainObject->Data);
  UT_ASSERT_MEM_EQUAL(AslAliasShortToShort, MainObject->Data, sizeof(AslAliasShortToShort));
  UT_ASSERT_EQUAL(MainObject->DataSize, sizeof(AslAliasShortToShort));

  if (!EFI_ERROR (Status)) {
    AmlDebugPrintLinkedObjects (ListHead);
  }
  AmlFreeObjectList (ListHead);

  return UNIT_TEST_PASSED;
}

/***************************************************************/
// Alias(\_SB.PCI0.TST0, \_SB.PCI0.TST0)
/***************************************************************/
UNIT_TEST_STATUS
EFIAPI
TestAliasSameName(
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

  Status = AmlOPAlias("\\_SB.PCI0.TST0", "\\_SB.PCI0.TST0", ListHead);
  UT_ASSERT_NOT_EFI_ERROR(Status);

  // Get first Child Node
  Node = GetFirstNode(ListHead);
  MainObject = AML_OBJECT_INSTANCE_FROM_LINK (Node);
  UT_ASSERT_NOT_NULL(MainObject->Data);
  UT_ASSERT_MEM_EQUAL(AslAliasSameName, MainObject->Data, sizeof(AslAliasSameName));
  UT_ASSERT_EQUAL(MainObject->DataSize, sizeof(AslAliasSameName));

  if (!EFI_ERROR (Status)) {
    AmlDebugPrintLinkedObjects (ListHead);
  }
  AmlFreeObjectList (ListHead);

  return UNIT_TEST_PASSED;
}

/***************************************************************/
// Alias(\_SB.PCI0.TS, TS)
/***************************************************************/
UNIT_TEST_STATUS
EFIAPI
TestAliasFilledName(
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

  Status = AmlOPAlias("\\_SB.PCI0.TS", "TS", ListHead);
  UT_ASSERT_NOT_EFI_ERROR(Status);

  // Get first Child Node
  Node = GetFirstNode(ListHead);
  MainObject = AML_OBJECT_INSTANCE_FROM_LINK (Node);
  UT_ASSERT_NOT_NULL(MainObject->Data);
  UT_ASSERT_MEM_EQUAL(AslAliasFilledName, MainObject->Data, sizeof(AslAliasFilledName));
  UT_ASSERT_EQUAL(MainObject->DataSize, sizeof(AslAliasFilledName));

  if (!EFI_ERROR (Status)) {
    AmlDebugPrintLinkedObjects (ListHead);
  }
  AmlFreeObjectList (ListHead);

  return UNIT_TEST_PASSED;
}

/***************************************************************/
// Creates and runs the Alias Test Suite
/***************************************************************/
EFI_STATUS
EFIAPI
AliasTestMain (
  VOID
  )
{
  EFI_STATUS                  Status;
  UNIT_TEST_FRAMEWORK_HANDLE  Framework;
  UNIT_TEST_SUITE_HANDLE      AliasTestSuite;

  DEBUG((DEBUG_INFO, "%a v%a\n", ALIAS_UNIT_TEST_NAME, ALIAS_UNIT_TEST_VERSION));

  //
  // Start setting up the test framework for running the tests.
  //
  Status = InitUnitTestFramework (&Framework, ALIAS_UNIT_TEST_NAME, gEfiCallerBaseName, ALIAS_UNIT_TEST_VERSION);
  if (EFI_ERROR(Status)) {
    DEBUG((DEBUG_ERROR, "Failed in InitUnitTestFramework. Status = %r\n", Status));
    goto Done;
  }

  ///
  // Test the Alias function
  //
  Status = CreateUnitTestSuite (&AliasTestSuite, Framework, "Alias Test Suite", "AmdEdk2Common.Lib.AmlLib", NULL, NULL);
  if (EFI_ERROR(Status)) {
    DEBUG((DEBUG_ERROR, "Failed in CreateUnitTestSuite for Alias Test Suite\n"));
    Status = EFI_OUT_OF_RESOURCES;
    goto Done;
  }
  AddTestCase(AliasTestSuite, "Alias Test Input Validation",  "AliasTestInputValidation",  AliasTestInputValidation, NULL, NULL, NULL);

  AddTestCase(AliasTestSuite, "Alias(\\_SB, TSTA)",  "TestAliasSb",  TestAliasSb, NULL, NULL, NULL);
  AddTestCase(AliasTestSuite, "Alias(\\_SB.PCI0.TST0, \\_SB.PCI0.XXX)",  "TestAliasLongNameStrings",  TestAliasLongNameStrings, NULL, NULL, NULL);
  AddTestCase(AliasTestSuite, "Alias(\\_SB.PCI0.LPCB.EC0.MTH1, MTH1)",  "TestAliasLongToShort",  TestAliasLongToShort, NULL, NULL, NULL);
  AddTestCase(AliasTestSuite, "Alias(MTH1, FORK)",  "TestAliasShortToShort",  TestAliasShortToShort, NULL, NULL, NULL);
  AddTestCase(AliasTestSuite, "Alias(\\_SB.PCI0.TST0, \\_SB.PCI0.TST0)",  "TestAliasSameName",  TestAliasSameName, NULL, NULL, NULL);
  AddTestCase(AliasTestSuite, "Alias(\\_SB.PCI0.TS, TS)",  "TestAliasFilledName",  TestAliasFilledName, NULL, NULL, NULL);

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
