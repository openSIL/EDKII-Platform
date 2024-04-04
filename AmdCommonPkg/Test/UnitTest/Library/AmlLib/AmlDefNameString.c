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


#define NAME_STRING_UNIT_TEST_NAME        "DefNameString Test Application"
#define NAME_STRING_UNIT_TEST_VERSION     "0.1"

UINT8  RootFill3_NameString[] = {0x5C, 0x5F, 0x5F, 0x5F, 0x5F};

UINT8  RootFill2_NameString[] = {0x5C, 0x5F, 0x5F, 0x5F, 0x5F};

UINT8  RootFill3NameString[]  = {0x5C, 0x41, 0x5F, 0x5F, 0x5F};

UINT8  RootFill2NameString[]  = {0x5C, 0x42, 0x43, 0x5F, 0x5F};

UINT8  RootFill1NameString[]  = {0x5C, 0x44, 0x45, 0x46, 0x5F};

UINT8  RootDualNameString[]   = {0x5C, 0x2E, 0x47, 0x48, 0x49, 0x5F, 0x4A,
                                 0x4B, 0x4C, 0x4D};

UINT8  RootMultiNameString[]  = {0x5C, 0x2F, 0x03, 0x47, 0x48, 0x49, 0x5F,
                                 0x4A, 0x4B, 0x4C, 0x4D, 0x4E, 0x4F, 0x50,
                                 0x45};

UINT8  MethodNameString[]       = {0x4D, 0x54, 0x48, 0x41};

UINT8  PrefixNameString[]       = {0x5E, 0x50, 0x43, 0x49, 0x31};

UINT8  PrefixDualNameString[]   = {0x5E, 0x2E, 0x50, 0x43, 0x49, 0x31, 0x4D,
                                  0x54, 0x48, 0x45};

UINT8  PrefixMultiNameString[]  = {0x5E, 0x2F, 0x03, 0x50, 0x43, 0x49, 0x31,
                                   0x44, 0x45, 0x56, 0x33, 0x4D, 0x54, 0x48,
                                   0x45};

UINT8  DualPrefixNameString[]      = {0x5E, 0x5E, 0x50, 0x43, 0x49, 0x31};

UINT8  DualPrefixDualNameString[]  = {0x5E, 0x5E, 0x2E, 0x50, 0x43, 0x49,
                                      0x31, 0x4D, 0x54, 0x48, 0x45};

UINT8  DualPrefixMultiNameString[] = {0x5E, 0x5E, 0x2F, 0x03, 0x50, 0x43,
                                      0x49, 0x31, 0x44, 0x45, 0x56, 0x33,
                                      0x4D, 0x54, 0x48, 0x45};

UINT8  TrioPrefixNameString[]      = {0x5E, 0x5E, 0x5E, 0x50, 0x43, 0x49,
                                      0x31};

UINT8  TrioPrefixDualNameString[]  = {0x5E, 0x5E, 0x5E, 0x2E, 0x50, 0x43, 0x49,
                                      0x31, 0x4D, 0x54, 0x48, 0x45};

UINT8  TrioPrefixMultiNameString[] = {0x5E, 0x5E, 0x5E, 0x2F, 0x03, 0x50, 0x43,
                                      0x49, 0x31, 0x44, 0x45, 0x56, 0x33, 0x4D,
                                      0x54, 0x48, 0x45};

UINT8  QuadPrefixMultiNameString[] = {0x5E, 0x5E, 0x5E, 0x5E, 0x2F, 0x03, 0x50,
                                      0x43, 0x49, 0x31, 0x44, 0x45, 0x56, 0x33,
                                      0x4D, 0x54, 0x48, 0x45};

UINT8  PentaPrefixMultiNameStringMethod[] = {0x5E, 0x5E, 0x5E, 0x5E, 0x5E, 0x2F,
                                             0x03, 0x50, 0x43, 0x49, 0x31, 0x44,
                                             0x45, 0x56, 0x33, 0x4D, 0x54, 0x48,
                                             0x45};

/***************************************************************/
/* AML DefNameString Input Validation Tests                   */
/***************************************************************/
UNIT_TEST_STATUS
EFIAPI
NameStringTestInputValidation(
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
  Status = AmlOPNameString("TEST", NULL);
  UT_ASSERT_EQUAL(EFI_INVALID_PARAMETER, Status);

  // NULL String Input
  DEBUG((DEBUG_ERROR,"NULL String\n"));
  Status = AmlOPNameString(NULL, ListHead);
  UT_ASSERT_EQUAL(EFI_INVALID_PARAMETER, Status);

  // Invalid String Input Bad Start
  DEBUG((DEBUG_ERROR,"aBCD\n"));
  Status = AmlOPNameString("aBCD", ListHead);
  UT_ASSERT_EQUAL(EFI_INVALID_PARAMETER, Status);

  // Invalid String Input lowercase
  DEBUG((DEBUG_ERROR,"\\Abc\n"));
  Status = AmlOPNameString("\\Abc", ListHead);
  UT_ASSERT_EQUAL(EFI_INVALID_PARAMETER, Status);

  // Invalid String NULL Root Char
  DEBUG((DEBUG_ERROR,"\\\n"));
  Status = AmlOPNameString("\\", ListHead);
  UT_ASSERT_EQUAL(EFI_INVALID_PARAMETER, Status);

  // Invalid String NULL Parent Prefix
  DEBUG((DEBUG_ERROR,"^\n"));
  Status = AmlOPNameString("^", ListHead);
  UT_ASSERT_EQUAL(EFI_INVALID_PARAMETER, Status);

  if (!EFI_ERROR (Status)) {
    AmlDebugPrintLinkedObjects (ListHead);
  }
  AmlFreeObjectList (ListHead);

  return UNIT_TEST_PASSED;
}

/***************************************************************/
/* AML DefOPNameString Happy Path Tests                        */
/***************************************************************/
/***************************************************************/
// External(\_, UnknownObj)
/***************************************************************/
UNIT_TEST_STATUS
EFIAPI
TestFill3_NameString(
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

  // Test Fill 3 _ Name
  Status = AmlOPNameString("\\_", ListHead);
  UT_ASSERT_NOT_EFI_ERROR(Status);

  // Get first Child Node
  Node = GetFirstNode(ListHead);
  MainObject = AML_OBJECT_INSTANCE_FROM_LINK (Node);
  UT_ASSERT_NOT_NULL(MainObject->Data);
  UT_ASSERT_MEM_EQUAL(RootFill3_NameString, MainObject->Data, sizeof(RootFill3_NameString));
  UT_ASSERT_EQUAL(MainObject->DataSize, sizeof(RootFill3_NameString));

  if (!EFI_ERROR (Status)) {
    AmlDebugPrintLinkedObjects (ListHead);
  }

  AmlFreeObjectList (ListHead);

  return UNIT_TEST_PASSED;
}

/***************************************************************/
// External(\__, UnknownObj)
/***************************************************************/
UNIT_TEST_STATUS
EFIAPI
TestFill2_NameString(
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

  // Test Fill 3 _ Name
  Status = AmlOPNameString("\\__", ListHead);
  UT_ASSERT_NOT_EFI_ERROR(Status);

  // Get first Child Node
  Node = GetFirstNode(ListHead);
  MainObject = AML_OBJECT_INSTANCE_FROM_LINK (Node);
  UT_ASSERT_NOT_NULL(MainObject->Data);
  UT_ASSERT_MEM_EQUAL(RootFill2_NameString, MainObject->Data, sizeof(RootFill2_NameString));
  UT_ASSERT_EQUAL(MainObject->DataSize, sizeof(RootFill2_NameString));

  if (!EFI_ERROR (Status)) {
    AmlDebugPrintLinkedObjects (ListHead);
  }

  AmlFreeObjectList (ListHead);

  return UNIT_TEST_PASSED;
}

///***************************************************************/
//// External(\A, UnknownObj)
///***************************************************************/
UNIT_TEST_STATUS
EFIAPI
TestFill3ANameString(
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

  // Test Fill 3 A Name
  Status = AmlOPNameString("\\A", ListHead);
  UT_ASSERT_NOT_EFI_ERROR(Status);

  // Get first Child Node
  Node = GetFirstNode(ListHead);
  MainObject = AML_OBJECT_INSTANCE_FROM_LINK (Node);
  UT_ASSERT_NOT_NULL(MainObject->Data);
  UT_ASSERT_MEM_EQUAL(RootFill3NameString, MainObject->Data, sizeof(RootFill3NameString));
  UT_ASSERT_EQUAL(MainObject->DataSize, sizeof(RootFill3NameString));

  if (!EFI_ERROR (Status)) {
    AmlDebugPrintLinkedObjects (ListHead);
  }

  AmlFreeObjectList (ListHead);

  return UNIT_TEST_PASSED;
}

/***************************************************************/
// External(\BC, UnknownObj)
/***************************************************************/
UNIT_TEST_STATUS
EFIAPI
TestFillTwoNameString(
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

  // Test Fill Two Name
  Status = AmlOPNameString("\\BC", ListHead);
  UT_ASSERT_NOT_EFI_ERROR(Status);

  // Get first Child Node
  Node = GetFirstNode(ListHead);
  MainObject = AML_OBJECT_INSTANCE_FROM_LINK (Node);
  UT_ASSERT_NOT_NULL(MainObject->Data);
  UT_ASSERT_MEM_EQUAL(RootFill2NameString, MainObject->Data, sizeof(RootFill2NameString));
  UT_ASSERT_EQUAL(MainObject->DataSize, sizeof(RootFill2NameString));

  if (!EFI_ERROR (Status)) {
    AmlDebugPrintLinkedObjects (ListHead);
  }

  AmlFreeObjectList (ListHead);

  return UNIT_TEST_PASSED;
}

/***************************************************************/
// External(\DEF, UnknownObj)
/***************************************************************/
UNIT_TEST_STATUS
EFIAPI
TestFillOneNameString(
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

  // Test Fill One Name String
  Status = AmlOPNameString("\\DEF", ListHead);
  UT_ASSERT_NOT_EFI_ERROR(Status);

  // Get first Child Node
  Node = GetFirstNode(ListHead);
  MainObject = AML_OBJECT_INSTANCE_FROM_LINK (Node);
  UT_ASSERT_NOT_NULL(MainObject->Data);
  UT_ASSERT_MEM_EQUAL(RootFill1NameString, MainObject->Data, sizeof(RootFill1NameString));
  UT_ASSERT_EQUAL(MainObject->DataSize, sizeof(RootFill1NameString));

  if (!EFI_ERROR (Status)) {
    AmlDebugPrintLinkedObjects (ListHead);
  }

  AmlFreeObjectList (ListHead);

  return UNIT_TEST_PASSED;
}
/***************************************************************/
// External(\GHI.JKLM, UnknownObj)
/***************************************************************/
UNIT_TEST_STATUS
EFIAPI
TestDualPathNameString(
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

  // Test Dual Path Name String
  Status = AmlOPNameString("\\GHI.JKLM", ListHead);
  UT_ASSERT_NOT_EFI_ERROR(Status);

  // Get first Child Node
  Node = GetFirstNode(ListHead);
  MainObject = AML_OBJECT_INSTANCE_FROM_LINK (Node);
  UT_ASSERT_NOT_NULL(MainObject->Data);
  UT_ASSERT_MEM_EQUAL(RootDualNameString, MainObject->Data, sizeof(RootDualNameString));
  UT_ASSERT_EQUAL(MainObject->DataSize, sizeof(RootDualNameString));

  if (!EFI_ERROR (Status)) {
    AmlDebugPrintLinkedObjects (ListHead);
  }

  AmlFreeObjectList (ListHead);

  return UNIT_TEST_PASSED;
}

/***************************************************************/
// External(\GHI.JKLM.NOPE, UnknownObj)
/***************************************************************/
UNIT_TEST_STATUS
EFIAPI
TestMultiPathNameString(
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

  // Test Multi Path Name String
  Status = AmlOPNameString("\\GHI.JKLM.NOPE", ListHead);
  UT_ASSERT_NOT_EFI_ERROR(Status);

  // Get first Child Node
  Node = GetFirstNode(ListHead);
  MainObject = AML_OBJECT_INSTANCE_FROM_LINK (Node);
  UT_ASSERT_NOT_NULL(MainObject->Data);
  UT_ASSERT_MEM_EQUAL(RootMultiNameString, MainObject->Data, sizeof(RootMultiNameString));
  UT_ASSERT_EQUAL(MainObject->DataSize, sizeof(RootMultiNameString));

  if (!EFI_ERROR (Status)) {
    AmlDebugPrintLinkedObjects (ListHead);
  }

  AmlFreeObjectList (ListHead);

  return UNIT_TEST_PASSED;
}

/***************************************************************/
// Return(MTHA())
/***************************************************************/
UNIT_TEST_STATUS
EFIAPI
TestMethodNameString(
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

  // Test MTHA() Name String
  Status = AmlOPNameString("MTHA()", ListHead);
  UT_ASSERT_NOT_EFI_ERROR(Status);

  // Get first Child Node
  Node = GetFirstNode(ListHead);
  MainObject = AML_OBJECT_INSTANCE_FROM_LINK (Node);
  UT_ASSERT_NOT_NULL(MainObject->Data);
  UT_ASSERT_MEM_EQUAL(MethodNameString, MainObject->Data, sizeof(MethodNameString));
  UT_ASSERT_EQUAL(MainObject->DataSize, sizeof(MethodNameString));

  if (!EFI_ERROR (Status)) {
    AmlDebugPrintLinkedObjects (ListHead);
  }

  AmlFreeObjectList (ListHead);

  return UNIT_TEST_PASSED;
}

/***************************************************************/
// Return(^PCI1)
/***************************************************************/
UNIT_TEST_STATUS
EFIAPI
TestPrefixNameString(
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

  // Test Prefix Name String
  Status = AmlOPNameString("^PCI1", ListHead);
  UT_ASSERT_NOT_EFI_ERROR(Status);

  // Get first Child Node
    AmlDebugPrintLinkedObjects (ListHead);
  Node = GetFirstNode(ListHead);
  MainObject = AML_OBJECT_INSTANCE_FROM_LINK (Node);
  UT_ASSERT_NOT_NULL(MainObject->Data);
  UT_ASSERT_MEM_EQUAL(PrefixNameString, MainObject->Data, sizeof(PrefixNameString));
  UT_ASSERT_EQUAL(MainObject->DataSize, sizeof(PrefixNameString));

  if (!EFI_ERROR (Status)) {
    AmlDebugPrintLinkedObjects (ListHead);
  }

  AmlFreeObjectList (ListHead);

  return UNIT_TEST_PASSED;
}

/***************************************************************/
// Return(^PCI1.MTHE)
/***************************************************************/
UNIT_TEST_STATUS
EFIAPI
TestPrefixDualNameString(
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

  // Test Prefix Name String
  Status = AmlOPNameString("^PCI1.MTHE", ListHead);
  UT_ASSERT_NOT_EFI_ERROR(Status);

  // Get first Child Node
    AmlDebugPrintLinkedObjects (ListHead);
  Node = GetFirstNode(ListHead);
  MainObject = AML_OBJECT_INSTANCE_FROM_LINK (Node);
  UT_ASSERT_NOT_NULL(MainObject->Data);
  UT_ASSERT_MEM_EQUAL(PrefixDualNameString, MainObject->Data, sizeof(PrefixDualNameString));
  UT_ASSERT_EQUAL(MainObject->DataSize, sizeof(PrefixDualNameString));

  if (!EFI_ERROR (Status)) {
    AmlDebugPrintLinkedObjects (ListHead);
  }

  AmlFreeObjectList (ListHead);

  return UNIT_TEST_PASSED;
}

/***************************************************************/
// Return(^PCI1.DEV3.MTHE)
/***************************************************************/
UNIT_TEST_STATUS
EFIAPI
TestPrefixMultiNameString(
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

  // Test Prefix Name String
  Status = AmlOPNameString("^PCI1.DEV3.MTHE", ListHead);
  UT_ASSERT_NOT_EFI_ERROR(Status);

  // Get first Child Node
    AmlDebugPrintLinkedObjects (ListHead);
  Node = GetFirstNode(ListHead);
  MainObject = AML_OBJECT_INSTANCE_FROM_LINK (Node);
  UT_ASSERT_NOT_NULL(MainObject->Data);
  UT_ASSERT_MEM_EQUAL(PrefixMultiNameString, MainObject->Data, sizeof(PrefixMultiNameString));
  UT_ASSERT_EQUAL(MainObject->DataSize, sizeof(PrefixMultiNameString));

  if (!EFI_ERROR (Status)) {
    AmlDebugPrintLinkedObjects (ListHead);
  }

  AmlFreeObjectList (ListHead);

  return UNIT_TEST_PASSED;
}

/***************************************************************/
// Return(^^PCI1)
/***************************************************************/
UNIT_TEST_STATUS
EFIAPI
TestDualPrefixNameString(
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

  // Test Prefix Name String
  Status = AmlOPNameString("^^PCI1", ListHead);
  UT_ASSERT_NOT_EFI_ERROR(Status);

  // Get first Child Node
    AmlDebugPrintLinkedObjects (ListHead);
  Node = GetFirstNode(ListHead);
  MainObject = AML_OBJECT_INSTANCE_FROM_LINK (Node);
  UT_ASSERT_NOT_NULL(MainObject->Data);
  UT_ASSERT_MEM_EQUAL(DualPrefixNameString, MainObject->Data, sizeof(DualPrefixNameString));
  UT_ASSERT_EQUAL(MainObject->DataSize, sizeof(DualPrefixNameString));

  if (!EFI_ERROR (Status)) {
    AmlDebugPrintLinkedObjects (ListHead);
  }

  AmlFreeObjectList (ListHead);

  return UNIT_TEST_PASSED;
}

/***************************************************************/
// Return(^^PCI1.MTHE)
/***************************************************************/
UNIT_TEST_STATUS
EFIAPI
TestDualPrefixDualNameString(
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

  // Test Prefix Name String
  Status = AmlOPNameString("^^PCI1.MTHE", ListHead);
  UT_ASSERT_NOT_EFI_ERROR(Status);

  // Get first Child Node
  Node = GetFirstNode(ListHead);
  MainObject = AML_OBJECT_INSTANCE_FROM_LINK (Node);
  UT_ASSERT_NOT_NULL(MainObject->Data);
  UT_ASSERT_MEM_EQUAL(DualPrefixDualNameString, MainObject->Data, sizeof(DualPrefixDualNameString));
  UT_ASSERT_EQUAL(MainObject->DataSize, sizeof(DualPrefixDualNameString));

  if (!EFI_ERROR (Status)) {
    AmlDebugPrintLinkedObjects (ListHead);
  }

  AmlFreeObjectList (ListHead);

  return UNIT_TEST_PASSED;
}

/***************************************************************/
// Return(^^PCI1.DEV3.MTHE)
/***************************************************************/
UNIT_TEST_STATUS
EFIAPI
TestDualPrefixMultiNameString(
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

  // Test Prefix Name String
  Status = AmlOPNameString("^^PCI1.DEV3.MTHE", ListHead);
  UT_ASSERT_NOT_EFI_ERROR(Status);

  // Get first Child Node
  Node = GetFirstNode(ListHead);
  MainObject = AML_OBJECT_INSTANCE_FROM_LINK (Node);
  UT_ASSERT_NOT_NULL(MainObject->Data);
  UT_ASSERT_MEM_EQUAL(DualPrefixMultiNameString, MainObject->Data, sizeof(DualPrefixMultiNameString));
  UT_ASSERT_EQUAL(MainObject->DataSize, sizeof(DualPrefixMultiNameString));

  if (!EFI_ERROR (Status)) {
    AmlDebugPrintLinkedObjects (ListHead);
  }

  AmlFreeObjectList (ListHead);

  return UNIT_TEST_PASSED;
}

/***************************************************************/
// Return(^^^PCI1)
/***************************************************************/
UNIT_TEST_STATUS
EFIAPI
TestTrioPrefixNameString(
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

  // Test Prefix Name String
  Status = AmlOPNameString("^^^PCI1", ListHead);
  UT_ASSERT_NOT_EFI_ERROR(Status);

  // Get first Child Node
  Node = GetFirstNode(ListHead);
  MainObject = AML_OBJECT_INSTANCE_FROM_LINK (Node);
  UT_ASSERT_NOT_NULL(MainObject->Data);
  UT_ASSERT_MEM_EQUAL(TrioPrefixNameString, MainObject->Data, sizeof(TrioPrefixNameString));
  UT_ASSERT_EQUAL(MainObject->DataSize, sizeof(TrioPrefixNameString));

  if (!EFI_ERROR (Status)) {
    AmlDebugPrintLinkedObjects (ListHead);
  }

  AmlFreeObjectList (ListHead);

  return UNIT_TEST_PASSED;
}

/***************************************************************/
// Return(^^^PCI1.MTHE)
/***************************************************************/
UNIT_TEST_STATUS
EFIAPI
TestTrioPrefixDualNameString(
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

  // Test Prefix Name String
  Status = AmlOPNameString("^^^PCI1.MTHE", ListHead);
  UT_ASSERT_NOT_EFI_ERROR(Status);

  // Get first Child Node
  Node = GetFirstNode(ListHead);
  MainObject = AML_OBJECT_INSTANCE_FROM_LINK (Node);
  UT_ASSERT_NOT_NULL(MainObject->Data);
  UT_ASSERT_MEM_EQUAL(TrioPrefixDualNameString, MainObject->Data, sizeof(TrioPrefixDualNameString));
  UT_ASSERT_EQUAL(MainObject->DataSize, sizeof(TrioPrefixDualNameString));

  if (!EFI_ERROR (Status)) {
    AmlDebugPrintLinkedObjects (ListHead);
  }

  AmlFreeObjectList (ListHead);

  return UNIT_TEST_PASSED;
}

/***************************************************************/
// Return(^^^PCI1.DEV3.MTHE)
/***************************************************************/
UNIT_TEST_STATUS
EFIAPI
TestTrioPrefixMultiNameString(
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

  // Test Prefix Name String
  Status = AmlOPNameString("^^^PCI1.DEV3.MTHE", ListHead);
  UT_ASSERT_NOT_EFI_ERROR(Status);

  // Get first Child Node
  Node = GetFirstNode(ListHead);
  MainObject = AML_OBJECT_INSTANCE_FROM_LINK (Node);
  UT_ASSERT_NOT_NULL(MainObject->Data);
  UT_ASSERT_MEM_EQUAL(TrioPrefixMultiNameString, MainObject->Data, sizeof(TrioPrefixMultiNameString));
  UT_ASSERT_EQUAL(MainObject->DataSize, sizeof(TrioPrefixMultiNameString));

  if (!EFI_ERROR (Status)) {
    AmlDebugPrintLinkedObjects (ListHead);
  }

  AmlFreeObjectList (ListHead);

  return UNIT_TEST_PASSED;
}

/***************************************************************/
// Return(^^^^PCI1.DEV3.MTHE)
/***************************************************************/
UNIT_TEST_STATUS
EFIAPI
TestQuadPrefixMultiNameString(
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

  // Test Prefix Name String
  Status = AmlOPNameString("^^^^PCI1.DEV3.MTHE", ListHead);
  UT_ASSERT_NOT_EFI_ERROR(Status);

  // Get first Child Node
  Node = GetFirstNode(ListHead);
  MainObject = AML_OBJECT_INSTANCE_FROM_LINK (Node);
  UT_ASSERT_NOT_NULL(MainObject->Data);
  UT_ASSERT_MEM_EQUAL(QuadPrefixMultiNameString, MainObject->Data, sizeof(QuadPrefixMultiNameString));
  UT_ASSERT_EQUAL(MainObject->DataSize, sizeof(QuadPrefixMultiNameString));

  if (!EFI_ERROR (Status)) {
    AmlDebugPrintLinkedObjects (ListHead);
  }

  AmlFreeObjectList (ListHead);

  return UNIT_TEST_PASSED;
}

/***************************************************************/
// Return(^^^^^PCI1.DEV3.MTHE())
/***************************************************************/
UNIT_TEST_STATUS
EFIAPI
TestPentaPrefixMultiNameString(
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

  // Test Prefix Name String
  Status = AmlOPNameString("^^^^^PCI1.DEV3.MTHE()", ListHead);
  UT_ASSERT_NOT_EFI_ERROR(Status);

  // Get first Child Node
  Node = GetFirstNode(ListHead);
  MainObject = AML_OBJECT_INSTANCE_FROM_LINK (Node);
  UT_ASSERT_NOT_NULL(MainObject->Data);
  UT_ASSERT_MEM_EQUAL(PentaPrefixMultiNameStringMethod, MainObject->Data, sizeof(PentaPrefixMultiNameStringMethod));
  UT_ASSERT_EQUAL(MainObject->DataSize, sizeof(PentaPrefixMultiNameStringMethod));

  if (!EFI_ERROR (Status)) {
    AmlDebugPrintLinkedObjects (ListHead);
  }

  AmlFreeObjectList (ListHead);

  return UNIT_TEST_PASSED;
}

/***************************************************************/
// Creates and runs the DefNameString Test Suite
/***************************************************************/
EFI_STATUS
EFIAPI
DefNameStringTestMain (
  VOID
  )
{
  EFI_STATUS                  Status;
  UNIT_TEST_FRAMEWORK_HANDLE  Framework;
  UNIT_TEST_SUITE_HANDLE      NameStringTestSuite;

  DEBUG((DEBUG_INFO, "%a v%a\n", NAME_STRING_UNIT_TEST_NAME, NAME_STRING_UNIT_TEST_VERSION));

  //
  // Start setting up the test framework for running the tests.
  //
  Status = InitUnitTestFramework (&Framework, NAME_STRING_UNIT_TEST_NAME, gEfiCallerBaseName, NAME_STRING_UNIT_TEST_VERSION);
  if (EFI_ERROR(Status)) {
    DEBUG((DEBUG_ERROR, "Failed in InitUnitTestFramework. Status = %r\n", Status));
    goto Done;
  }

  ///
  // Test the DefNameString function
  //
  Status = CreateUnitTestSuite (&NameStringTestSuite, Framework, "DefNameString Test Suite", "AmdEdk2Common.Lib.AmlLib", NULL, NULL);
  if (EFI_ERROR(Status)) {
    DEBUG((DEBUG_ERROR, "Failed in CreateUnitTestSuite for DefNameString Test Suite\n"));
    Status = EFI_OUT_OF_RESOURCES;
    goto Done;
  }

  AddTestCase(NameStringTestSuite, "Test Input Validation",              "TestInputValidation",             NameStringTestInputValidation,  NULL, NULL, NULL);
  AddTestCase(NameStringTestSuite, "Test Fill 3 _ Name String",          "TestFill3_NameString",            TestFill3_NameString,           NULL, NULL, NULL);
  AddTestCase(NameStringTestSuite, "Test Fill 2 _ Name String",          "TestFill2_NameString",            TestFill2_NameString,           NULL, NULL, NULL);
  AddTestCase(NameStringTestSuite, "Test Fill 3 A Name String",          "TestFill3ANameString",            TestFill3ANameString,           NULL, NULL, NULL);
  AddTestCase(NameStringTestSuite, "Test Fill 2 Name String",            "TestFillTwoNameString",           TestFillTwoNameString,          NULL, NULL, NULL);
  AddTestCase(NameStringTestSuite, "Test Fill 1 Name String",            "TestFillOneNameString",           TestFillOneNameString,          NULL, NULL, NULL);
  AddTestCase(NameStringTestSuite, "Test Dual Name String",              "TestDualPathNameString",          TestDualPathNameString,         NULL, NULL, NULL);
  AddTestCase(NameStringTestSuite, "Test Multi Name String",             "TestMultiPathNameString",         TestMultiPathNameString,        NULL, NULL, NULL);
  AddTestCase(NameStringTestSuite, "Test Method Name String",            "TestMethodNameString",            TestMethodNameString,           NULL, NULL, NULL);
  AddTestCase(NameStringTestSuite, "Test Prefix Name String",            "TestPrefixNameString",            TestPrefixNameString,           NULL, NULL, NULL);
  AddTestCase(NameStringTestSuite, "Test Prefix Dual Name String",       "TestPrefixDualNameString",        TestPrefixDualNameString,       NULL, NULL, NULL);
  AddTestCase(NameStringTestSuite, "Test Prefix Multi Name String",      "TestPrefixMultiNameString",       TestPrefixMultiNameString,      NULL, NULL, NULL);
  AddTestCase(NameStringTestSuite, "Test Dual Prefix Name String",       "TestDualPrefixNameString",        TestDualPrefixNameString,       NULL, NULL, NULL);
  AddTestCase(NameStringTestSuite, "Test Dual Prefix Dual Name String",  "TestDualPrefixDualNameString",    TestDualPrefixDualNameString,   NULL, NULL, NULL);
  AddTestCase(NameStringTestSuite, "Test Dual Prefix Multi Name String", "TestDualPrefixMultiNameString",   TestDualPrefixMultiNameString,  NULL, NULL, NULL);
  AddTestCase(NameStringTestSuite, "Test Trio Prefix Name String",       "TestTrioPrefixNameString",        TestTrioPrefixNameString,       NULL, NULL, NULL);
  AddTestCase(NameStringTestSuite, "Test Trio Prefix Dual Name String",  "TestTrioPrefixDualNameString",    TestTrioPrefixDualNameString,   NULL, NULL, NULL);
  AddTestCase(NameStringTestSuite, "Test Trio Prefix Multi Name String", "TestTrioPrefixMultiNameString",   TestTrioPrefixMultiNameString,  NULL, NULL, NULL);
  AddTestCase(NameStringTestSuite, "Test Quad Prefix Multi Name String", "TestQuadPrefixMultiNameString",   TestQuadPrefixMultiNameString,  NULL, NULL, NULL);
  AddTestCase(NameStringTestSuite, "Test Penta Prefix Multi Name String", "TestPentaPrefixMultiNameString", TestPentaPrefixMultiNameString, NULL, NULL, NULL);


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
