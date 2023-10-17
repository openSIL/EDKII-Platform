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

#define INTEGER_UNIT_TEST_NAME        "DefDataInteger Test Application"
#define INTEGER_UNIT_TEST_VERSION     "0.1"

#define INTEGER_DATA_OFFSET           5

UINT8      DefDataIntTestA[] = {0x08, 0x54, 0x53, 0x54, 0x41, 0x00};

UINT8      DefDataIntTestB[] = {0x08, 0x54, 0x53, 0x54, 0x42, 0x01};

UINT8      DefDataIntTestC[] = {0x08, 0x54, 0x53, 0x54, 0x43, 0x0A, 0x02};

UINT8      DefDataIntTestD[] = {0x08, 0x54, 0x53, 0x54, 0x44, 0x0A, 0xFF};

UINT8      DefDataIntTestE[] = {0x08, 0x54, 0x53, 0x54, 0x45, 0x0A, 0xFF};

UINT8      DefDataIntTestF[] = {0x08, 0x54, 0x53, 0x54, 0x46, 0x0B, 0x00,
                                0x01};

UINT8      DefDataIntTestG[] = {0x08, 0x54, 0x53, 0x54, 0x47, 0x0B, 0xFF,
                                0xFF};

UINT8      DefDataIntTestH[] = {0x08, 0x54, 0x53, 0x54, 0x48, 0x0B, 0xFF,
                                0xFF};

UINT8      DefDataIntTestI[] = {0x08, 0x54, 0x53, 0x54, 0x49, 0x0C, 0x00,
                                0x00, 0x01, 0x00};

UINT8      DefDataIntTestJ[] = {0x08, 0x54, 0x53, 0x54, 0x4A, 0x0C, 0xFF,
                                0xFF, 0xFF, 0xFF};

UINT8      DefDataIntTestK[] = {0x08, 0x54, 0x53, 0x54, 0x4B, 0x0C, 0xFF,
                                0xFF, 0xFF, 0xFF};

UINT8      DefDataIntTestL[] = {0x08, 0x54, 0x53, 0x54, 0x4C, 0x0E, 0x00,
                                0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00};

UINT8      DefDataIntTestM[] = {0x08, 0x54, 0x53, 0x54, 0x4D, 0x0E, 0xFE,
                                0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};

UINT8      DefDataIntTestN[] = {0x08, 0x54, 0x53, 0x54, 0x4E, 0x0E, 0xFE,
                                0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};

UINT8      DefDataIntTestO[] = {0x08, 0x54, 0x53, 0x54, 0x4F, 0xFF};

/***************************************************************/
/* AML DefDataInteger Input Validation Tests                   */
/***************************************************************/
UNIT_TEST_STATUS
EFIAPI
IntegerTestInputValidation(
  IN  UNIT_TEST_CONTEXT   Context
)
{
  EFI_STATUS           Status;
  LIST_ENTRY           *ListHead;
  LIST_ENTRY           AmlListHead;

  ListHead = &AmlListHead;

  InitializeListHead (ListHead);

  // NULL List Head
  Status = AmlOPDataInteger(0, NULL);
  UT_ASSERT_EQUAL(EFI_INVALID_PARAMETER, Status);

  if (!EFI_ERROR (Status)) {
    AmlDebugPrintLinkedObjects (ListHead);
  }
  AmlFreeObjectList (ListHead);
  return UNIT_TEST_PASSED;
}

/***************************************************************/
/* AML DefDataInteger Happy Path Tests                         */
/***************************************************************/

/***************************************************************/
// Name (TSTA, 0)
/***************************************************************/
UNIT_TEST_STATUS
EFIAPI
TestZeroDataInteger(
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

    // Data Integer 0
    Status = AmlOPDataInteger(0, ListHead);
    UT_ASSERT_NOT_EFI_ERROR(Status);

  // TSTA Name Close
  Status = AmlName(AmlClose, "TSTA", ListHead);
  UT_ASSERT_NOT_EFI_ERROR(Status);

  // Get first Child Node
  Node = GetFirstNode(ListHead);
  MainObject = AML_OBJECT_INSTANCE_FROM_LINK (Node);
  UT_ASSERT_NOT_NULL(MainObject->Data);
  UT_ASSERT_MEM_EQUAL(DefDataIntTestA, MainObject->Data, sizeof(DefDataIntTestA));
  UT_ASSERT_EQUAL(MainObject->DataSize, sizeof(DefDataIntTestA));

  if (!EFI_ERROR (Status)) {
    AmlDebugPrintLinkedObjects (ListHead);
  }

  AmlFreeObjectList (ListHead);

  return UNIT_TEST_PASSED;
}

/***************************************************************/
// Name (TSTB, 1)
/***************************************************************/
UNIT_TEST_STATUS
EFIAPI
TestOneDataInteger(
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

    // Data Integer 1
    Status = AmlOPDataInteger(1, ListHead);
    UT_ASSERT_NOT_EFI_ERROR(Status);

  // TSTB Name Close
  Status = AmlName(AmlClose, "TSTB", ListHead);
  UT_ASSERT_NOT_EFI_ERROR(Status);

  // Get first Child Node
  Node = GetFirstNode(ListHead);
  MainObject = AML_OBJECT_INSTANCE_FROM_LINK (Node);
  UT_ASSERT_NOT_NULL(MainObject->Data);
  UT_ASSERT_MEM_EQUAL(DefDataIntTestB, MainObject->Data, sizeof(DefDataIntTestB));
  UT_ASSERT_EQUAL(MainObject->DataSize, sizeof(DefDataIntTestB));

  if (!EFI_ERROR (Status)) {
    AmlDebugPrintLinkedObjects (ListHead);
  }

  AmlFreeObjectList (ListHead);

  return UNIT_TEST_PASSED;
}

/***************************************************************/
// Name (TSTC, 2)
/***************************************************************/
UNIT_TEST_STATUS
EFIAPI
TestMinByteDataInteger(
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

    // Data Integer 2
    Status = AmlOPDataInteger(2, ListHead);
    UT_ASSERT_NOT_EFI_ERROR(Status);

  // TSTC Name Close
  Status = AmlName(AmlClose, "TSTC", ListHead);
  UT_ASSERT_NOT_EFI_ERROR(Status);

  // Get first Child Node
  Node = GetFirstNode(ListHead);
  MainObject = AML_OBJECT_INSTANCE_FROM_LINK (Node);
  UT_ASSERT_NOT_NULL(MainObject->Data);
  UT_ASSERT_MEM_EQUAL(DefDataIntTestC, MainObject->Data, sizeof(DefDataIntTestC));
  UT_ASSERT_EQUAL(MainObject->DataSize, sizeof(DefDataIntTestC));

  if (!EFI_ERROR (Status)) {
    AmlDebugPrintLinkedObjects (ListHead);
  }

  AmlFreeObjectList (ListHead);

  return UNIT_TEST_PASSED;
}

/***************************************************************/
// Name (TSTD, 0xFF)
/***************************************************************/
UNIT_TEST_STATUS
EFIAPI
TestMaxByteDataInteger(
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

    // Data Integer 0xFF
    Status = AmlOPDataInteger(0xFF, ListHead);
    UT_ASSERT_NOT_EFI_ERROR(Status);

  // TSTD Name Close
  Status = AmlName(AmlClose, "TSTD", ListHead);
  UT_ASSERT_NOT_EFI_ERROR(Status);

  // Get first Child Node
  Node = GetFirstNode(ListHead);
  MainObject = AML_OBJECT_INSTANCE_FROM_LINK (Node);
  UT_ASSERT_NOT_NULL(MainObject->Data);
  UT_ASSERT_MEM_EQUAL(DefDataIntTestD, MainObject->Data, sizeof(DefDataIntTestD));
  UT_ASSERT_EQUAL(MainObject->DataSize, sizeof(DefDataIntTestD));

  if (!EFI_ERROR (Status)) {
    AmlDebugPrintLinkedObjects (ListHead);
  }

  AmlFreeObjectList (ListHead);

  return UNIT_TEST_PASSED;
}
/***************************************************************/
// Name (TSTE, 0x00FF)
/***************************************************************/
UNIT_TEST_STATUS
EFIAPI
TestLongByteDataInteger(
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

    // Data Integer 0x00FF
    Status = AmlOPDataInteger(0x00FF, ListHead);
    UT_ASSERT_NOT_EFI_ERROR(Status);

  // TSTE Name Close
  Status = AmlName(AmlClose, "TSTE", ListHead);
  UT_ASSERT_NOT_EFI_ERROR(Status);

  // Get first Child Node
  Node = GetFirstNode(ListHead);
  MainObject = AML_OBJECT_INSTANCE_FROM_LINK (Node);
  UT_ASSERT_NOT_NULL(MainObject->Data);
  UT_ASSERT_MEM_EQUAL(DefDataIntTestE, MainObject->Data, sizeof(DefDataIntTestE));
  UT_ASSERT_EQUAL(MainObject->DataSize, sizeof(DefDataIntTestE));

  if (!EFI_ERROR (Status)) {
    AmlDebugPrintLinkedObjects (ListHead);
  }

  AmlFreeObjectList (ListHead);

  return UNIT_TEST_PASSED;
}
/***************************************************************/
// Name (TSTF, 0x100)
/***************************************************************/
UNIT_TEST_STATUS
EFIAPI
TestMinWordDataInteger(
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

    // Data Integer 0x100
    Status = AmlOPDataInteger(0x100, ListHead);
    UT_ASSERT_NOT_EFI_ERROR(Status);

  // TSTF Name Close
  Status = AmlName(AmlClose, "TSTF", ListHead);
  UT_ASSERT_NOT_EFI_ERROR(Status);

  // Get first Child Node
  Node = GetFirstNode(ListHead);
  MainObject = AML_OBJECT_INSTANCE_FROM_LINK (Node);
  UT_ASSERT_NOT_NULL(MainObject->Data);
  UT_ASSERT_MEM_EQUAL(DefDataIntTestF, MainObject->Data, sizeof(DefDataIntTestF));
  UT_ASSERT_EQUAL(MainObject->DataSize, sizeof(DefDataIntTestF));

  if (!EFI_ERROR (Status)) {
    AmlDebugPrintLinkedObjects (ListHead);
  }

  AmlFreeObjectList (ListHead);

  return UNIT_TEST_PASSED;
}

/***************************************************************/
// Name (TSTG, 0xFFFF)
/***************************************************************/
UNIT_TEST_STATUS
EFIAPI
TestMaxWordDataInteger(
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

    // Data Integer 0xFFFF
    Status = AmlOPDataInteger(0xFFFF, ListHead);
    UT_ASSERT_NOT_EFI_ERROR(Status);

  // TSTG Name Close
  Status = AmlName(AmlClose, "TSTG", ListHead);
  UT_ASSERT_NOT_EFI_ERROR(Status);

  // Get first Child Node
  Node = GetFirstNode(ListHead);
  MainObject = AML_OBJECT_INSTANCE_FROM_LINK (Node);
  UT_ASSERT_NOT_NULL(MainObject->Data);
  UT_ASSERT_MEM_EQUAL(DefDataIntTestG, MainObject->Data, sizeof(DefDataIntTestG));
  UT_ASSERT_EQUAL(MainObject->DataSize, sizeof(DefDataIntTestG));

  if (!EFI_ERROR (Status)) {
    AmlDebugPrintLinkedObjects (ListHead);
  }

  AmlFreeObjectList (ListHead);

  return UNIT_TEST_PASSED;
}

/***************************************************************/
// Name (TSTH, 0x0000FFFF)
/***************************************************************/
UNIT_TEST_STATUS
EFIAPI
TestLongWordDataInteger(
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

    // Data Integer 0x0000FFFF
    Status = AmlOPDataInteger(0x0000FFFF, ListHead);
    UT_ASSERT_NOT_EFI_ERROR(Status);

  // TSTH Name Close
  Status = AmlName(AmlClose, "TSTH", ListHead);
  UT_ASSERT_NOT_EFI_ERROR(Status);

  // Get first Child Node
  Node = GetFirstNode(ListHead);
  MainObject = AML_OBJECT_INSTANCE_FROM_LINK (Node);
  UT_ASSERT_NOT_NULL(MainObject->Data);
  UT_ASSERT_MEM_EQUAL(DefDataIntTestH, MainObject->Data, sizeof(DefDataIntTestH));
  UT_ASSERT_EQUAL(MainObject->DataSize, sizeof(DefDataIntTestH));

  if (!EFI_ERROR (Status)) {
    AmlDebugPrintLinkedObjects (ListHead);
  }

  AmlFreeObjectList (ListHead);

  return UNIT_TEST_PASSED;
}

/***************************************************************/
// Name (TSTI, 0x10000)
/***************************************************************/
UNIT_TEST_STATUS
EFIAPI
TestMinDwordDataInteger(
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

    // Data Integer 0x10000
    Status = AmlOPDataInteger(0x10000, ListHead);
    UT_ASSERT_NOT_EFI_ERROR(Status);

  // TSTI Name Close
  Status = AmlName(AmlClose, "TSTI", ListHead);
  UT_ASSERT_NOT_EFI_ERROR(Status);

  // Get first Child Node
  Node = GetFirstNode(ListHead);
  MainObject = AML_OBJECT_INSTANCE_FROM_LINK (Node);
  UT_ASSERT_NOT_NULL(MainObject->Data);
  UT_ASSERT_MEM_EQUAL(DefDataIntTestI, MainObject->Data, sizeof(DefDataIntTestI));
  UT_ASSERT_EQUAL(MainObject->DataSize, sizeof(DefDataIntTestI));

  if (!EFI_ERROR (Status)) {
    AmlDebugPrintLinkedObjects (ListHead);
  }

  AmlFreeObjectList (ListHead);

  return UNIT_TEST_PASSED;
}

/***************************************************************/
// Name (TSTJ, 0xFFFFFFFF)
/***************************************************************/
UNIT_TEST_STATUS
EFIAPI
TestMaxDwordDataInteger(
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

    // Data Integer 0xFFFFFFFF
    Status = AmlOPDataInteger(0xFFFFFFFF, ListHead);
    UT_ASSERT_NOT_EFI_ERROR(Status);

    AmlDebugPrintLinkedObjects (ListHead);
  // TSTJ Name Close
  Status = AmlName(AmlClose, "TSTJ", ListHead);
  UT_ASSERT_NOT_EFI_ERROR(Status);

  // Get first Child Node
  Node = GetFirstNode(ListHead);

  MainObject = AML_OBJECT_INSTANCE_FROM_LINK (Node);
  UT_ASSERT_NOT_NULL(MainObject->Data);
  UT_ASSERT_MEM_EQUAL(DefDataIntTestJ, MainObject->Data, sizeof(DefDataIntTestJ));
  UT_ASSERT_EQUAL(MainObject->DataSize, sizeof(DefDataIntTestJ));

  if (!EFI_ERROR (Status)) {
    AmlDebugPrintLinkedObjects (ListHead);
  }

  AmlFreeObjectList (ListHead);

  return UNIT_TEST_PASSED;
}

/***************************************************************/
// Name (TSTK, 0x00000000FFFFFFFF)
/***************************************************************/
UNIT_TEST_STATUS
EFIAPI
TestLongDwordDataInteger(
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

  // TSTK Name Start
  Status = AmlName(AmlStart, "TSTK", ListHead);
  UT_ASSERT_NOT_EFI_ERROR(Status);

    // Data Integer 0x00000000FFFFFFFF
    Status = AmlOPDataInteger(0x00000000FFFFFFFF, ListHead);
    UT_ASSERT_NOT_EFI_ERROR(Status);

    AmlDebugPrintLinkedObjects (ListHead);
  // TSTK Name Close
  Status = AmlName(AmlClose, "TSTK", ListHead);
  UT_ASSERT_NOT_EFI_ERROR(Status);

    AmlDebugPrintLinkedObjects (ListHead);
  // Get first Child Node
  Node = GetFirstNode(ListHead);
  MainObject = AML_OBJECT_INSTANCE_FROM_LINK (Node);
  UT_ASSERT_NOT_NULL(MainObject->Data);
  UT_ASSERT_MEM_EQUAL(DefDataIntTestK, MainObject->Data, sizeof(DefDataIntTestK));
  UT_ASSERT_EQUAL(MainObject->DataSize, sizeof(DefDataIntTestK));

  if (!EFI_ERROR (Status)) {
    AmlDebugPrintLinkedObjects (ListHead);
  }

  AmlFreeObjectList (ListHead);

  return UNIT_TEST_PASSED;
}

/***************************************************************/
// Name (TSTL, 0x100000000)
/***************************************************************/
//UNIT_TEST_STATUS
//EFIAPI
//TestMinQwordDataInteger(
//  IN  UNIT_TEST_CONTEXT   Context
//)
//{
//  EFI_STATUS           Status;
//  LIST_ENTRY           *ListHead;
//  LIST_ENTRY           AmlListHead;
//  LIST_ENTRY           *Node;
//  AML_OBJECT_INSTANCE  *MainObject;
//
//  ListHead = &AmlListHead;
//
//  InitializeListHead (ListHead);
//
//  // TSTL Name Start
//  Status = AmlName(AmlStart, "TSTL", ListHead);
//  UT_ASSERT_NOT_EFI_ERROR(Status);
//
//    // Data Integer 0x100000000
//    Status = AmlOPDataInteger(0x100000000, ListHead);
//    UT_ASSERT_NOT_EFI_ERROR(Status);
//
//  // TSTL Name Close
//  Status = AmlName(AmlClose, "TSTL", ListHead);
//  UT_ASSERT_NOT_EFI_ERROR(Status);
//
//  // Get first Child Node
//  Node = GetNextNode(ListHead, ListHead);
//  MainObject = AML_OBJECT_INSTANCE_FROM_LINK (Node);
//  UT_ASSERT_NOT_NULL(MainObject->Data);
//  UT_ASSERT_MEM_EQUAL(DefDataIntTestL, MainObject->Data, sizeof(DefDataIntTestL));
//  UT_ASSERT_EQUAL(MainObject->DataSize, sizeof(DefDataIntTestL));
//
//  if (!EFI_ERROR (Status)) {
//    AmlDebugPrintLinkedObjects (ListHead);
//  }
//
//  AmlFreeObjectList (ListHead);
//  return UNIT_TEST_PASSED;
//}

/***************************************************************/
// Name (TSTM, 0xFFFFFFFFFFFFFFFE)
/***************************************************************/
UNIT_TEST_STATUS
EFIAPI
TestMaxQwordDataInteger(
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

  // TSTM Name Start
  Status = AmlName(AmlStart, "TSTM", ListHead);
  UT_ASSERT_NOT_EFI_ERROR(Status);

    // Data Integer 0xFFFFFFFFFFFFFFFE
    Status = AmlOPDataInteger(0xFFFFFFFFFFFFFFFE, ListHead);
    UT_ASSERT_NOT_EFI_ERROR(Status);

    AmlDebugPrintLinkedObjects (ListHead);
  // TSTM Name Close
  Status = AmlName(AmlClose, "TSTM", ListHead);
  UT_ASSERT_NOT_EFI_ERROR(Status);

    AmlDebugPrintLinkedObjects (ListHead);
  // Get first Child Node
  Node = GetFirstNode(ListHead);
  MainObject = AML_OBJECT_INSTANCE_FROM_LINK (Node);
  UT_ASSERT_NOT_NULL(MainObject->Data);
  UT_ASSERT_MEM_EQUAL(DefDataIntTestM, MainObject->Data, sizeof(DefDataIntTestM));
  UT_ASSERT_EQUAL(MainObject->DataSize, sizeof(DefDataIntTestM));

  if (!EFI_ERROR (Status)) {
    AmlDebugPrintLinkedObjects (ListHead);
  }

  AmlFreeObjectList (ListHead);

  return UNIT_TEST_PASSED;
}

/***************************************************************/
// Name (TSTN, 0x0000000000000000FFFFFFFFFFFFFFFE)
/***************************************************************/
UNIT_TEST_STATUS
EFIAPI
TestLongQwordDataInteger(
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

  // TSTN Name Start
  Status = AmlName(AmlStart, "TSTN", ListHead);
  UT_ASSERT_NOT_EFI_ERROR(Status);

    // Data Integer 0x0000000000000000FFFFFFFFFFFFFFFE
    Status = AmlOPDataInteger(0x0000000000000000FFFFFFFFFFFFFFFE, ListHead);
    UT_ASSERT_NOT_EFI_ERROR(Status);

    AmlDebugPrintLinkedObjects (ListHead);
  // TSTN Name Close
  Status = AmlName(AmlClose, "TSTN", ListHead);
  UT_ASSERT_NOT_EFI_ERROR(Status);

    AmlDebugPrintLinkedObjects (ListHead);
  // Get first Child Node
  Node = GetFirstNode(ListHead);
  MainObject = AML_OBJECT_INSTANCE_FROM_LINK (Node);
  UT_ASSERT_NOT_NULL(MainObject->Data);
  UT_ASSERT_MEM_EQUAL(DefDataIntTestN, MainObject->Data, sizeof(DefDataIntTestN));
  UT_ASSERT_EQUAL(MainObject->DataSize, sizeof(DefDataIntTestN));

  if (!EFI_ERROR (Status)) {
    AmlDebugPrintLinkedObjects (ListHead);
  }

  AmlFreeObjectList (ListHead);

  return UNIT_TEST_PASSED;
}

/***************************************************************/
// Name (TSTO, 0xFFFFFFFFFFFFFFFF)
/***************************************************************/
UNIT_TEST_STATUS
EFIAPI
TestAllOnesDataInteger(
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

  // TSTO Name Start
  Status = AmlName(AmlStart, "TSTO", ListHead);
  UT_ASSERT_NOT_EFI_ERROR(Status);

    // Data Integer 0xFFFFFFFFFFFFFFFF
    Status = AmlOPDataInteger(0xFFFFFFFFFFFFFFFF, ListHead);
    UT_ASSERT_NOT_EFI_ERROR(Status);

  // TSTO Name Close
  Status = AmlName(AmlClose, "TSTO", ListHead);
  UT_ASSERT_NOT_EFI_ERROR(Status);

  // Get first Child Node
  Node = GetFirstNode(ListHead);
  MainObject = AML_OBJECT_INSTANCE_FROM_LINK (Node);
  UT_ASSERT_NOT_NULL(MainObject->Data);
  UT_ASSERT_MEM_EQUAL(DefDataIntTestO, MainObject->Data, sizeof(DefDataIntTestO));
  UT_ASSERT_EQUAL(MainObject->DataSize, sizeof(DefDataIntTestO));

  if (!EFI_ERROR (Status)) {
    AmlDebugPrintLinkedObjects (ListHead);
  }

  AmlFreeObjectList (ListHead);

  return UNIT_TEST_PASSED;
}

/***************************************************************/
// Creates and runs the DefDataInteger Test Suite
/***************************************************************/
EFI_STATUS
EFIAPI
DefDataIntegerTestMain (
  VOID
  )
{
  EFI_STATUS                  Status;
  UNIT_TEST_FRAMEWORK_HANDLE  Framework;
  UNIT_TEST_SUITE_HANDLE      DataIntegerTestSuite;

  DEBUG((DEBUG_INFO, "%a v%a\n", INTEGER_UNIT_TEST_NAME, INTEGER_UNIT_TEST_VERSION));

  //
  // Start setting up the test framework for running the tests.
  //
  Status = InitUnitTestFramework (&Framework, INTEGER_UNIT_TEST_NAME, gEfiCallerBaseName, INTEGER_UNIT_TEST_VERSION);
  if (EFI_ERROR(Status)) {
    DEBUG((DEBUG_ERROR, "Failed in InitUnitTestFramework. Status = %r\n", Status));
    goto Done;
  }

  ///
  // Test the DefDataInteger function
  //
  Status = CreateUnitTestSuite (&DataIntegerTestSuite, Framework, "DefDataInteger Test Suite", "AmdEdk2Common.Lib.AmlLib", NULL, NULL);
  if (EFI_ERROR(Status)) {
    DEBUG((DEBUG_ERROR, "Failed in CreateUnitTestSuite for DefDataInteger Test Suite\n"));
    Status = EFI_OUT_OF_RESOURCES;
    goto Done;
  }

//AddTestCase(DataIntegerTestSuite, "Test Input Validation",          "TestInputValidation",    IntegerTestInputValidation,    NULL, NULL, NULL);
  AddTestCase(DataIntegerTestSuite, "Test Zero Data Integer",        "TestZeroDataInteger",      TestZeroDataInteger,      NULL, NULL, NULL);
  AddTestCase(DataIntegerTestSuite, "Test One Data Integer",         "TestOneDataInteger",       TestOneDataInteger,       NULL, NULL, NULL);
  AddTestCase(DataIntegerTestSuite, "Test Min Byte Data Integer",    "TestMinByteDataInteger",   TestMinByteDataInteger,   NULL, NULL, NULL);
  AddTestCase(DataIntegerTestSuite, "Test Max Byte Data Integer",    "TestMaxByteDataInteger",   TestMaxByteDataInteger,   NULL, NULL, NULL);
  AddTestCase(DataIntegerTestSuite, "Test Long Byte Data Integer",   "TestLongByteDataInteger",  TestLongByteDataInteger,  NULL, NULL, NULL);
  AddTestCase(DataIntegerTestSuite, "Test Min Word Data Integer",    "TestMinWordDataInteger",   TestMinWordDataInteger,   NULL, NULL, NULL);
  AddTestCase(DataIntegerTestSuite, "Test Max Word Data Integer",    "TestMaxWordDataInteger",   TestMaxWordDataInteger,   NULL, NULL, NULL);
  AddTestCase(DataIntegerTestSuite, "Test Long Word Data Integer",   "TestLongWordDataInteger",  TestLongWordDataInteger,  NULL, NULL, NULL);
  AddTestCase(DataIntegerTestSuite, "Test Min Dword Data Integer",   "TestMinDwordDataInteger",  TestMinDwordDataInteger,  NULL, NULL, NULL);
  AddTestCase(DataIntegerTestSuite, "Test Max Dword Data Integer",   "TestMaxDwordDataInteger",  TestMaxDwordDataInteger,  NULL, NULL, NULL);
  AddTestCase(DataIntegerTestSuite, "Test Long Dword Data Integer",  "TestLongDwordDataInteger", TestLongDwordDataInteger, NULL, NULL, NULL);
//  AddTestCase(DataIntegerTestSuite, "Test Min Qword Data Integer",   "TestMinQwordDataInteger",  TestMinQwordDataInteger,  NULL, NULL, NULL);
  AddTestCase(DataIntegerTestSuite, "Test Max Qword Data Integer",   "TestMaxQwordDataInteger",  TestMaxQwordDataInteger,  NULL, NULL, NULL);
  AddTestCase(DataIntegerTestSuite, "Test Long Qword Data Integer",  "TestLongQwordDataInteger", TestLongQwordDataInteger, NULL, NULL, NULL);
  AddTestCase(DataIntegerTestSuite, "Test All Ones Data Integer",    "TestAllOnesDataInteger",   TestAllOnesDataInteger,   NULL, NULL, NULL);

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

