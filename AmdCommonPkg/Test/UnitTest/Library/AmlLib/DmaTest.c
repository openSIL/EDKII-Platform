
/*****************************************************************************
 *
 * Copyright (C) 2020-2022 Advanced Micro Devices, Inc. All rights reserved.
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


#define DMA_UNIT_TEST_NAME        "DefDma Test Application"
#define DMA_UNIT_TEST_VERSION     "0.1"

UINT8   AslCompatMaster8[] = { 0x08,0x54,0x53,0x54,0x41,0x11,0x08,0x0A,0x05,0x2A,0x00,0x04,0x79,0x00 };

UINT8   AslTypeAMaster8[] = { 0x08,0x54,0x53,0x54,0x42,0x11,0x08,0x0A,0x05,0x2A,0x00,0x24,0x79,0x00 };

UINT8   AslTypeBMaster8[] = { 0x08,0x54,0x53,0x54,0x43,0x11,0x08,0x0A,0x05,0x2A,0x00,0x44,0x79,0x00 };

UINT8   AslTypeFMaster8[] = { 0x08,0x54,0x53,0x54,0x44,0x11,0x08,0x0A,0x05,0x2A,0x00,0x64,0x79,0x00 };

UINT8   AslCompatNotMaster8[] = { 0x08,0x54,0x53,0x54,0x45,0x11,0x08,0x0A,0x05,0x2A,0x00,0x00,0x79,0x00 };

UINT8   AslCompatNotMaster8_16[] = { 0x08,0x54,0x53,0x54,0x46,0x11,0x08,0x0A,0x05,0x2A,0x00,0x01,0x79,0x00 };

UINT8   AslCompatNotMaster16[] = { 0x08,0x54,0x53,0x54,0x47,0x11,0x08,0x0A,0x05,0x2A,0x00,0x02,0x79,0x00 };

UINT8   AslCompatNotMaster8Chan0[] = { 0x08,0x54,0x53,0x54,0x48,0x11,0x08,0x0A,0x05,0x2A,0x01,0x00,0x79,0x00 };

UINT8   AslCompatNotMaster8Chan7[] = { 0x08,0x54,0x53,0x54,0x49,0x11,0x08,0x0A,0x05,0x2A,0x80,0x00,0x79,0x00 };

UINT8   AslCompatNotMaster8ChanAll[] = { 0x08,0x54,0x53,0x54,0x4A,0x11,0x08,0x0A,0x05,0x2A,0xFF,0x00,0x79,0x00 };

/***************************************************************/
/* AML DefDma Input Validation Tests                    */
/***************************************************************/
UNIT_TEST_STATUS
EFIAPI
DmaTestInputValidation(
  IN  UNIT_TEST_CONTEXT   Context
)
{
  EFI_STATUS           Status;
  LIST_ENTRY           *ListHead;
  LIST_ENTRY           AmlListHead;

  ListHead = &AmlListHead;

  InitializeListHead (ListHead);

  // NULL List Head
  Status = AmlOPDma(Compatibility, NotBusMaster, Transfer8, 0, NULL);
  UT_ASSERT_EQUAL(EFI_INVALID_PARAMETER, Status);

  // Invalid DMA Type
  Status = AmlOPDma(0xF0, NotBusMaster, Transfer8, 0, ListHead);
  UT_ASSERT_EQUAL(EFI_INVALID_PARAMETER, Status);

  // Invalid Bus Master Input
  Status = AmlOPDma(Compatibility, 5, Transfer8, 0, ListHead);
  UT_ASSERT_EQUAL(EFI_INVALID_PARAMETER, Status);

  // Invalid Transfer Size
  Status = AmlOPDma(Compatibility, NotBusMaster, 3, 0, ListHead);
  UT_ASSERT_EQUAL(EFI_INVALID_PARAMETER, Status);

  if (!EFI_ERROR (Status)) {
    AmlDebugPrintLinkedObjects (ListHead);
  }
  AmlFreeObjectList (ListHead);

  return UNIT_TEST_PASSED;
}

/***************************************************************/
/* AML DefDma Happy Path Tests                        */
/***************************************************************/

/***************************************************************/
// Name(TSTA, ResourceTemplate()
/***************************************************************/
UNIT_TEST_STATUS
EFIAPI
TestCompatMaster8(
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

    // ResourceTemplate()
    Status = AmlResourceTemplate(AmlStart, ListHead);
    UT_ASSERT_NOT_EFI_ERROR(Status);

      Status = AmlOPDma(Compatibility, BusMaster, Transfer8, 0, ListHead);
      UT_ASSERT_NOT_EFI_ERROR(Status);

    Status = AmlResourceTemplate(AmlClose, ListHead);
    UT_ASSERT_NOT_EFI_ERROR(Status);

  // TSTA Name Close
  Status = AmlName(AmlClose, "TSTA", ListHead);
  UT_ASSERT_NOT_EFI_ERROR(Status);

  // Get first Child Node
  Node = GetFirstNode(ListHead);
  MainObject = AML_OBJECT_INSTANCE_FROM_LINK (Node);
  UT_ASSERT_NOT_NULL(MainObject->Data);
  UT_ASSERT_MEM_EQUAL(AslCompatMaster8, MainObject->Data, sizeof(AslCompatMaster8));
  UT_ASSERT_EQUAL(MainObject->DataSize, sizeof(AslCompatMaster8));

  if (!EFI_ERROR (Status)) {
    AmlDebugPrintLinkedObjects (ListHead);
  }
  AmlFreeObjectList (ListHead);

  return UNIT_TEST_PASSED;
}

/***************************************************************/
// Name(TSTB, ResourceTemplate()
/***************************************************************/
UNIT_TEST_STATUS
EFIAPI
TestTypeAMaster8(
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

    // ResourceTemplate()
    Status = AmlResourceTemplate(AmlStart, ListHead);
    UT_ASSERT_NOT_EFI_ERROR(Status);

      Status = AmlOPDma(TypeA, BusMaster, Transfer8, 0, ListHead);
      UT_ASSERT_NOT_EFI_ERROR(Status);

    Status = AmlResourceTemplate(AmlClose, ListHead);
    UT_ASSERT_NOT_EFI_ERROR(Status);

  // TSTB Name Close
  Status = AmlName(AmlClose, "TSTB", ListHead);
  UT_ASSERT_NOT_EFI_ERROR(Status);

  // Get first Child Node
  Node = GetFirstNode(ListHead);
  MainObject = AML_OBJECT_INSTANCE_FROM_LINK (Node);
  UT_ASSERT_NOT_NULL(MainObject->Data);
  UT_ASSERT_MEM_EQUAL(AslTypeAMaster8, MainObject->Data, sizeof(AslTypeAMaster8));
  UT_ASSERT_EQUAL(MainObject->DataSize, sizeof(AslTypeAMaster8));

  if (!EFI_ERROR (Status)) {
    AmlDebugPrintLinkedObjects (ListHead);
  }
  AmlFreeObjectList (ListHead);

  return UNIT_TEST_PASSED;
}

/***************************************************************/
// Name(TSTC, ResourceTemplate()
/***************************************************************/
UNIT_TEST_STATUS
EFIAPI
TestTypeBMaster8(
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

    // ResourceTemplate()
    Status = AmlResourceTemplate(AmlStart, ListHead);
    UT_ASSERT_NOT_EFI_ERROR(Status);

      Status = AmlOPDma(TypeB, BusMaster, Transfer8, 0, ListHead);
      UT_ASSERT_NOT_EFI_ERROR(Status);

    Status = AmlResourceTemplate(AmlClose, ListHead);
    UT_ASSERT_NOT_EFI_ERROR(Status);

  // TSTC Name Close
  Status = AmlName(AmlClose, "TSTC", ListHead);
  UT_ASSERT_NOT_EFI_ERROR(Status);

  // Get first Child Node
  Node = GetFirstNode(ListHead);
  MainObject = AML_OBJECT_INSTANCE_FROM_LINK (Node);
  UT_ASSERT_NOT_NULL(MainObject->Data);
  UT_ASSERT_MEM_EQUAL(AslTypeBMaster8, MainObject->Data, sizeof(AslTypeBMaster8));
  UT_ASSERT_EQUAL(MainObject->DataSize, sizeof(AslTypeBMaster8));

  if (!EFI_ERROR (Status)) {
    AmlDebugPrintLinkedObjects (ListHead);
  }
  AmlFreeObjectList (ListHead);

  return UNIT_TEST_PASSED;
}

/***************************************************************/
// Name(TSTD, ResourceTemplate()
/***************************************************************/
UNIT_TEST_STATUS
EFIAPI
TestTypeFMaster8(
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

    // ResourceTemplate()
    Status = AmlResourceTemplate(AmlStart, ListHead);
    UT_ASSERT_NOT_EFI_ERROR(Status);

      Status = AmlOPDma(TypeF, BusMaster, Transfer8, 0, ListHead);
      UT_ASSERT_NOT_EFI_ERROR(Status);

    Status = AmlResourceTemplate(AmlClose, ListHead);
    UT_ASSERT_NOT_EFI_ERROR(Status);

  // TSTD Name Close
  Status = AmlName(AmlClose, "TSTD", ListHead);
  UT_ASSERT_NOT_EFI_ERROR(Status);

  // Get first Child Node
  Node = GetFirstNode(ListHead);
  MainObject = AML_OBJECT_INSTANCE_FROM_LINK (Node);
  UT_ASSERT_NOT_NULL(MainObject->Data);
  UT_ASSERT_MEM_EQUAL(AslTypeFMaster8, MainObject->Data, sizeof(AslTypeFMaster8));
  UT_ASSERT_EQUAL(MainObject->DataSize, sizeof(AslTypeFMaster8));

  if (!EFI_ERROR (Status)) {
    AmlDebugPrintLinkedObjects (ListHead);
  }
  AmlFreeObjectList (ListHead);

  return UNIT_TEST_PASSED;
}

/***************************************************************/
// Name(TSTE, ResourceTemplate()
/***************************************************************/
UNIT_TEST_STATUS
EFIAPI
TestCompatNotMaster8(
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

    // ResourceTemplate()
    Status = AmlResourceTemplate(AmlStart, ListHead);
    UT_ASSERT_NOT_EFI_ERROR(Status);

      Status = AmlOPDma(Compatibility, NotBusMaster, Transfer8, 0, ListHead);
      UT_ASSERT_NOT_EFI_ERROR(Status);

    Status = AmlResourceTemplate(AmlClose, ListHead);
    UT_ASSERT_NOT_EFI_ERROR(Status);

  // TSTE Name Close
  Status = AmlName(AmlClose, "TSTE", ListHead);
  UT_ASSERT_NOT_EFI_ERROR(Status);

  // Get first Child Node
  Node = GetFirstNode(ListHead);
  MainObject = AML_OBJECT_INSTANCE_FROM_LINK (Node);
  UT_ASSERT_NOT_NULL(MainObject->Data);
  UT_ASSERT_MEM_EQUAL(AslCompatNotMaster8, MainObject->Data, sizeof(AslCompatNotMaster8));
  UT_ASSERT_EQUAL(MainObject->DataSize, sizeof(AslCompatNotMaster8));

  if (!EFI_ERROR (Status)) {
    AmlDebugPrintLinkedObjects (ListHead);
  }
  AmlFreeObjectList (ListHead);

  return UNIT_TEST_PASSED;
}

/***************************************************************/
// Name(TSTF, ResourceTemplate()
/***************************************************************/
UNIT_TEST_STATUS
EFIAPI
TestCompatNotMaster8_16(
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

    // ResourceTemplate()
    Status = AmlResourceTemplate(AmlStart, ListHead);
    UT_ASSERT_NOT_EFI_ERROR(Status);

      Status = AmlOPDma(Compatibility, NotBusMaster, Transfer8_16, 0, ListHead);
      UT_ASSERT_NOT_EFI_ERROR(Status);

    Status = AmlResourceTemplate(AmlClose, ListHead);
    UT_ASSERT_NOT_EFI_ERROR(Status);

  // TSTF Name Close
  Status = AmlName(AmlClose, "TSTF", ListHead);
  UT_ASSERT_NOT_EFI_ERROR(Status);

  // Get first Child Node
  Node = GetFirstNode(ListHead);
  MainObject = AML_OBJECT_INSTANCE_FROM_LINK (Node);
  UT_ASSERT_NOT_NULL(MainObject->Data);
  UT_ASSERT_MEM_EQUAL(AslCompatNotMaster8_16, MainObject->Data, sizeof(AslCompatNotMaster8_16));
  UT_ASSERT_EQUAL(MainObject->DataSize, sizeof(AslCompatNotMaster8_16));

  if (!EFI_ERROR (Status)) {
    AmlDebugPrintLinkedObjects (ListHead);
  }
  AmlFreeObjectList (ListHead);

  return UNIT_TEST_PASSED;
}

/***************************************************************/
// Name(TSTG, ResourceTemplate()
/***************************************************************/
UNIT_TEST_STATUS
EFIAPI
TestCompatNotMaster16(
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

    // ResourceTemplate()
    Status = AmlResourceTemplate(AmlStart, ListHead);
    UT_ASSERT_NOT_EFI_ERROR(Status);

      Status = AmlOPDma(Compatibility, NotBusMaster, Transfer16, 0, ListHead);
      UT_ASSERT_NOT_EFI_ERROR(Status);

    Status = AmlResourceTemplate(AmlClose, ListHead);
    UT_ASSERT_NOT_EFI_ERROR(Status);

  // TSTG Name Close
  Status = AmlName(AmlClose, "TSTG", ListHead);
  UT_ASSERT_NOT_EFI_ERROR(Status);


  // Get first Child Node
  Node = GetFirstNode(ListHead);
  MainObject = AML_OBJECT_INSTANCE_FROM_LINK (Node);
  UT_ASSERT_NOT_NULL(MainObject->Data);
  UT_ASSERT_MEM_EQUAL(AslCompatNotMaster16, MainObject->Data, sizeof(AslCompatNotMaster16));
  UT_ASSERT_EQUAL(MainObject->DataSize, sizeof(AslCompatNotMaster16));

  if (!EFI_ERROR (Status)) {
    AmlDebugPrintLinkedObjects (ListHead);
  }
  AmlFreeObjectList (ListHead);

  return UNIT_TEST_PASSED;
}

/***************************************************************/
// Name(TSTH, ResourceTemplate()
/***************************************************************/
UNIT_TEST_STATUS
EFIAPI
TestCompatNotMaster8Chan0(
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

  // TSTH Name Start
  Status = AmlName(AmlStart, "TSTH", ListHead);
  UT_ASSERT_NOT_EFI_ERROR(Status);

    // ResourceTemplate()
    Status = AmlResourceTemplate(AmlStart, ListHead);
    UT_ASSERT_NOT_EFI_ERROR(Status);

      Status = AmlOPDma(Compatibility, NotBusMaster, Transfer8, 1, ListHead);
      UT_ASSERT_NOT_EFI_ERROR(Status);

    Status = AmlResourceTemplate(AmlClose, ListHead);
    UT_ASSERT_NOT_EFI_ERROR(Status);

  // TSTH Name Close
  Status = AmlName(AmlClose, "TSTH", ListHead);
  UT_ASSERT_NOT_EFI_ERROR(Status);

  // Get first Child Node
  Node = GetFirstNode(ListHead);
  MainObject = AML_OBJECT_INSTANCE_FROM_LINK (Node);
  UT_ASSERT_NOT_NULL(MainObject->Data);
  UT_ASSERT_MEM_EQUAL(AslCompatNotMaster8Chan0, MainObject->Data, sizeof(AslCompatNotMaster8Chan0));
  UT_ASSERT_EQUAL(MainObject->DataSize, sizeof(AslCompatNotMaster8Chan0));

  if (!EFI_ERROR (Status)) {
    AmlDebugPrintLinkedObjects (ListHead);
  }
  AmlFreeObjectList (ListHead);

  return UNIT_TEST_PASSED;
}

/***************************************************************/
// Name(TSTI, ResourceTemplate()
/***************************************************************/
UNIT_TEST_STATUS
EFIAPI
TestCompatNotMaster8Chan7(
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

  // TSTI Name Start
  Status = AmlName(AmlStart, "TSTI", ListHead);
  UT_ASSERT_NOT_EFI_ERROR(Status);

    // ResourceTemplate()
    Status = AmlResourceTemplate(AmlStart, ListHead);
    UT_ASSERT_NOT_EFI_ERROR(Status);

      Status = AmlOPDma(Compatibility, NotBusMaster, Transfer8, 0x80, ListHead);
      UT_ASSERT_NOT_EFI_ERROR(Status);

    Status = AmlResourceTemplate(AmlClose, ListHead);
    UT_ASSERT_NOT_EFI_ERROR(Status);

  // TSTI Name Close
  Status = AmlName(AmlClose, "TSTI", ListHead);
  UT_ASSERT_NOT_EFI_ERROR(Status);

  // Get first Child Node
  Node = GetFirstNode(ListHead);
  MainObject = AML_OBJECT_INSTANCE_FROM_LINK (Node);
  UT_ASSERT_NOT_NULL(MainObject->Data);
  UT_ASSERT_MEM_EQUAL(AslCompatNotMaster8Chan7, MainObject->Data, sizeof(AslCompatNotMaster8Chan7));
  UT_ASSERT_EQUAL(MainObject->DataSize, sizeof(AslCompatNotMaster8Chan7));

  if (!EFI_ERROR (Status)) {
    AmlDebugPrintLinkedObjects (ListHead);
  }
  AmlFreeObjectList (ListHead);

  return UNIT_TEST_PASSED;
}

/***************************************************************/
// Name(TSTJ, ResourceTemplate()
/***************************************************************/
UNIT_TEST_STATUS
EFIAPI
TestCompatNotMaster8ChanAll(
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

  // TSTJ Name Start
  Status = AmlName(AmlStart, "TSTJ", ListHead);
  UT_ASSERT_NOT_EFI_ERROR(Status);

    // ResourceTemplate()
    Status = AmlResourceTemplate(AmlStart, ListHead);
    UT_ASSERT_NOT_EFI_ERROR(Status);

      Status = AmlOPDma(Compatibility, NotBusMaster, Transfer8, 0xFF, ListHead);
      UT_ASSERT_NOT_EFI_ERROR(Status);

    Status = AmlResourceTemplate(AmlClose, ListHead);
    UT_ASSERT_NOT_EFI_ERROR(Status);

  // TSTJ Name Close
  Status = AmlName(AmlClose, "TSTJ", ListHead);
  UT_ASSERT_NOT_EFI_ERROR(Status);

  // Get first Child Node
  Node = GetFirstNode(ListHead);
  MainObject = AML_OBJECT_INSTANCE_FROM_LINK (Node);
  UT_ASSERT_NOT_NULL(MainObject->Data);
  UT_ASSERT_MEM_EQUAL(AslCompatNotMaster8ChanAll, MainObject->Data, sizeof(AslCompatNotMaster8ChanAll));
  UT_ASSERT_EQUAL(MainObject->DataSize, sizeof(AslCompatNotMaster8ChanAll));

  if (!EFI_ERROR (Status)) {
    AmlDebugPrintLinkedObjects (ListHead);
  }
  AmlFreeObjectList (ListHead);

  return UNIT_TEST_PASSED;
}

/***************************************************************/
// Creates and runs the Dma Test Suite
/***************************************************************/
EFI_STATUS
EFIAPI
DmaTestMain (
  VOID
  )
{
  EFI_STATUS                  Status;
  UNIT_TEST_FRAMEWORK_HANDLE  Framework;
  UNIT_TEST_SUITE_HANDLE      DmaTestSuite;

  DEBUG((DEBUG_INFO, "%a v%a\n", DMA_UNIT_TEST_NAME, DMA_UNIT_TEST_VERSION));

  //
  // Start setting up the test framework for running the tests.
  //
  Status = InitUnitTestFramework (&Framework, DMA_UNIT_TEST_NAME, gEfiCallerBaseName, DMA_UNIT_TEST_VERSION);
  if (EFI_ERROR(Status)) {
    DEBUG((DEBUG_ERROR, "Failed in InitUnitTestFramework. Status = %r\n", Status));
    goto Done;
  }

  ///
  // Test the Dma function
  //
  Status = CreateUnitTestSuite (&DmaTestSuite, Framework, "Dma Test Suite", "AmdEdk2Common.Lib.AmlLib", NULL, NULL);
  if (EFI_ERROR(Status)) {
    DEBUG((DEBUG_ERROR, "Failed in CreateUnitTestSuite for Dma Test Suite\n"));
    Status = EFI_OUT_OF_RESOURCES;
    goto Done;
  }
  AddTestCase(DmaTestSuite, "Dma Test Input Validation",  "DmaTestInputValidation",  DmaTestInputValidation, NULL, NULL, NULL);

  AddTestCase(DmaTestSuite, "Name(TSTA, ResourceTemplate()",  "TestCompatMaster8",  TestCompatMaster8, NULL, NULL, NULL);
  AddTestCase(DmaTestSuite, "Name(TSTB, ResourceTemplate()",  "TestTypeAMaster8",  TestTypeAMaster8, NULL, NULL, NULL);
  AddTestCase(DmaTestSuite, "Name(TSTC, ResourceTemplate()",  "TestTypeBMaster8",  TestTypeBMaster8, NULL, NULL, NULL);
  AddTestCase(DmaTestSuite, "Name(TSTD, ResourceTemplate()",  "TestTypeFMaster8",  TestTypeFMaster8, NULL, NULL, NULL);
  AddTestCase(DmaTestSuite, "Name(TSTE, ResourceTemplate()",  "TestCompatNotMaster8",  TestCompatNotMaster8, NULL, NULL, NULL);
  AddTestCase(DmaTestSuite, "Name(TSTF, ResourceTemplate()",  "TestCompatNotMaster8_16",  TestCompatNotMaster8_16, NULL, NULL, NULL);
  AddTestCase(DmaTestSuite, "Name(TSTG, ResourceTemplate()",  "TestCompatNotMaster16",  TestCompatNotMaster16, NULL, NULL, NULL);
  AddTestCase(DmaTestSuite, "Name(TSTH, ResourceTemplate()",  "TestCompatNotMaster8Chan0",  TestCompatNotMaster8Chan0, NULL, NULL, NULL);
  AddTestCase(DmaTestSuite, "Name(TSTI, ResourceTemplate()",  "TestCompatNotMaster8Chan7",  TestCompatNotMaster8Chan7, NULL, NULL, NULL);
  AddTestCase(DmaTestSuite, "Name(TSTJ, ResourceTemplate()",  "TestCompatNotMaster8ChanAll",  TestCompatNotMaster8ChanAll, NULL, NULL, NULL);



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
