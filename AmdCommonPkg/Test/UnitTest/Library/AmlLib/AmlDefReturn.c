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


#define RETURN_UNIT_TEST_NAME        "DefReturn Test Application"
#define RETURN_UNIT_TEST_VERSION     "0.1"


UINT8      DefReturnZero[]     = {0xA4, 0x00};

UINT8      DefReturnInteger1[] = {0xA4, 0x01};

UINT8      DefReturnStringBuf0[] = {0xA4, 0x42, 0x55, 0x46, 0x30};

UINT8      DefReturnA053[] = {0xA4, 0x5C, 0x2F, 0x04, 0x5F, 0x53, 0x42, 0x5F,
                              0x41, 0x57, 0x52, 0x30, 0x41, 0x42, 0x52, 0x30,
                              0x41, 0x30, 0x35, 0x33};

/***************************************************************/
/* AML DefReturn Input Validation Tests                        */
/***************************************************************/
UNIT_TEST_STATUS
EFIAPI
ReturnTestInputValidation(
  IN  UNIT_TEST_CONTEXT   Context
)
{
  EFI_STATUS           Status;
  LIST_ENTRY           *ListHead;
  LIST_ENTRY           AmlListHead;

  ListHead = &AmlListHead;

  InitializeListHead (ListHead);

  // Invalid Phase Input
  Status = AmlReturn(33, ListHead);
  UT_ASSERT_EQUAL(EFI_INVALID_PARAMETER, Status);

  // NULL List Head
  Status = AmlReturn(AmlStart, NULL);
  UT_ASSERT_EQUAL(EFI_INVALID_PARAMETER, Status);

  // Close Before Start
  Status = AmlReturn(AmlClose, ListHead);
  UT_ASSERT_EQUAL(EFI_NOT_FOUND, Status);

  // Happy Path Start
  Status = AmlReturn(AmlStart, ListHead);
  UT_ASSERT_EQUAL(EFI_SUCCESS, Status);

  // Happy Path Close
  Status = AmlReturn(AmlClose, ListHead);
  UT_ASSERT_EQUAL(EFI_SUCCESS, Status);

  if (!EFI_ERROR (Status)) {
    AmlDebugPrintLinkedObjects (ListHead);
  }
  AmlFreeObjectList (ListHead);

  return UNIT_TEST_PASSED;
}

/***************************************************************/
/* AML DefReturn Happy Path Tests                              */
/***************************************************************/
/***************************************************************/
// Return()
/***************************************************************/
UNIT_TEST_STATUS
EFIAPI
TestNoReturnValue(
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

  // No Return Object Specified Start
  Status = AmlReturn(AmlStart, ListHead);
  UT_ASSERT_NOT_EFI_ERROR(Status);

  // No Return Object Specified Close
  Status = AmlReturn(AmlClose, ListHead);
  UT_ASSERT_NOT_EFI_ERROR(Status);

  // Get first Child Node
  Node = GetFirstNode(ListHead);
  MainObject = AML_OBJECT_INSTANCE_FROM_LINK (Node);
  UT_ASSERT_NOT_NULL(MainObject->Data);
  UT_ASSERT_MEM_EQUAL(DefReturnZero, MainObject->Data, 2);
  UT_ASSERT_EQUAL(MainObject->DataSize, 2);

  if (!EFI_ERROR (Status)) {
    AmlDebugPrintLinkedObjects (ListHead);
  }
  AmlFreeObjectList (ListHead);

  return UNIT_TEST_PASSED;
}

/***************************************************************/
// Return(0)
/***************************************************************/
UNIT_TEST_STATUS
EFIAPI
TestZeroReturnValue(
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

// Return 0 Start
  Status = AmlReturn(AmlStart, ListHead);
  UT_ASSERT_NOT_EFI_ERROR(Status);

    // Data Integer 1
    Status = AmlOPDataInteger(0, ListHead);
    UT_ASSERT_NOT_EFI_ERROR(Status);

  // Return Close
  Status = AmlReturn(AmlClose, ListHead);
  UT_ASSERT_NOT_EFI_ERROR(Status);

  // Get first Child Node
  Node = GetFirstNode(ListHead);
  MainObject = AML_OBJECT_INSTANCE_FROM_LINK (Node);
  UT_ASSERT_NOT_NULL(MainObject->Data);
  UT_ASSERT_MEM_EQUAL(DefReturnZero, MainObject->Data, 2);
  UT_ASSERT_EQUAL(MainObject->DataSize, 2);

  if (!EFI_ERROR (Status)) {
    AmlDebugPrintLinkedObjects (ListHead);
  }

  AmlFreeObjectList (ListHead);

  return UNIT_TEST_PASSED;
}

/***************************************************************/
// Return(1)
/***************************************************************/
UNIT_TEST_STATUS
EFIAPI
TestOneReturnValue(
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
/***************************************************************/
// Return 1 Start
  Status = AmlReturn(AmlStart, ListHead);
  UT_ASSERT_NOT_EFI_ERROR(Status);

    // Data Integer 1
    Status = AmlOPDataInteger(1, ListHead);
    UT_ASSERT_NOT_EFI_ERROR(Status);

  // Return Close
  Status = AmlReturn(AmlClose, ListHead);
  UT_ASSERT_NOT_EFI_ERROR(Status);

  // Get first Child Node
  Node = GetFirstNode(ListHead);
  MainObject = AML_OBJECT_INSTANCE_FROM_LINK (Node);
  UT_ASSERT_NOT_NULL(MainObject->Data);
  UT_ASSERT_MEM_EQUAL(DefReturnInteger1, MainObject->Data, 2);
  UT_ASSERT_EQUAL(MainObject->DataSize, 2);

  if (!EFI_ERROR (Status)) {
    AmlDebugPrintLinkedObjects (ListHead);
  }

  AmlFreeObjectList (ListHead);

  return UNIT_TEST_PASSED;
}

/***************************************************************/
// Return(BUF0)
/***************************************************************/
UNIT_TEST_STATUS
EFIAPI
TestBufferReturnValue(
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
  /***************************************************************/
  // Return 1 Start
  Status = AmlReturn(AmlStart, ListHead);
  UT_ASSERT_NOT_EFI_ERROR(Status);

    // BUF0 String
    Status = AmlOPNameString("BUF0", ListHead);
    UT_ASSERT_NOT_EFI_ERROR(Status);

  // Return Close
  Status = AmlReturn(AmlClose, ListHead);
  UT_ASSERT_NOT_EFI_ERROR(Status);

  // Get first Child Node
  Node = GetFirstNode(ListHead);
  MainObject = AML_OBJECT_INSTANCE_FROM_LINK (Node);
  UT_ASSERT_NOT_NULL(MainObject->Data);
  UT_ASSERT_MEM_EQUAL(DefReturnStringBuf0, MainObject->Data, sizeof(DefReturnStringBuf0));
  UT_ASSERT_EQUAL(MainObject->DataSize, sizeof(DefReturnStringBuf0));

  if (!EFI_ERROR (Status)) {
    AmlDebugPrintLinkedObjects (ListHead);
  }

  AmlFreeObjectList (ListHead);

  return UNIT_TEST_PASSED;
}

/***************************************************************/
// Return(\_SB.AWR0.ABR0.A053())
/***************************************************************/
UNIT_TEST_STATUS
EFIAPI
TestA053ReturnValue(
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
  /***************************************************************/
  // Return A053 Start
  Status = AmlReturn(AmlStart, ListHead);
  UT_ASSERT_NOT_EFI_ERROR(Status);

    // \\_SB.AWR0.ABR0.A053() String
    Status = AmlOPNameString("\\_SB.AWR0.ABR0.A053()", ListHead);
    UT_ASSERT_NOT_EFI_ERROR(Status);

  // Return Close
  Status = AmlReturn(AmlClose, ListHead);
  UT_ASSERT_NOT_EFI_ERROR(Status);

  // Get first Child Node
  Node = GetFirstNode(ListHead);
  MainObject = AML_OBJECT_INSTANCE_FROM_LINK (Node);
  UT_ASSERT_NOT_NULL(MainObject->Data);
  UT_ASSERT_MEM_EQUAL(DefReturnA053, MainObject->Data, sizeof(DefReturnA053));
  UT_ASSERT_EQUAL(MainObject->DataSize, sizeof(DefReturnA053));

  if (!EFI_ERROR (Status)) {
    AmlDebugPrintLinkedObjects (ListHead);
  }

  AmlFreeObjectList (ListHead);

  return UNIT_TEST_PASSED;
}



/***************************************************************/
// Creates and runs the DefReturn Test Suite
/***************************************************************/
EFI_STATUS
EFIAPI
DefReturnTestMain (
  VOID
  )
{
  EFI_STATUS                  Status;
  UNIT_TEST_FRAMEWORK_HANDLE  Framework;
  UNIT_TEST_SUITE_HANDLE      ReturnTestSuite;


  DEBUG((DEBUG_INFO, "%a v%a\n", RETURN_UNIT_TEST_NAME, RETURN_UNIT_TEST_VERSION));

  //
  // Start setting up the test framework for running the tests.
  //
  Status = InitUnitTestFramework (&Framework, RETURN_UNIT_TEST_NAME, gEfiCallerBaseName, RETURN_UNIT_TEST_VERSION);
  if (EFI_ERROR(Status)) {
    DEBUG((DEBUG_ERROR, "Failed in InitUnitTestFramework. Status = %r\n", Status));
    goto Done;
  }

  ///
  // Test the DefReturn function
  //
  Status = CreateUnitTestSuite (&ReturnTestSuite, Framework, "DefReturn Test Suite", "AmdEdk2Common.Lib.AmlLib", NULL, NULL);
  if (EFI_ERROR(Status)) {
    DEBUG((DEBUG_ERROR, "Failed in CreateUnitTestSuite for DefReturn Test Suite\n"));
    Status = EFI_OUT_OF_RESOURCES;
    goto Done;
  }

//AddTestCase(ReturnTestSuite, "Test Input Validation",          "TestInputValidation",    TestInputValidation,    NULL, NULL, NULL);
  AddTestCase(ReturnTestSuite, "Test No Return Value",           "TestNoReturnValue",      TestNoReturnValue,      NULL, NULL, NULL);
  AddTestCase(ReturnTestSuite, "Test Zero Return Value",         "TestZeroReturnValue",    TestZeroReturnValue,    NULL, NULL, NULL);
  AddTestCase(ReturnTestSuite, "Test One Return Value",          "TestOneReturnValue",     TestOneReturnValue,     NULL, NULL, NULL);
  AddTestCase(ReturnTestSuite, "Test Buffer Return Value",       "TestBufferReturnValue",  TestBufferReturnValue,  NULL, NULL, NULL);
  AddTestCase(ReturnTestSuite, "Test A053 Buffer Return Value",  "TestA053ReturnValue",    TestA053ReturnValue,    NULL, NULL, NULL);

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

