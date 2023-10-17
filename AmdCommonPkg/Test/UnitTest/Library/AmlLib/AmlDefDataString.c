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


#define STRING_UNIT_TEST_NAME        "DefDataString Test Application"
#define STRING_UNIT_TEST_VERSION     "0.1"

UINT8      DefDataStrTestA[] = {0x08, 0x54, 0x53, 0x54, 0x41, 0x0D, 0x00};

UINT8      DefDataStrTestB[] = {0x08, 0x54, 0x53, 0x54, 0x42, 0x0D, 0x41,
                                0x00};

UINT8      DefDataStrTestC[] = {0x08, 0x54, 0x53, 0x54, 0x43, 0x0D, 0x41, 0x42,
                                0x43, 0x44, 0x45, 0x46, 0x47, 0x48, 0x49, 0x4A,
                                0x4B, 0x4C, 0x4D, 0x4E, 0x4F, 0x50, 0x51, 0x52,
                                0x53, 0x54, 0x55, 0x56, 0x57, 0x58, 0x59, 0x5A,
                                0x00};

UINT8      DefDataStrTestD[] = {0x08, 0x54, 0x53, 0x54, 0x44, 0x0D, 0x61, 0x62,
                                0x63, 0x64, 0x65, 0x66, 0x67, 0x68, 0x69, 0x6A,
                                0x6B, 0x6C, 0x6D, 0x6E, 0x6F, 0x70, 0x71, 0x72,
                                0x73, 0x74, 0x75, 0x76, 0x77, 0x78, 0x79, 0x7A,
                                0x00};

UINT8      DefDataStrTestE[] = {0x08, 0x54, 0x53, 0x54, 0x45, 0x0D, 0x30, 0x31,
                                0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38, 0x39,
                                0x30, 0x00};

UINT8      DefDataStrTestF[] = {0x08, 0x54, 0x53, 0x54, 0x46, 0x0D, 0x41, 0x20,
                                0x21, 0x23, 0x24, 0x25, 0x5E, 0x2A, 0x28, 0x5C,
                                0x29, 0x5F, 0x2B, 0x60, 0x2D, 0x2E, 0x2C, 0x2F,
                                0x3F, 0x3B, 0x3A, 0x3C, 0x3E, 0x3D, 0x3F, 0x40,
                                0x7B, 0x7D, 0x5B, 0x5D, 0x7C, 0x7E, 0x00};

UINT8      DefDataStrTestG[] = {0x08, 0x54, 0x53, 0x54, 0x47, 0x0D, 0x07, 0x08,
                                0x0C, 0x0A, 0x0D, 0x09, 0x0B, 0x00};

/***************************************************************/
/* AML DefDataString Input Validation Tests                   */
/***************************************************************/
UNIT_TEST_STATUS
EFIAPI
StringTestInputValidation(
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
  Status = AmlOPDataString("TEST", NULL);
  UT_ASSERT_EQUAL(EFI_INVALID_PARAMETER, Status);

  // NULL String Input
  DEBUG((DEBUG_ERROR,"NULL String\n"));
  Status = AmlOPDataString(NULL, ListHead);
  UT_ASSERT_EQUAL(EFI_INVALID_PARAMETER, Status);

  // Invalid String Input Low End
  DEBUG((DEBUG_ERROR,"ÇÇÇÇ\n"));
  Status = AmlOPDataString("ÇÇÇÇ", ListHead);
  UT_ASSERT_EQUAL(EFI_INVALID_PARAMETER, Status);

  // Invalid String Input High End
  DEBUG((DEBUG_ERROR,"■■■■\n"));
  Status = AmlOPDataString("■■■■", ListHead);
  UT_ASSERT_EQUAL(EFI_INVALID_PARAMETER, Status);

  if (!EFI_ERROR (Status)) {
    AmlDebugPrintLinkedObjects (ListHead);
  }
  AmlFreeObjectList (ListHead);

  return UNIT_TEST_PASSED;
}

/***************************************************************/
/* AML DefOPDataString Happy Path Tests                        */
/***************************************************************/

/***************************************************************/
// Name (TSTA, "")
/***************************************************************/
UNIT_TEST_STATUS
EFIAPI
TestEmptyString(
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

  // TSTA Name Start
  Status = AmlName(AmlStart, "TSTA", ListHead);
  UT_ASSERT_NOT_EFI_ERROR(Status);

    // Data String 0
    Status = AmlOPDataString("", ListHead);
    UT_ASSERT_NOT_EFI_ERROR(Status);

  // TSTA Name Close
  Status = AmlName(AmlClose, "TSTA", ListHead);
  UT_ASSERT_NOT_EFI_ERROR(Status);

  // Get first Child Node
  Node = GetFirstNode(ListHead);
  MainObject = AML_OBJECT_INSTANCE_FROM_LINK (Node);
  UT_ASSERT_NOT_NULL(MainObject->Data);
  UT_ASSERT_MEM_EQUAL(DefDataStrTestA, MainObject->Data, sizeof(DefDataStrTestA));
  UT_ASSERT_EQUAL(MainObject->DataSize, sizeof(DefDataStrTestA));

  if (!EFI_ERROR (Status)) {
    AmlDebugPrintLinkedObjects (ListHead);
  }

  AmlFreeObjectList (ListHead);

  return UNIT_TEST_PASSED;
}

/***************************************************************/
// Name (TSTB, "A")
/***************************************************************/
UNIT_TEST_STATUS
EFIAPI
TestShortString(
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

  // TSTB Name Start
  Status = AmlName(AmlStart, "TSTB", ListHead);
  UT_ASSERT_NOT_EFI_ERROR(Status);

    // Data String A Start
    Status = AmlOPDataString("A", ListHead);
    UT_ASSERT_NOT_EFI_ERROR(Status);

  // TSTB Name Close
  Status = AmlName(AmlClose, "TSTB", ListHead);
  UT_ASSERT_NOT_EFI_ERROR(Status);

  // Get first Child Node
  Node = GetFirstNode(ListHead);
  MainObject = AML_OBJECT_INSTANCE_FROM_LINK (Node);
  UT_ASSERT_NOT_NULL(MainObject->Data);
  UT_ASSERT_MEM_EQUAL(DefDataStrTestB, MainObject->Data, sizeof(DefDataStrTestB));
  UT_ASSERT_EQUAL(MainObject->DataSize, sizeof(DefDataStrTestB));

  if (!EFI_ERROR (Status)) {
    AmlDebugPrintLinkedObjects (ListHead);
  }

  AmlFreeObjectList (ListHead);

  return UNIT_TEST_PASSED;
}

///***************************************************************/
//// Name (TSTC, "ABCDEFGHIJKLMNOPQRSTUVWXYZ")
///***************************************************************/
UNIT_TEST_STATUS
EFIAPI
TestUpperCaseString(
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

  // TSTC Name Start
  Status = AmlName(AmlStart, "TSTC", ListHead);
  UT_ASSERT_NOT_EFI_ERROR(Status);

    // Data String "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
    Status = AmlOPDataString("ABCDEFGHIJKLMNOPQRSTUVWXYZ", ListHead);
    UT_ASSERT_NOT_EFI_ERROR(Status);

  // TSTC Name Close
  Status = AmlName(AmlClose, "TSTC", ListHead);
  UT_ASSERT_NOT_EFI_ERROR(Status);

  // Get first Child Node
  Node = GetFirstNode(ListHead);
  MainObject = AML_OBJECT_INSTANCE_FROM_LINK (Node);
  UT_ASSERT_NOT_NULL(MainObject->Data);
  UT_ASSERT_MEM_EQUAL(DefDataStrTestC, MainObject->Data, sizeof(DefDataStrTestC));
  UT_ASSERT_EQUAL(MainObject->DataSize, sizeof(DefDataStrTestC));

  if (!EFI_ERROR (Status)) {
    AmlDebugPrintLinkedObjects (ListHead);
  }

  AmlFreeObjectList (ListHead);

  return UNIT_TEST_PASSED;
}

/***************************************************************/
// Name (TSTD, "abcdefghijklmnopqrstuvwxyz")
/***************************************************************/
UNIT_TEST_STATUS
EFIAPI
TestLowerCaseString(
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
  // TSTD Name Start
  Status = AmlName(AmlStart, "TSTD", ListHead);
  UT_ASSERT_NOT_EFI_ERROR(Status);

    // Data String "abcdefghijklmnopqrstuvwxyz"
    Status = AmlOPDataString("abcdefghijklmnopqrstuvwxyz", ListHead);
    UT_ASSERT_NOT_EFI_ERROR(Status);

  // TSTD Name Close
  Status = AmlName(AmlClose, "TSTD", ListHead);
  UT_ASSERT_NOT_EFI_ERROR(Status);

  // Get first Child Node
  Node = GetFirstNode(ListHead);
  MainObject = AML_OBJECT_INSTANCE_FROM_LINK (Node);
  UT_ASSERT_NOT_NULL(MainObject->Data);
  UT_ASSERT_MEM_EQUAL(DefDataStrTestD, MainObject->Data, sizeof(DefDataStrTestD));
  UT_ASSERT_EQUAL(MainObject->DataSize, sizeof(DefDataStrTestD));

  if (!EFI_ERROR (Status)) {
    AmlDebugPrintLinkedObjects (ListHead);
  }

  AmlFreeObjectList (ListHead);

  return UNIT_TEST_PASSED;
}
/***************************************************************/
// Name (TSTE, "01234567890")
/***************************************************************/
UNIT_TEST_STATUS
EFIAPI
TestNumberString(
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

  // TSTE Name Start
  Status = AmlName(AmlStart, "TSTE", ListHead);
  UT_ASSERT_NOT_EFI_ERROR(Status);

    // Data String 01234567890
    Status = AmlOPDataString("01234567890", ListHead);
    UT_ASSERT_NOT_EFI_ERROR(Status);

  // TSTE Name Close
  Status = AmlName(AmlClose, "TSTE", ListHead);
  UT_ASSERT_NOT_EFI_ERROR(Status);

  // Get first Child Node
  Node = GetFirstNode(ListHead);
  MainObject = AML_OBJECT_INSTANCE_FROM_LINK (Node);
  UT_ASSERT_NOT_NULL(MainObject->Data);
  UT_ASSERT_MEM_EQUAL(DefDataStrTestE, MainObject->Data, sizeof(DefDataStrTestE));
  UT_ASSERT_EQUAL(MainObject->DataSize, sizeof(DefDataStrTestE));

  if (!EFI_ERROR (Status)) {
    AmlDebugPrintLinkedObjects (ListHead);
  }

  AmlFreeObjectList (ListHead);

  return UNIT_TEST_PASSED;
}
/***************************************************************/
// Name (TSTF, "A !#$%^*(\\)_+`-.,/?;:<>=?@{}[]|~")
/***************************************************************/
UNIT_TEST_STATUS
EFIAPI
TestSymbolString(
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

  // TSTF Name Start
  Status = AmlName(AmlStart, "TSTF", ListHead);
  UT_ASSERT_NOT_EFI_ERROR(Status);

    // Data String Symbols
    Status = AmlOPDataString("A !#$%^*(\\)_+`-.,/?;:<>=?@{}[]|~", ListHead);
    UT_ASSERT_NOT_EFI_ERROR(Status);

  // TSTF Name Close
  Status = AmlName(AmlClose, "TSTF", ListHead);
  UT_ASSERT_NOT_EFI_ERROR(Status);

  // Get first Child Node
  Node = GetFirstNode(ListHead);
  MainObject = AML_OBJECT_INSTANCE_FROM_LINK (Node);
  UT_ASSERT_NOT_NULL(MainObject->Data);
  UT_ASSERT_MEM_EQUAL(DefDataStrTestF, MainObject->Data, sizeof(DefDataStrTestF));
  UT_ASSERT_EQUAL(MainObject->DataSize, sizeof(DefDataStrTestF));

  if (!EFI_ERROR (Status)) {
    AmlDebugPrintLinkedObjects (ListHead);
  }

  AmlFreeObjectList (ListHead);

  return UNIT_TEST_PASSED;
}

/***************************************************************/
// Name (TSTG, "\a\b\f\n\r\t\v")
/***************************************************************/
UNIT_TEST_STATUS
EFIAPI
TestEscapeCharString(
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

  // TSTG Name Start
  Status = AmlName(AmlStart, "TSTG", ListHead);
  UT_ASSERT_NOT_EFI_ERROR(Status);

    // Data String "\a\b\f\n\r\t\v"
    Status = AmlOPDataString("\a\b\f\n\r\t\v", ListHead);
    UT_ASSERT_NOT_EFI_ERROR(Status);

  // TSTG Name Close
  Status = AmlName(AmlClose, "TSTG", ListHead);
  UT_ASSERT_NOT_EFI_ERROR(Status);

  // Get first Child Node
  Node = GetFirstNode(ListHead);
  MainObject = AML_OBJECT_INSTANCE_FROM_LINK (Node);
  UT_ASSERT_NOT_NULL(MainObject->Data);
  UT_ASSERT_MEM_EQUAL(DefDataStrTestG, MainObject->Data, sizeof(DefDataStrTestG));
  UT_ASSERT_EQUAL(MainObject->DataSize, sizeof(DefDataStrTestG));

  if (!EFI_ERROR (Status)) {
    AmlDebugPrintLinkedObjects (ListHead);
  }

  AmlFreeObjectList (ListHead);

  return UNIT_TEST_PASSED;
}

/***************************************************************/
// Creates and runs the DefDataString Test Suite
/***************************************************************/
EFI_STATUS
EFIAPI
DefDataStringTestMain (
  VOID
  )
{
  EFI_STATUS                  Status;
  UNIT_TEST_FRAMEWORK_HANDLE  Framework;
  UNIT_TEST_SUITE_HANDLE      DataStringTestSuite;


  DEBUG((DEBUG_INFO, "%a v%a\n", STRING_UNIT_TEST_NAME, STRING_UNIT_TEST_VERSION));

  //
  // Start setting up the test framework for running the tests.
  //
  Status = InitUnitTestFramework (&Framework, STRING_UNIT_TEST_NAME, gEfiCallerBaseName, STRING_UNIT_TEST_VERSION);
  if (EFI_ERROR(Status)) {
    DEBUG((DEBUG_ERROR, "Failed in InitUnitTestFramework. Status = %r\n", Status));
    goto Done;
  }

  ///
  // Test the DefDataString function
  //
  Status = CreateUnitTestSuite (&DataStringTestSuite, Framework, "DefDataString Test Suite", "AmdEdk2Common.Lib.AmlLib", NULL, NULL);
  if (EFI_ERROR(Status)) {
    DEBUG((DEBUG_ERROR, "Failed in CreateUnitTestSuite for DefDataString Test Suite\n"));
    Status = EFI_OUT_OF_RESOURCES;
    goto Done;
  }

  AddTestCase(DataStringTestSuite, "Test Input Validation",  "TestInputValidation",  StringTestInputValidation, NULL, NULL, NULL);
  AddTestCase(DataStringTestSuite, "Test Empty String",      "TestEmptyString",      TestEmptyString,           NULL, NULL, NULL);
  AddTestCase(DataStringTestSuite, "Test Short String",      "TestShortString",      TestShortString,           NULL, NULL, NULL);
  AddTestCase(DataStringTestSuite, "Test Upper Case String", "TestUpperCaseString",  TestUpperCaseString,       NULL, NULL, NULL);
  AddTestCase(DataStringTestSuite, "Test Lower Case String", "TestLowerCaseString",  TestLowerCaseString,       NULL, NULL, NULL);
  AddTestCase(DataStringTestSuite, "Test Number String",     "TestNumberString",     TestNumberString,          NULL, NULL, NULL);
  AddTestCase(DataStringTestSuite, "Test Symbol String",     "TestSymbolString",     TestSymbolString,          NULL, NULL, NULL);
  AddTestCase(DataStringTestSuite, "Test Escape Character",  "TestEscapeCharString", TestEscapeCharString,      NULL, NULL, NULL);

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
