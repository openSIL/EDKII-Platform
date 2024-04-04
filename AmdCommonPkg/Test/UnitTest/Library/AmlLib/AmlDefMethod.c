/*****************************************************************************
 *
 * Copyright (C) 2020-2024 Advanced Micro Devices, Inc. All rights reserved.
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


#define METHOD_UNIT_TEST_NAME        "DefMethod Test Application"
#define METHOD_UNIT_TEST_VERSION     "0.1"


UINT8      DefMethodTestA[] = {0x14, 0x08, 0x54, 0x53, 0x54, 0x41, 0x0B, 0xA4,
                               0x00};

UINT8      DefMethodTestB[] = {0x14, 0x0C, 0x54, 0x53, 0x54, 0x42, 0x4F, 0xA4,
                               0x0D, 0x4F, 0x4E, 0x45, 0x00};

UINT8      DefMethodTestC[] = {0x14, 0x08, 0x54, 0x53, 0x54, 0x43, 0x07, 0xA4,
                               0x00};

UINT8      DefMethodTestD[] = {0x14, 0x34, 0x54, 0x53, 0x54, 0x44, 0x00, 0x08,
                               0x54, 0x4D, 0x50, 0x31, 0x01, 0x08, 0x54, 0x4D,
                               0x50, 0x32, 0x0D, 0x41, 0x4D, 0x44, 0x54, 0x45,
                               0x53, 0x54, 0x00, 0x08, 0x54, 0x4D, 0x50, 0x33,
                               0x0D, 0x41, 0x4D, 0x44, 0x20, 0x41, 0x4D, 0x4C,
                               0x20, 0x54, 0x45, 0x53, 0x54, 0x00, 0x08, 0x54,
                               0x4D, 0x50, 0x34, 0x0A, 0x02};

UINT8      DefMethodTestE[] = {0x14, 0x0A, 0x54, 0x53, 0x54, 0x45, 0x4F, 0x54,
                               0x53, 0x54, 0x44};

UINT8      DefMethodTestF[] = {0x14, 0x45, 0x04, 0x54, 0x53, 0x54, 0x46, 0x07,
                               0x08, 0x54, 0x4D, 0x50, 0x31, 0x01, 0x08, 0x54,
                               0x4D, 0x50, 0x32, 0x0D, 0x41, 0x4D, 0x44, 0x54,
                               0x45, 0x53, 0x54, 0x00, 0x08, 0x54, 0x4D, 0x50,
                               0x33, 0x0D, 0x41, 0x4D, 0x44, 0x20, 0x41, 0x4D,
                               0x4C, 0x20, 0x54, 0x45, 0x53, 0x54, 0x00, 0x08,
                               0x54, 0x4D, 0x50, 0x34, 0x0A, 0x02, 0x54, 0x53,
                               0x54, 0x41, 0x54, 0x4D, 0x50, 0x31, 0x54, 0x4D,
                               0x50, 0x32, 0x54, 0x4D, 0x50, 0x33 };

UINT8      DefMethodTestPPCV[] = {0x14, 0x0B, 0x5F, 0x50, 0x50, 0x43, 0x00,
                                  0xA4, 0x50, 0x50, 0x43, 0x56};

/***************************************************************/
/* AML DefMethod Input Validation Tests                        */
/***************************************************************/
UNIT_TEST_STATUS
EFIAPI
MethodTestInputValidation(
  IN  UNIT_TEST_CONTEXT   Context
)
{
  EFI_STATUS           Status;
  LIST_ENTRY           *ListHead;
  LIST_ENTRY           AmlListHead;
  CHAR8                Identifier[20];

  ListHead = &AmlListHead;

  InitializeListHead (ListHead);

  // Invalid Phase Input
  Status = AmlMethod(33, Identifier, 0, NotSerialized, 0, ListHead);
  UT_ASSERT_EQUAL(EFI_INVALID_PARAMETER, Status);

  // Invalid Name Input
  Status = AmlMethod(AmlStart, NULL, 0, NotSerialized, 0, ListHead);
  UT_ASSERT_EQUAL(EFI_INVALID_PARAMETER, Status);

  // Invalid Argument Count Input
  Status = AmlMethod(AmlStart, Identifier, 8, NotSerialized, 0, ListHead);
  UT_ASSERT_EQUAL(EFI_INVALID_PARAMETER, Status);

  // Invalid Sync Level Input
  Status = AmlMethod(AmlStart, Identifier, 0, NotSerialized, 16, ListHead);
  UT_ASSERT_EQUAL(EFI_INVALID_PARAMETER, Status);

  // NULL List Head
  Status = AmlMethod(AmlStart, Identifier, 0, NotSerialized, 0, NULL);
  UT_ASSERT_EQUAL(EFI_INVALID_PARAMETER, Status);

  // Close Before Start
  Status = AmlMethod(AmlClose, Identifier, 0, NotSerialized, 0, ListHead);
  UT_ASSERT_EQUAL(EFI_NOT_FOUND, Status);

  // Invalid Serialized Flag Value
  Status = AmlMethod(AmlStart, Identifier, 0, 57, 0, ListHead);
  UT_ASSERT_EQUAL(EFI_INVALID_PARAMETER, Status);

  // Happy Path Start
  Status = AmlMethod(AmlStart, Identifier, 0, NotSerialized, 0, ListHead);
  UT_ASSERT_EQUAL(EFI_SUCCESS, Status);

  // Happy Path Close
  Status = AmlMethod(AmlClose, Identifier, 0, NotSerialized, 0, ListHead);
  UT_ASSERT_EQUAL(EFI_SUCCESS, Status);


  if (!EFI_ERROR (Status)) {
    AmlDebugPrintLinkedObjects (ListHead);
  }
  AmlFreeObjectList (ListHead);

  return UNIT_TEST_PASSED;
}

/***************************************************************/
/* AML DefMethod Happy Path Tests                              */
/***************************************************************/
/***************************************************************/
//   Method(TSTA, 3, Serialized) {
//     Return(0)
//   }
/***************************************************************/
UNIT_TEST_STATUS
EFIAPI
TestMethodA(
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

  // Test Method A Start
  Status = AmlMethod(AmlStart, "TSTA", 3, Serialized, 0, ListHead);
  UT_ASSERT_NOT_EFI_ERROR(Status);

    // Return Start
    Status = AmlReturn(AmlStart, ListHead);
    UT_ASSERT_NOT_EFI_ERROR(Status);

    // Data Integer 1
      Status = AmlOPDataInteger(0, ListHead);
      UT_ASSERT_NOT_EFI_ERROR(Status);

    // Return Close
    Status = AmlReturn(AmlClose, ListHead);
    UT_ASSERT_NOT_EFI_ERROR(Status);

  // Test Method A Close
  Status = AmlMethod(AmlClose, "TSTA", 3, Serialized, 0, ListHead);
  UT_ASSERT_NOT_EFI_ERROR(Status);

  // Get first Child Node
  Node = GetFirstNode(ListHead);
  MainObject = AML_OBJECT_INSTANCE_FROM_LINK (Node);
  UT_ASSERT_NOT_NULL(MainObject->Data);
  UT_ASSERT_MEM_EQUAL(DefMethodTestA, MainObject->Data, sizeof(DefMethodTestA));
  UT_ASSERT_EQUAL(MainObject->DataSize, sizeof(DefMethodTestA));

  if (!EFI_ERROR (Status)) {
    AmlDebugPrintLinkedObjects (ListHead);
  }
  AmlFreeObjectList (ListHead);

  return UNIT_TEST_PASSED;
}

/***************************************************************/
//   Method(TSTB, 7, Serialized, 4) {
//     Return("ONE")
//   }
/***************************************************************/
UNIT_TEST_STATUS
EFIAPI
TestMethodB(
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

  // Test Method B Start
  Status = AmlMethod(AmlStart, "TSTB", 7, Serialized, 4, ListHead);
  UT_ASSERT_NOT_EFI_ERROR(Status);

    // Return ONE Start
    Status = AmlReturn(AmlStart, ListHead);
    UT_ASSERT_NOT_EFI_ERROR(Status);

      // Data String ONE
      Status = AmlOPDataString("ONE", ListHead);
      UT_ASSERT_NOT_EFI_ERROR(Status);

    // Return ONE Close
    Status = AmlReturn(AmlClose, ListHead);
    UT_ASSERT_NOT_EFI_ERROR(Status);

  // Test Method B Close
  Status = AmlMethod(AmlClose, "TSTB", 7, Serialized, 4, ListHead);
  UT_ASSERT_NOT_EFI_ERROR(Status);

  // Get first Child Node
  Node = GetFirstNode(ListHead);
  MainObject = AML_OBJECT_INSTANCE_FROM_LINK (Node);
  UT_ASSERT_NOT_NULL(MainObject->Data);
  UT_ASSERT_MEM_EQUAL(DefMethodTestB, MainObject->Data, sizeof(DefMethodTestB));
  UT_ASSERT_EQUAL(MainObject->DataSize, sizeof(DefMethodTestB));

  if (!EFI_ERROR (Status)) {
    AmlDebugPrintLinkedObjects (ListHead);
  }

  AmlFreeObjectList (ListHead);

  return UNIT_TEST_PASSED;
}

/***************************************************************/
//   Method(TSTC, 7, NotSerialized) {
//     Return(0)
//   }
/***************************************************************/
UNIT_TEST_STATUS
EFIAPI
TestMethodC(
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

  // Test Method C Start
  Status = AmlMethod(AmlStart, "TSTC", 7, NotSerialized, 0, ListHead);
  UT_ASSERT_NOT_EFI_ERROR(Status);

    // Return Start
    Status = AmlReturn(AmlStart, ListHead);
    UT_ASSERT_NOT_EFI_ERROR(Status);

    // Data Integer 1
      Status = AmlOPDataInteger(0, ListHead);
      UT_ASSERT_NOT_EFI_ERROR(Status);

    // Return Close
    Status = AmlReturn(AmlClose, ListHead);
    UT_ASSERT_NOT_EFI_ERROR(Status);

  // Test Method C Close
  Status = AmlMethod(AmlClose, "TSTC", 7, NotSerialized, 0, ListHead);
  UT_ASSERT_NOT_EFI_ERROR(Status);

  // Get first Child Node
  Node = GetFirstNode(ListHead);
  MainObject = AML_OBJECT_INSTANCE_FROM_LINK (Node);
  UT_ASSERT_NOT_NULL(MainObject->Data);
  UT_ASSERT_MEM_EQUAL(DefMethodTestC, MainObject->Data, sizeof(DefMethodTestC));
  UT_ASSERT_EQUAL(MainObject->DataSize, sizeof(DefMethodTestC));

  if (!EFI_ERROR (Status)) {
    AmlDebugPrintLinkedObjects (ListHead);
  }
  AmlFreeObjectList (ListHead);

  return UNIT_TEST_PASSED;
}

/***************************************************************/
//   Method(TSTD, 0, NotSerialized, 0) {
//     Name(TMP1, One)
//     Name(TMP2, "AMDTEST")
//     Name(TMP3, "AMD AML TEST")
//     Name(TMP4, 0x02)
//   }
/***************************************************************/
UNIT_TEST_STATUS
EFIAPI
TestMethodD(
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

  // Test Method D Start
  Status = AmlMethod(AmlStart, "TSTD", 0, NotSerialized, 0, ListHead);
  UT_ASSERT_NOT_EFI_ERROR(Status);

    // TMP1 Name Start
    Status = AmlName(AmlStart, "TMP1", ListHead);
    UT_ASSERT_NOT_EFI_ERROR(Status);

      // Data Integer 1
      Status = AmlOPDataInteger(1, ListHead);
      UT_ASSERT_NOT_EFI_ERROR(Status);

    // TMP1 Name Close
    Status = AmlName(AmlClose, "TMP1", ListHead);
    UT_ASSERT_NOT_EFI_ERROR(Status);

    // TMP2 Name Start
    Status = AmlName(AmlStart, "TMP2", ListHead);
    UT_ASSERT_NOT_EFI_ERROR(Status);

    // "AMDTEST" String
      Status = AmlOPDataString("AMDTEST", ListHead);
      UT_ASSERT_NOT_EFI_ERROR(Status);

    // TMP2 Name Close
    Status = AmlName(AmlClose, "TMP2", ListHead);
    UT_ASSERT_NOT_EFI_ERROR(Status);

    // TMP3 Name Start
    Status = AmlName(AmlStart, "TMP3", ListHead);
    UT_ASSERT_NOT_EFI_ERROR(Status);

      // "AMD AML TEST" String
      Status = AmlOPDataString("AMD AML TEST", ListHead);
      UT_ASSERT_NOT_EFI_ERROR(Status);

    // TMP3 Name Close
    Status = AmlName(AmlClose, "TMP3", ListHead);
    UT_ASSERT_NOT_EFI_ERROR(Status);

    // TMP4 Name Start
    Status = AmlName(AmlStart, "TMP4", ListHead);
    UT_ASSERT_NOT_EFI_ERROR(Status);

      // Data Integer 0x02
      Status = AmlOPDataInteger(0x02, ListHead);
      UT_ASSERT_NOT_EFI_ERROR(Status);

    // TMP4 Name Close
    Status = AmlName(AmlClose, "TMP4", ListHead);
    UT_ASSERT_NOT_EFI_ERROR(Status);

  // Test Method D Close
  Status = AmlMethod(AmlClose, "TSTD", 0, NotSerialized, 0, ListHead);
  UT_ASSERT_NOT_EFI_ERROR(Status);

  // Get first Child Node
  Node = GetFirstNode(ListHead);
  MainObject = AML_OBJECT_INSTANCE_FROM_LINK (Node);
  UT_ASSERT_NOT_NULL(MainObject->Data);
  UT_ASSERT_MEM_EQUAL(DefMethodTestD, MainObject->Data, sizeof(DefMethodTestD));
  UT_ASSERT_EQUAL(MainObject->DataSize, sizeof(DefMethodTestD));

  if (!EFI_ERROR (Status)) {
    AmlDebugPrintLinkedObjects (ListHead);
  }
  AmlFreeObjectList (ListHead);

  return UNIT_TEST_PASSED;
}

/***************************************************************/
//   Method(TSTE, 7, Serialized, 4) {
//     TSTD()
//   }
/***************************************************************/
UNIT_TEST_STATUS
EFIAPI
TestMethodE(
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

  // Test Method A Start
  Status = AmlMethod(AmlStart, "TSTE", 7, Serialized, 4, ListHead);
  UT_ASSERT_NOT_EFI_ERROR(Status);

    // TSTD()
    Status = AmlOPNameString("TSTD", ListHead);
    UT_ASSERT_NOT_EFI_ERROR(Status);

  // Test Method A Close
  Status = AmlMethod(AmlClose, "TSTE", 7, Serialized, 4, ListHead);
  UT_ASSERT_NOT_EFI_ERROR(Status);

  // Get first Child Node
  Node = GetFirstNode(ListHead);
  MainObject = AML_OBJECT_INSTANCE_FROM_LINK (Node);
  UT_ASSERT_NOT_NULL(MainObject->Data);
  UT_ASSERT_MEM_EQUAL(DefMethodTestE, MainObject->Data, sizeof(DefMethodTestE));
  UT_ASSERT_EQUAL(MainObject->DataSize, sizeof(DefMethodTestE));

  if (!EFI_ERROR (Status)) {
    AmlDebugPrintLinkedObjects (ListHead);
  }

  AmlFreeObjectList (ListHead);

  return UNIT_TEST_PASSED;
}


/***************************************************************/
//   Method(TSTF, 7, NotSerialized, 0) {
//     Name(TMP1, One)
//     Name(TMP2, "AMDTEST")
//     Name(TMP3, "AMD AML TEST")
//     Name(TMP4, 0x02)
//     TSTA(TMP1, TMP2, TMP3)
//   }
/***************************************************************/
UNIT_TEST_STATUS
EFIAPI
TestMethodF(
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

  // Test Method F Start
  Status = AmlMethod(AmlStart, "TSTF", 7, NotSerialized, 0, ListHead);
  UT_ASSERT_NOT_EFI_ERROR(Status);

    // TMP1 Name Start
    Status = AmlName(AmlStart, "TMP1", ListHead);
    UT_ASSERT_NOT_EFI_ERROR(Status);

    // Data Integer 1
      Status = AmlOPDataInteger(1, ListHead);
      UT_ASSERT_NOT_EFI_ERROR(Status);

    // TMP1 Name Close
    Status = AmlName(AmlClose, "TMP1", ListHead);
    UT_ASSERT_NOT_EFI_ERROR(Status);

    // TMP2 Name Start
    Status = AmlName(AmlStart, "TMP2", ListHead);
    UT_ASSERT_NOT_EFI_ERROR(Status);

    // "AMDTEST" String
      Status = AmlOPDataString("AMDTEST", ListHead);
      UT_ASSERT_NOT_EFI_ERROR(Status);

    // TMP2 Name Close
    Status = AmlName(AmlClose, "TMP2", ListHead);
    UT_ASSERT_NOT_EFI_ERROR(Status);

    // TMP3 Name Start
    Status = AmlName(AmlStart, "TMP3", ListHead);
    UT_ASSERT_NOT_EFI_ERROR(Status);

      // "AMD AML TEST" String
      Status = AmlOPDataString("AMD AML TEST", ListHead);
      UT_ASSERT_NOT_EFI_ERROR(Status);

    // TMP3 Name Close
    Status = AmlName(AmlClose, "TMP3", ListHead);
    UT_ASSERT_NOT_EFI_ERROR(Status);

    // TMP4 Name Start
    Status = AmlName(AmlStart, "TMP4", ListHead);
    UT_ASSERT_NOT_EFI_ERROR(Status);

      // Data Integer 0x02
      Status = AmlOPDataInteger(0x02, ListHead);
      UT_ASSERT_NOT_EFI_ERROR(Status);

    // TMP4 Name Close
    Status = AmlName(AmlClose, "TMP4", ListHead);
    UT_ASSERT_NOT_EFI_ERROR(Status);

    // TSTA Method Call
    Status = AmlOPNameString("TSTA", ListHead);
    UT_ASSERT_NOT_EFI_ERROR(Status);

    // TMP1 Argument
    Status = AmlOPNameString("TMP1", ListHead);
    UT_ASSERT_NOT_EFI_ERROR(Status);

    // TMP2 Argument
    Status = AmlOPNameString("TMP2", ListHead);
    UT_ASSERT_NOT_EFI_ERROR(Status);

    // TMP3 Argument
    Status = AmlOPNameString("TMP3", ListHead);
    UT_ASSERT_NOT_EFI_ERROR(Status);

  // Test Method F Close
  Status = AmlMethod(AmlClose, "TSTF", 7, NotSerialized, 0, ListHead);
  UT_ASSERT_NOT_EFI_ERROR(Status);

  // Get first Child Node
  Node = GetFirstNode(ListHead);
  MainObject = AML_OBJECT_INSTANCE_FROM_LINK (Node);
  UT_ASSERT_NOT_NULL(MainObject->Data);
  UT_ASSERT_MEM_EQUAL(DefMethodTestF, MainObject->Data, sizeof(DefMethodTestF));
  UT_ASSERT_EQUAL(MainObject->DataSize, sizeof(DefMethodTestF));

  if (!EFI_ERROR (Status)) {
    AmlDebugPrintLinkedObjects (ListHead);
  }
  AmlFreeObjectList (ListHead);

  return UNIT_TEST_PASSED;
}


/***************************************************************/
//   Name(PPCV, 0x00)    // untested line
//   Name(_PPC, 0, NotSerialized) {
//     Return(PPCV)
//   }
/***************************************************************/
UNIT_TEST_STATUS
EFIAPI
TestMethodPPC(
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

  // Test Method A Start
  Status = AmlMethod(AmlStart, "_PPC", 0, NotSerialized, 0, ListHead);
  UT_ASSERT_NOT_EFI_ERROR(Status);

    // Return PPCV Start
    Status = AmlReturn(AmlStart, ListHead);
    UT_ASSERT_NOT_EFI_ERROR(Status);

      // Data String PPCV
      Status = AmlOPNameString("PPCV", ListHead);
      UT_ASSERT_NOT_EFI_ERROR(Status);

    // Return PPCV Close
    Status = AmlReturn(AmlClose, ListHead);
    UT_ASSERT_NOT_EFI_ERROR(Status);

  // Test Method A Close
  Status = AmlMethod(AmlClose, "_PPC", 0, NotSerialized, 0, ListHead);
  UT_ASSERT_NOT_EFI_ERROR(Status);

  // Get first Child Node
  Node = GetFirstNode(ListHead);
  MainObject = AML_OBJECT_INSTANCE_FROM_LINK (Node);
  UT_ASSERT_NOT_NULL(MainObject->Data);
  UT_ASSERT_MEM_EQUAL(DefMethodTestPPCV, MainObject->Data, sizeof(DefMethodTestPPCV));
  UT_ASSERT_EQUAL(MainObject->DataSize, sizeof(DefMethodTestPPCV));

  if (!EFI_ERROR (Status)) {
    AmlDebugPrintLinkedObjects (ListHead);
  }

  AmlFreeObjectList (ListHead);

  return UNIT_TEST_PASSED;
}



/***************************************************************/
// Creates and runs the DefMethod Test Suite
/***************************************************************/
EFI_STATUS
EFIAPI
DefMethodTestMain (
  VOID
  )
{
  EFI_STATUS                  Status;
  UNIT_TEST_FRAMEWORK_HANDLE  Framework;
  UNIT_TEST_SUITE_HANDLE      MethodTestSuite;

  DEBUG((DEBUG_INFO, "%a v%a\n", METHOD_UNIT_TEST_NAME, METHOD_UNIT_TEST_VERSION));

  //
  // Start setting up the test framework for running the tests.
  //
  Status = InitUnitTestFramework (&Framework, METHOD_UNIT_TEST_NAME, gEfiCallerBaseName, METHOD_UNIT_TEST_VERSION);
  if (EFI_ERROR(Status)) {
    DEBUG((DEBUG_ERROR, "Failed in InitUnitTestFramework. Status = %r\n", Status));
    goto Done;
  }

  ///
  // Test the DefMethod function
  //
  Status = CreateUnitTestSuite (&MethodTestSuite, Framework, "DefMethod Test Suite", "AmdEdk2Common.Lib.AmlLib", NULL, NULL);
  if (EFI_ERROR(Status)) {
    DEBUG((DEBUG_ERROR, "Failed in CreateUnitTestSuite for DefMethod Test Suite\n"));
    Status = EFI_OUT_OF_RESOURCES;
    goto Done;
  }

//AddTestCase(MethodTestSuite, "Test Input Validation",   "TestInputValidation",    MethodTestInputValidation,    NULL, NULL, NULL);
  AddTestCase(MethodTestSuite, "Test Method A",         "TestMethodA",      TestMethodA,      NULL, NULL, NULL);
  AddTestCase(MethodTestSuite, "Test Method B",         "TestMethodB",      TestMethodB,      NULL, NULL, NULL);
  AddTestCase(MethodTestSuite, "Test Method C",         "TestMethodC",      TestMethodC,      NULL, NULL, NULL);
  AddTestCase(MethodTestSuite, "Test Method D",         "TestMethodd",      TestMethodD,      NULL, NULL, NULL);
  AddTestCase(MethodTestSuite, "Test Method E",         "TestMethodE",      TestMethodE,      NULL, NULL, NULL);
  AddTestCase(MethodTestSuite, "Test Method F",         "TestMethodF",      TestMethodF,      NULL, NULL, NULL);
  AddTestCase(MethodTestSuite, "Test Method PPC",       "TestMethodPPC",    TestMethodPPC,    NULL, NULL, NULL);

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

