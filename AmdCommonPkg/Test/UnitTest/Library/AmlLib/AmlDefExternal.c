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


#define EXTERNAL_UNIT_TEST_NAME        "DefExternal Test Application"
#define EXTERNAL_UNIT_TEST_VERSION     "0.1"

UINT8      DefExternalSb[]              = {0x15, 0x5C, 0x5F, 0x53, 0x42, 0x5F,
                                           0x06, 0x00};

UINT8      DefExternalPci0[]            = {0x15, 0x5C, 0x2E, 0x5F, 0x53, 0x42,
                                           0x5F, 0x50, 0x43, 0x49, 0x30, 0x06,
                                           0x00};

UINT8      DefUnknownExternal[]         = {0x15, 0x5C, 0x2F, 0x03, 0x5F, 0x53,
                                           0x42, 0x5F, 0x50, 0x43, 0x49, 0x30,
                                           0x54, 0x53, 0x54, 0x30, 0x00, 0x00};

UINT8      DefIntegerExternal[]         = {0x15, 0x5C, 0x2F, 0x03, 0x5F, 0x53,
                                           0x42, 0x5F, 0x50, 0x43, 0x49, 0x30,
                                           0x54, 0x53, 0x54, 0x31, 0x01, 0x00};

UINT8      DefStringExternal[]          = {0x15, 0x5C, 0x2F, 0x03, 0x5F, 0x53,
                                           0x42, 0x5F, 0x50, 0x43, 0x49, 0x30,
                                           0x54, 0x53, 0x54, 0x32, 0x02, 0x00};

UINT8      DefBufferExternal[]          = {0x15, 0x5C, 0x2F, 0x03, 0x5F, 0x53,
                                           0x42, 0x5F, 0x50, 0x43, 0x49, 0x30,
                                           0x54, 0x53, 0x54, 0x33, 0x03, 0x00};

UINT8      DefPackageExternal[]         = {0x15, 0x5C, 0x2F, 0x03, 0x5F, 0x53,
                                           0x42, 0x5F, 0x50, 0x43, 0x49, 0x30,
                                           0x54, 0x53, 0x54, 0x34, 0x04, 0x00};

UINT8      DefFieldUnitExternal[]       = {0x15, 0x5C, 0x2F, 0x03, 0x5F, 0x53,
                                           0x42, 0x5F, 0x50, 0x43, 0x49, 0x30,
                                           0x54, 0x53, 0x54, 0x35, 0x05, 0x00};

UINT8      DefDeviceExternal[]          = {0x15, 0x5C, 0x2F, 0x03, 0x5F, 0x53,
                                           0x42, 0x5F, 0x50, 0x43, 0x49, 0x30,
                                           0x54, 0x53, 0x54, 0x36, 0x06, 0x00};

UINT8      DefEventExternal[]           = {0x15, 0x5C, 0x2F, 0x03, 0x5F, 0x53,
                                           0x42, 0x5F, 0x50, 0x43, 0x49, 0x30,
                                           0x54, 0x53, 0x54, 0x37, 0x07, 0x00};

UINT8      DefMethodExternal[]          = {0x15, 0x5C, 0x2F, 0x03, 0x5F, 0x53,
                                           0x42, 0x5F, 0x50, 0x43, 0x49, 0x30,
                                           0x54, 0x53, 0x54, 0x38, 0x08, 0x00};

UINT8      DefMutexExternal[]           = {0x15, 0x5C, 0x2F, 0x03, 0x5F, 0x53,
                                           0x42, 0x5F, 0x50, 0x43, 0x49, 0x30,
                                           0x54, 0x53, 0x54, 0x39, 0x09, 0x00};

UINT8      DefOpRegionExternal[]        = {0x15, 0x5C, 0x2F, 0x03, 0x5F, 0x53,
                                           0x42, 0x5F, 0x50, 0x43, 0x49, 0x30,
                                           0x54, 0x53, 0x54, 0x41, 0x0A, 0x00};

UINT8      DefPowerResourceExternal[]   = {0x15, 0x5C, 0x2F, 0x03, 0x5F, 0x53,
                                           0x42, 0x5F, 0x50, 0x43, 0x49, 0x30,
                                           0x54, 0x53, 0x54, 0x42, 0x0B, 0x00};

UINT8      DefProcessorExternal[]       = {0x15, 0x5C, 0x2F, 0x03, 0x5F, 0x53,
                                           0x42, 0x5F, 0x50, 0x43, 0x49, 0x30,
                                           0x54, 0x53, 0x54, 0x43, 0x0C, 0x00};

UINT8      DefThermalZoneExternal[]     = {0x15, 0x5C, 0x2F, 0x03, 0x5F, 0x53,
                                           0x42, 0x5F, 0x50, 0x43, 0x49, 0x30,
                                           0x54, 0x53, 0x54, 0x44, 0x0D, 0x00};

UINT8      DefBufferFieldExternal[]     = {0x15, 0x5C, 0x2F, 0x03, 0x5F, 0x53,
                                           0x42, 0x5F, 0x50, 0x43, 0x49, 0x30,
                                           0x54, 0x53, 0x54, 0x45, 0x0E, 0x00};

UINT8      DefExternalLpc[]             = {0x15, 0x5C, 0x2F, 0x03, 0x5F, 0x53,
                                           0x42, 0x5F, 0x50, 0x43, 0x49, 0x30,
                                           0x4C, 0x50, 0x43, 0x42, 0x06, 0x00};

UINT8      DefExternalEc0[]             = {0x15, 0x5C, 0x2F, 0x04, 0x5F, 0x53,
                                           0x42, 0x5F, 0x50, 0x43, 0x49, 0x30,
                                           0x4C, 0x50, 0x43, 0x42, 0x45, 0x43,
                                           0x30, 0x5F, 0x06, 0x00};

UINT8      DefExternalTreeInt[]         = {0x15, 0x5C, 0x2F, 0x05, 0x5F, 0x53,
                                           0x42, 0x5F, 0x50, 0x43, 0x49, 0x30,
                                           0x4C, 0x50, 0x43, 0x42, 0x45, 0x43,
                                           0x30, 0x5F, 0x45, 0x43, 0x52, 0x47,
                                           0x01, 0x00};

UINT8      DefExternalTreeMux[]         = {0x15, 0x5C, 0x2F, 0x05, 0x5F, 0x53,
                                           0x42, 0x5F, 0x50, 0x43, 0x49, 0x30,
                                           0x4C, 0x50, 0x43, 0x42, 0x45, 0x43,
                                           0x30, 0x5F, 0x45, 0x43, 0x4D, 0x58,
                                           0x09, 0x00};

UINT8      DefExternalTreeMethod[]      = {0x15, 0x5C, 0x2F, 0x05, 0x5F, 0x53,
                                           0x42, 0x5F, 0x50, 0x43, 0x49, 0x30,
                                           0x4C, 0x50, 0x43, 0x42, 0x45, 0x43,
                                           0x30, 0x5F, 0x4D, 0x54, 0x48, 0x30,
                                           0x08, 0x00};

UINT8      DefExternalMethod1Arg[]      = {0x15, 0x5C, 0x2F, 0x05, 0x5F, 0x53,
                                           0x42, 0x5F, 0x50, 0x43, 0x49, 0x30,
                                           0x4C, 0x50, 0x43, 0x42, 0x45, 0x43,
                                           0x30, 0x5F, 0x4D, 0x54, 0x48, 0x31,
                                           0x08, 0x01};

UINT8      DefExternalMethod3Arg[]      = {0x15, 0x5C, 0x2F, 0x05, 0x5F, 0x53,
                                           0x42, 0x5F, 0x50, 0x43, 0x49, 0x30,
                                           0x4C, 0x50, 0x43, 0x42, 0x45, 0x43,
                                           0x30, 0x5F, 0x4D, 0x54, 0x48, 0x32,
                                           0x08, 0x03};

UINT8      DefExternalMethod7Arg[]      = {0x15, 0x5C, 0x2F, 0x05, 0x5F, 0x53,
                                           0x42, 0x5F, 0x50, 0x43, 0x49, 0x30,
                                           0x4C, 0x50, 0x43, 0x42, 0x45, 0x43,
                                           0x30, 0x5F, 0x4D, 0x54, 0x48, 0x33,
                                           0x08, 0x07};

/***************************************************************/
/* AML DefExternal Input Validation Tests                        */
/***************************************************************/
UNIT_TEST_STATUS
EFIAPI
ExternalTestInputValidation(
  IN  UNIT_TEST_CONTEXT   Context
)
{
  EFI_STATUS           Status;
  LIST_ENTRY           *ListHead;
  LIST_ENTRY           AmlListHead;

  ListHead = &AmlListHead;

  InitializeListHead (ListHead);


  // Invalid Object Type Input
  DEBUG((DEBUG_ERROR,"Test Input Validation - External\n"));
  DEBUG((DEBUG_ERROR,"\\_SB\n"));
  Status = AmlOPExternal("\\_SB", 33, 0, ListHead);
  UT_ASSERT_EQUAL(EFI_INVALID_PARAMETER, Status);

  // Invalid Method Arg Count (max of 7 args)
  DEBUG((DEBUG_ERROR,"Invalid arg count method\n"));
  Status = AmlOPExternal("ABCD", MethodObj, 33, ListHead);
  UT_ASSERT_EQUAL(EFI_INVALID_PARAMETER, Status);

  // Invalid Arg Count (max of 7 args) Ignored for Non-MethodObj
  DEBUG((DEBUG_ERROR,"Invalid Arg Count Non Method\n"));
  Status = AmlOPExternal("ABCD", UnknownObj, 33, ListHead);
  UT_ASSERT_EQUAL(EFI_INVALID_PARAMETER, Status);

  // Invalid Arg NULL Name
  DEBUG((DEBUG_ERROR,"NULL Name\n"));
  Status = AmlOPExternal(NULL, UnknownObj, 0, ListHead);
  UT_ASSERT_EQUAL(EFI_INVALID_PARAMETER, Status);

  // NULL List Head
  DEBUG((DEBUG_ERROR,"\\_SB - Null List Head\n"));
  Status = AmlOPExternal("\\_SB", UnknownObj, 0, NULL);
  UT_ASSERT_EQUAL(EFI_INVALID_PARAMETER, Status);

  if (!EFI_ERROR (Status)) {
    AmlDebugPrintLinkedObjects (ListHead);
  }
  AmlFreeObjectList (ListHead);

  return UNIT_TEST_PASSED;
}

/***************************************************************/
/* AML DefExternal Happy Path Tests                            */
/***************************************************************/
/***************************************************************/
// External(\_SB, DeviceObj)
/***************************************************************/
UNIT_TEST_STATUS
EFIAPI
TestExternalSb(
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


  // External SB Create
  Status = AmlOPExternal("\\_SB", DeviceObj, 0, ListHead);
  UT_ASSERT_NOT_EFI_ERROR(Status);

  // Get first Child Node
  Node = GetFirstNode(ListHead);
  MainObject = AML_OBJECT_INSTANCE_FROM_LINK (Node);
  UT_ASSERT_NOT_NULL(MainObject->Data);
  UT_ASSERT_MEM_EQUAL(DefExternalSb, MainObject->Data, sizeof(DefExternalSb));
  UT_ASSERT_EQUAL(MainObject->DataSize, sizeof(DefExternalSb));

  if (!EFI_ERROR (Status)) {
    AmlDebugPrintLinkedObjects (ListHead);
  }
  AmlFreeObjectList (ListHead);

  return UNIT_TEST_PASSED;
}

/***************************************************************/
// External(\_SB.PCI0, DeviceObj)
/***************************************************************/
UNIT_TEST_STATUS
EFIAPI
TestExternalPci0(
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


  // External PCI0 Create
  Status = AmlOPExternal("\\_SB.PCI0", DeviceObj, 0, ListHead);
  UT_ASSERT_NOT_EFI_ERROR(Status);

  // Get first Child Node
  Node = GetFirstNode(ListHead);
  MainObject = AML_OBJECT_INSTANCE_FROM_LINK (Node);
  UT_ASSERT_NOT_NULL(MainObject->Data);
  UT_ASSERT_MEM_EQUAL(DefExternalPci0, MainObject->Data, sizeof(DefExternalPci0));
  UT_ASSERT_EQUAL(MainObject->DataSize, sizeof(DefExternalPci0));

  if (!EFI_ERROR (Status)) {
    AmlDebugPrintLinkedObjects (ListHead);
  }

  AmlFreeObjectList (ListHead);

  return UNIT_TEST_PASSED;
}

/***************************************************************/
// External(\_SB.PCI0.TST0, UnknownObj)
/***************************************************************/
UNIT_TEST_STATUS
EFIAPI
TestUnknownExternal(
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


  // External TST0 Create
  Status = AmlOPExternal("\\_SB.PCI0.TST0", UnknownObj, 0, ListHead);
  UT_ASSERT_NOT_EFI_ERROR(Status);

  // Get first Child Node
  Node = GetFirstNode(ListHead);
  MainObject = AML_OBJECT_INSTANCE_FROM_LINK (Node);
  UT_ASSERT_NOT_NULL(MainObject->Data);
  UT_ASSERT_MEM_EQUAL(DefUnknownExternal, MainObject->Data, sizeof(DefUnknownExternal));
  UT_ASSERT_EQUAL(MainObject->DataSize, sizeof(DefUnknownExternal));

  if (!EFI_ERROR (Status)) {
    AmlDebugPrintLinkedObjects (ListHead);
  }

  AmlFreeObjectList (ListHead);

  return UNIT_TEST_PASSED;
}

/***************************************************************/
// External(\_SB.PCI0.TST1, IntObj)
/***************************************************************/
UNIT_TEST_STATUS
EFIAPI
TestIntegerExternal(
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

  // External SB Create
  Status = AmlOPExternal("\\_SB.PCI0.TST1", IntObj, 0, ListHead);
  UT_ASSERT_NOT_EFI_ERROR(Status);

  // Get first Child Node
  Node = GetFirstNode(ListHead);
  MainObject = AML_OBJECT_INSTANCE_FROM_LINK (Node);
  UT_ASSERT_NOT_NULL(MainObject->Data);
  UT_ASSERT_MEM_EQUAL(DefIntegerExternal, MainObject->Data, sizeof(DefIntegerExternal));
  UT_ASSERT_EQUAL(MainObject->DataSize, sizeof(DefIntegerExternal));

  if (!EFI_ERROR (Status)) {
    AmlDebugPrintLinkedObjects (ListHead);
  }

  AmlFreeObjectList (ListHead);

  return UNIT_TEST_PASSED;
}


/***************************************************************/
// External(\_SB.PCI0.TST3, StrObj)
/***************************************************************/
UNIT_TEST_STATUS
EFIAPI
TestStringExternal(
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


  // External SB Close
  Status = AmlOPExternal("\\_SB.PCI0.TST2", StrObj, 0, ListHead);
  UT_ASSERT_NOT_EFI_ERROR(Status);

  // Get first Child Node
  AmlDebugPrintLinkedObjects (ListHead);
  Node = GetFirstNode(ListHead);
  MainObject = AML_OBJECT_INSTANCE_FROM_LINK (Node);
  UT_ASSERT_NOT_NULL(MainObject->Data);
  UT_ASSERT_MEM_EQUAL(DefStringExternal, MainObject->Data, sizeof(DefStringExternal));
  UT_ASSERT_EQUAL(MainObject->DataSize, sizeof(DefStringExternal));

  if (!EFI_ERROR (Status)) {
    AmlDebugPrintLinkedObjects (ListHead);
  }

  AmlFreeObjectList (ListHead);

  return UNIT_TEST_PASSED;
}

/***************************************************************/
// External(\_SB.PCI0.TST3, BuffObj)
/***************************************************************/
UNIT_TEST_STATUS
EFIAPI
TestBufferExternal(
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


  // External SB Close
  Status = AmlOPExternal("\\_SB.PCI0.TST3", BuffObj, 0, ListHead);
  UT_ASSERT_NOT_EFI_ERROR(Status);

  // Get first Child Node
  Node = GetFirstNode(ListHead);
  MainObject = AML_OBJECT_INSTANCE_FROM_LINK (Node);
  UT_ASSERT_NOT_NULL(MainObject->Data);
  UT_ASSERT_MEM_EQUAL(DefBufferExternal, MainObject->Data, sizeof(DefBufferExternal));
  UT_ASSERT_EQUAL(MainObject->DataSize, sizeof(DefBufferExternal));

  if (!EFI_ERROR (Status)) {
    AmlDebugPrintLinkedObjects (ListHead);
  }

  AmlFreeObjectList (ListHead);

  return UNIT_TEST_PASSED;
}

/***************************************************************/
// External(\_SB.PCI0.TST4, PkgObj)
/***************************************************************/
UNIT_TEST_STATUS
EFIAPI
TestPackageExternal(
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


  // External SB Close
  Status = AmlOPExternal("\\_SB.PCI0.TST4", PkgObj, 0, ListHead);
  UT_ASSERT_NOT_EFI_ERROR(Status);

  // Get first Child Node
  Node = GetFirstNode(ListHead);
  MainObject = AML_OBJECT_INSTANCE_FROM_LINK (Node);
  UT_ASSERT_NOT_NULL(MainObject->Data);
  UT_ASSERT_MEM_EQUAL(DefPackageExternal, MainObject->Data, sizeof(DefPackageExternal));
  UT_ASSERT_EQUAL(MainObject->DataSize, sizeof(DefPackageExternal));

  if (!EFI_ERROR (Status)) {
    AmlDebugPrintLinkedObjects (ListHead);
  }

  AmlFreeObjectList (ListHead);

  return UNIT_TEST_PASSED;
}

/***************************************************************/
// External(\_SB.PCI0.TST5, FieldUnitObj)
/***************************************************************/
UNIT_TEST_STATUS
EFIAPI
TestFieldUnitExternal(
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


  // External TST5 Create
  Status = AmlOPExternal("\\_SB.PCI0.TST5", FieldUnitObj, 0, ListHead);
  UT_ASSERT_NOT_EFI_ERROR(Status);

  // Get first Child Node
  Node = GetFirstNode(ListHead);
  MainObject = AML_OBJECT_INSTANCE_FROM_LINK (Node);
  UT_ASSERT_NOT_NULL(MainObject->Data);
  UT_ASSERT_MEM_EQUAL(DefFieldUnitExternal, MainObject->Data, sizeof(DefFieldUnitExternal));
  UT_ASSERT_EQUAL(MainObject->DataSize, sizeof(DefFieldUnitExternal));

  if (!EFI_ERROR (Status)) {
    AmlDebugPrintLinkedObjects (ListHead);
  }

  AmlFreeObjectList (ListHead);

  return UNIT_TEST_PASSED;
}

/***************************************************************/
// External(\_SB.PCI0.TST6, DeviceObj)
/***************************************************************/
UNIT_TEST_STATUS
EFIAPI
TestDeviceExternal(
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


  // External TST6 Create
  Status = AmlOPExternal("\\_SB.PCI0.TST6", DeviceObj, 0, ListHead);
  UT_ASSERT_NOT_EFI_ERROR(Status);

  // Get first Child Node
  Node = GetFirstNode(ListHead);
  MainObject = AML_OBJECT_INSTANCE_FROM_LINK (Node);
  UT_ASSERT_NOT_NULL(MainObject->Data);
  UT_ASSERT_MEM_EQUAL(DefDeviceExternal, MainObject->Data, sizeof(DefDeviceExternal));
  UT_ASSERT_EQUAL(MainObject->DataSize, sizeof(DefDeviceExternal));

  if (!EFI_ERROR (Status)) {
    AmlDebugPrintLinkedObjects (ListHead);
  }

  AmlFreeObjectList (ListHead);

  return UNIT_TEST_PASSED;
}


/***************************************************************/
// External(\_SB.PCI0.TST7, PkgObj)
/***************************************************************/
UNIT_TEST_STATUS
EFIAPI
TestEventExternal(
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


  // External TST7 Create
  Status = AmlOPExternal("\\_SB.PCI0.TST7", EventObj, 0, ListHead);
  UT_ASSERT_NOT_EFI_ERROR(Status);

  // Get first Child Node
  Node = GetFirstNode(ListHead);
  MainObject = AML_OBJECT_INSTANCE_FROM_LINK (Node);
  UT_ASSERT_NOT_NULL(MainObject->Data);
  UT_ASSERT_MEM_EQUAL(DefEventExternal, MainObject->Data, sizeof(DefEventExternal));
  UT_ASSERT_EQUAL(MainObject->DataSize, sizeof(DefEventExternal));

  if (!EFI_ERROR (Status)) {
    AmlDebugPrintLinkedObjects (ListHead);
  }

  AmlFreeObjectList (ListHead);

  return UNIT_TEST_PASSED;
}


/***************************************************************/
// External(\_SB.PCI0.TST8, MethodObj)
/***************************************************************/
UNIT_TEST_STATUS
EFIAPI
TestMethodExternal(
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


  // External TST8 Create
  Status = AmlOPExternal("\\_SB.PCI0.TST8", MethodObj, 0, ListHead);
  UT_ASSERT_NOT_EFI_ERROR(Status);

  // Get first Child Node
  Node = GetFirstNode(ListHead);
  MainObject = AML_OBJECT_INSTANCE_FROM_LINK (Node);
  UT_ASSERT_NOT_NULL(MainObject->Data);
  UT_ASSERT_MEM_EQUAL(DefMethodExternal, MainObject->Data, sizeof(DefMethodExternal));
  UT_ASSERT_EQUAL(MainObject->DataSize, sizeof(DefMethodExternal));

  if (!EFI_ERROR (Status)) {
    AmlDebugPrintLinkedObjects (ListHead);
  }

  AmlFreeObjectList (ListHead);

  return UNIT_TEST_PASSED;
}


/***************************************************************/
// External(\_SB.PCI0.TST9, MutexObj)
/***************************************************************/
UNIT_TEST_STATUS
EFIAPI
TestMutexExternal(
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


  // External TST9 Create
  Status = AmlOPExternal("\\_SB.PCI0.TST9", MutexObj, 0, ListHead);
  UT_ASSERT_NOT_EFI_ERROR(Status);

  // Get first Child Node
  Node = GetFirstNode(ListHead);
  MainObject = AML_OBJECT_INSTANCE_FROM_LINK (Node);
  UT_ASSERT_NOT_NULL(MainObject->Data);
  UT_ASSERT_MEM_EQUAL(DefMutexExternal, MainObject->Data, sizeof(DefMutexExternal));
  UT_ASSERT_EQUAL(MainObject->DataSize, sizeof(DefMutexExternal));

  if (!EFI_ERROR (Status)) {
    AmlDebugPrintLinkedObjects (ListHead);
  }

  AmlFreeObjectList (ListHead);

  return UNIT_TEST_PASSED;
}

/***************************************************************/
// External(\_SB.PCI0.TSTA, OpRegionObj)
/***************************************************************/
UNIT_TEST_STATUS
EFIAPI
TestOpRegionExternal(
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


  // External TSTA Create
  Status = AmlOPExternal("\\_SB.PCI0.TSTA", OpRegionObj, 0, ListHead);
  UT_ASSERT_NOT_EFI_ERROR(Status);

  // Get first Child Node
  Node = GetFirstNode(ListHead);
  MainObject = AML_OBJECT_INSTANCE_FROM_LINK (Node);
  UT_ASSERT_NOT_NULL(MainObject->Data);
  UT_ASSERT_MEM_EQUAL(DefOpRegionExternal, MainObject->Data, sizeof(DefOpRegionExternal));
  UT_ASSERT_EQUAL(MainObject->DataSize, sizeof(DefOpRegionExternal));

  if (!EFI_ERROR (Status)) {
    AmlDebugPrintLinkedObjects (ListHead);
  }

  AmlFreeObjectList (ListHead);

  return UNIT_TEST_PASSED;
}

/***************************************************************/
// External(\_SB.PCI0.TSTB, PowerResObj)
/***************************************************************/
UNIT_TEST_STATUS
EFIAPI
TestPowerResourceExternal(
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


  // External TSTB Create
  Status = AmlOPExternal("\\_SB.PCI0.TSTB", PowerResObj, 0, ListHead);
  UT_ASSERT_NOT_EFI_ERROR(Status);

  // Get first Child Node
  Node = GetFirstNode(ListHead);
  MainObject = AML_OBJECT_INSTANCE_FROM_LINK (Node);
  UT_ASSERT_NOT_NULL(MainObject->Data);
  UT_ASSERT_MEM_EQUAL(DefPowerResourceExternal, MainObject->Data, sizeof(DefPowerResourceExternal));
  UT_ASSERT_EQUAL(MainObject->DataSize, sizeof(DefPowerResourceExternal));

  if (!EFI_ERROR (Status)) {
    AmlDebugPrintLinkedObjects (ListHead);
  }

  AmlFreeObjectList (ListHead);

  return UNIT_TEST_PASSED;
}

/***************************************************************/
// External(\_SB.PCI0.TSTC, ProcessorObj)
/***************************************************************/
UNIT_TEST_STATUS
EFIAPI
TestProcessorObjExternal(
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


  // External TSTC Create
  Status = AmlOPExternal("\\_SB.PCI0.TSTC", ProcessorObj, 0, ListHead);
  UT_ASSERT_NOT_EFI_ERROR(Status);

  // Get first Child Node
  Node = GetFirstNode(ListHead);
  MainObject = AML_OBJECT_INSTANCE_FROM_LINK (Node);
  UT_ASSERT_NOT_NULL(MainObject->Data);
  UT_ASSERT_MEM_EQUAL(DefProcessorExternal, MainObject->Data, sizeof(DefProcessorExternal));
  UT_ASSERT_EQUAL(MainObject->DataSize, sizeof(DefProcessorExternal));

  if (!EFI_ERROR (Status)) {
    AmlDebugPrintLinkedObjects (ListHead);
  }

  AmlFreeObjectList (ListHead);

  return UNIT_TEST_PASSED;
}

/***************************************************************/
// External(\_SB.PCI0.TSTD, ThermalZoneObj)
/***************************************************************/
UNIT_TEST_STATUS
EFIAPI
TestThermalZoneExternal(
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


  // External TSTD Create
  Status = AmlOPExternal("\\_SB.PCI0.TSTD", ThermalZoneObj, 0, ListHead);
  UT_ASSERT_NOT_EFI_ERROR(Status);

  // Get first Child Node
  Node = GetFirstNode(ListHead);
  MainObject = AML_OBJECT_INSTANCE_FROM_LINK (Node);
  UT_ASSERT_NOT_NULL(MainObject->Data);
  UT_ASSERT_MEM_EQUAL(DefThermalZoneExternal, MainObject->Data, sizeof(DefThermalZoneExternal));
  UT_ASSERT_EQUAL(MainObject->DataSize, sizeof(DefThermalZoneExternal));

  if (!EFI_ERROR (Status)) {
    AmlDebugPrintLinkedObjects (ListHead);
  }

  AmlFreeObjectList (ListHead);

  return UNIT_TEST_PASSED;
}

/***************************************************************/
// External(\_SB.PCI0.TSTE, BuffFieldObj)
/***************************************************************/
UNIT_TEST_STATUS
EFIAPI
TestBuffFieldExternal(
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


  // External TSTE Crete
  Status = AmlOPExternal("\\_SB.PCI0.TSTE", BuffFieldObj, 0, ListHead);
  UT_ASSERT_NOT_EFI_ERROR(Status);

  // Get first Child Node
  Node = GetFirstNode(ListHead);
  MainObject = AML_OBJECT_INSTANCE_FROM_LINK (Node);
  UT_ASSERT_NOT_NULL(MainObject->Data);
  UT_ASSERT_MEM_EQUAL(DefBufferFieldExternal, MainObject->Data, sizeof(DefBufferFieldExternal));
  UT_ASSERT_EQUAL(MainObject->DataSize, sizeof(DefBufferFieldExternal));

  if (!EFI_ERROR (Status)) {
    AmlDebugPrintLinkedObjects (ListHead);
  }

  AmlFreeObjectList (ListHead);

  return UNIT_TEST_PASSED;
}

/***************************************************************/
// External(\_SB.PCI0.LPCB, DeviceObj)
/***************************************************************/
UNIT_TEST_STATUS
EFIAPI
TestExternalLpc(
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


  // External LPCB Create
  Status = AmlOPExternal("\\_SB.PCI0.LPCB", DeviceObj, 0, ListHead);
  UT_ASSERT_NOT_EFI_ERROR(Status);

  // Get first Child Node
  Node = GetFirstNode(ListHead);
  MainObject = AML_OBJECT_INSTANCE_FROM_LINK (Node);
  UT_ASSERT_NOT_NULL(MainObject->Data);
  UT_ASSERT_MEM_EQUAL(DefExternalLpc, MainObject->Data, sizeof(DefExternalLpc));
  UT_ASSERT_EQUAL(MainObject->DataSize, sizeof(DefExternalLpc));

  if (!EFI_ERROR (Status)) {
    AmlDebugPrintLinkedObjects (ListHead);
  }

  AmlFreeObjectList (ListHead);

  return UNIT_TEST_PASSED;
}

/***************************************************************/
// External(\_SB.PCI0.LPCB.EC0, DeviceObj)
/***************************************************************/
UNIT_TEST_STATUS
EFIAPI
TestExternalEc0(
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


  // External EC0 Create
  Status = AmlOPExternal("\\_SB.PCI0.LPCB.EC0", DeviceObj, 0, ListHead);
  UT_ASSERT_NOT_EFI_ERROR(Status);

  // Get first Child Node
  Node = GetFirstNode(ListHead);
  MainObject = AML_OBJECT_INSTANCE_FROM_LINK (Node);
  UT_ASSERT_NOT_NULL(MainObject->Data);
  UT_ASSERT_MEM_EQUAL(DefExternalEc0, MainObject->Data, sizeof(DefExternalEc0));
  UT_ASSERT_EQUAL(MainObject->DataSize, sizeof(DefExternalEc0));

  if (!EFI_ERROR (Status)) {
    AmlDebugPrintLinkedObjects (ListHead);
  }

  AmlFreeObjectList (ListHead);

  return UNIT_TEST_PASSED;
}

/***************************************************************/
// External(\_SB.PCI0.LPCB.EC0.ECRG, IntObj)
/***************************************************************/
UNIT_TEST_STATUS
EFIAPI
TestExternalTreeInt(
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


  // External ECRG Create
  Status = AmlOPExternal("\\_SB.PCI0.LPCB.EC0.ECRG", IntObj, 0, ListHead);
  UT_ASSERT_NOT_EFI_ERROR(Status);

  // Get first Child Node
  Node = GetFirstNode(ListHead);
  MainObject = AML_OBJECT_INSTANCE_FROM_LINK (Node);
  UT_ASSERT_NOT_NULL(MainObject->Data);
  UT_ASSERT_MEM_EQUAL(DefExternalTreeInt, MainObject->Data, sizeof(DefExternalTreeInt));
  UT_ASSERT_EQUAL(MainObject->DataSize, sizeof(DefExternalTreeInt));

  if (!EFI_ERROR (Status)) {
    AmlDebugPrintLinkedObjects (ListHead);
  }

  AmlFreeObjectList (ListHead);

  return UNIT_TEST_PASSED;
}

/***************************************************************/
// External(\_SB.PCI0.LPCB.EC0.ECMX, MutexObj)
/***************************************************************/
UNIT_TEST_STATUS
EFIAPI
TestExternalTreeMux(
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


  // External ECMX Create
  Status = AmlOPExternal("\\_SB.PCI0.LPCB.EC0.ECMX", MutexObj, 0, ListHead);
  UT_ASSERT_NOT_EFI_ERROR(Status);

  // Get first Child Node
  Node = GetFirstNode(ListHead);
  MainObject = AML_OBJECT_INSTANCE_FROM_LINK (Node);
  UT_ASSERT_NOT_NULL(MainObject->Data);
  UT_ASSERT_MEM_EQUAL(DefExternalTreeMux, MainObject->Data, sizeof(DefExternalTreeMux));
  UT_ASSERT_EQUAL(MainObject->DataSize, sizeof(DefExternalTreeMux));

  if (!EFI_ERROR (Status)) {
    AmlDebugPrintLinkedObjects (ListHead);
  }

  AmlFreeObjectList (ListHead);

  return UNIT_TEST_PASSED;
}

/***************************************************************/
// External(\_SB.PCI0.LPCB.EC0.MTH0, MethodObj)
/***************************************************************/
UNIT_TEST_STATUS
EFIAPI
TestExternalTreeMethod(
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


  // External MTH0 Create
  Status = AmlOPExternal("\\_SB.PCI0.LPCB.EC0.MTH0", MethodObj, 0, ListHead);
  UT_ASSERT_NOT_EFI_ERROR(Status);

  // Get first Child Node
  Node = GetFirstNode(ListHead);
  MainObject = AML_OBJECT_INSTANCE_FROM_LINK (Node);
  UT_ASSERT_NOT_NULL(MainObject->Data);
  UT_ASSERT_MEM_EQUAL(DefExternalTreeMethod, MainObject->Data, sizeof(DefExternalTreeMethod));
  UT_ASSERT_EQUAL(MainObject->DataSize, sizeof(DefExternalTreeMethod));

  if (!EFI_ERROR (Status)) {
    AmlDebugPrintLinkedObjects (ListHead);
  }

  AmlFreeObjectList (ListHead);

  return UNIT_TEST_PASSED;
}

/***************************************************************/
// External(\_SB.PCI0.LPCB.EC0.MTH1, MethodObj, {StrObj})
/***************************************************************/
UNIT_TEST_STATUS
EFIAPI
TestExternalMethod1Arg(
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

  // External MTH1 Create
  Status = AmlOPExternal("\\_SB.PCI0.LPCB.EC0.MTH1", MethodObj, 1, ListHead);
  UT_ASSERT_NOT_EFI_ERROR(Status);

  // Get first Child Node
  Node = GetFirstNode(ListHead);
  MainObject = AML_OBJECT_INSTANCE_FROM_LINK (Node);
  UT_ASSERT_NOT_NULL(MainObject->Data);
  UT_ASSERT_MEM_EQUAL(DefExternalMethod1Arg, MainObject->Data, sizeof(DefExternalMethod1Arg));
  UT_ASSERT_EQUAL(MainObject->DataSize, sizeof(DefExternalMethod1Arg));

  if (!EFI_ERROR (Status)) {
    AmlDebugPrintLinkedObjects (ListHead);
  }

  AmlFreeObjectList (ListHead);

  return UNIT_TEST_PASSED;
}

/***************************************************************/
// External(\_SB.PCI0.LPCB.EC0.MTH2, MethodObj, {StrObj, MethodObj, ProcessorObj})
/***************************************************************/
UNIT_TEST_STATUS
EFIAPI
TestExternalMethod3Arg(
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


  // External MTH2 Create
  Status = AmlOPExternal("\\_SB.PCI0.LPCB.EC0.MTH2", MethodObj, 3, ListHead);
  UT_ASSERT_NOT_EFI_ERROR(Status);

  // Get first Child Node
  Node = GetFirstNode(ListHead);
  MainObject = AML_OBJECT_INSTANCE_FROM_LINK (Node);
  UT_ASSERT_NOT_NULL(MainObject->Data);
  UT_ASSERT_MEM_EQUAL(DefExternalMethod3Arg, MainObject->Data, sizeof(DefExternalMethod3Arg));
  UT_ASSERT_EQUAL(MainObject->DataSize, sizeof(DefExternalMethod3Arg));

  if (!EFI_ERROR (Status)) {
    AmlDebugPrintLinkedObjects (ListHead);
  }

  AmlFreeObjectList (ListHead);

  return UNIT_TEST_PASSED;
}

/***************************************************************/
// External(\_SB.PCI0.LPCB.EC0.MTH3, MethodObj, {StrObj, ThermalZoneObj, IntObj, IntObj, IntObj, IntObj, IntObj})
/***************************************************************/
UNIT_TEST_STATUS
EFIAPI
TestExternalMethod7Arg(
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


  // External MTH3 Create
  Status = AmlOPExternal("\\_SB.PCI0.LPCB.EC0.MTH3", MethodObj, 7, ListHead);
  UT_ASSERT_NOT_EFI_ERROR(Status);

  // Get first Child Node
  Node = GetFirstNode(ListHead);
  MainObject = AML_OBJECT_INSTANCE_FROM_LINK (Node);
  UT_ASSERT_NOT_NULL(MainObject->Data);
  UT_ASSERT_MEM_EQUAL(DefExternalMethod7Arg, MainObject->Data, sizeof(DefExternalMethod7Arg));
  UT_ASSERT_EQUAL(MainObject->DataSize, sizeof(DefExternalMethod7Arg));

  if (!EFI_ERROR (Status)) {
    AmlDebugPrintLinkedObjects (ListHead);
  }

  AmlFreeObjectList (ListHead);

  return UNIT_TEST_PASSED;
}


/***************************************************************/
// Creates and runs the DefExternal Test Suite
/***************************************************************/
EFI_STATUS
EFIAPI
DefExternalTestMain (
  VOID
  )
{
  EFI_STATUS                  Status;
  UNIT_TEST_FRAMEWORK_HANDLE  Framework;
  UNIT_TEST_SUITE_HANDLE      ExternalTestSuite;

  DEBUG((DEBUG_INFO, "%a v%a\n", EXTERNAL_UNIT_TEST_NAME, EXTERNAL_UNIT_TEST_VERSION));

  //
  // Start setting up the test framework for running the tests.
  //
  Status = InitUnitTestFramework (&Framework, EXTERNAL_UNIT_TEST_NAME, gEfiCallerBaseName, EXTERNAL_UNIT_TEST_VERSION);
  if (EFI_ERROR(Status)) {
    DEBUG((DEBUG_ERROR, "Failed in InitUnitTestFramework. Status = %r\n", Status));
    goto Done;
  }

  ///
  // Test the DefExternal function
  //
  Status = CreateUnitTestSuite (&ExternalTestSuite, Framework, "DefExternal Test Suite", "AmdEdk2Common.Lib.AmlLib", NULL, NULL);
  if (EFI_ERROR(Status)) {
    DEBUG((DEBUG_ERROR, "Failed in CreateUnitTestSuite for DefExternal Test Suite\n"));
    Status = EFI_OUT_OF_RESOURCES;
    goto Done;
  }

  AddTestCase(ExternalTestSuite, "External Test Input Validation", "ExternalTestInputValidation",    ExternalTestInputValidation,    NULL, NULL, NULL);
  AddTestCase(ExternalTestSuite, "Test External SB",               "TestExternalSb",            TestExternalSb,             NULL, NULL, NULL);
  AddTestCase(ExternalTestSuite, "Test External PCI0",             "TestExternalPci0",          TestExternalPci0,           NULL, NULL, NULL);
  AddTestCase(ExternalTestSuite, "Test Unknown External",          "TestUnknownExternal",       TestUnknownExternal,        NULL, NULL, NULL);
  AddTestCase(ExternalTestSuite, "Test Integer External",          "TestIntegerExternal",       TestIntegerExternal,        NULL, NULL, NULL);
  AddTestCase(ExternalTestSuite, "Test String External",           "TestStringExternal",        TestStringExternal,         NULL, NULL, NULL);
  AddTestCase(ExternalTestSuite, "Test Buffer External",           "TestBufferExternal",        TestBufferExternal,         NULL, NULL, NULL);
  AddTestCase(ExternalTestSuite, "Test Package External",          "TestPackageExternal",       TestPackageExternal,        NULL, NULL, NULL);
  AddTestCase(ExternalTestSuite, "Test Field Unit External",       "TestFieldUnitExternal",     TestFieldUnitExternal,      NULL, NULL, NULL);
  AddTestCase(ExternalTestSuite, "Test Device External",           "TestDeviceExternal",        TestDeviceExternal,         NULL, NULL, NULL);
  AddTestCase(ExternalTestSuite, "Test Event External",            "TestEventExternal",         TestEventExternal,          NULL, NULL, NULL);
  AddTestCase(ExternalTestSuite, "Test Method External",           "TestMethodExternal",        TestMethodExternal,         NULL, NULL, NULL);
  AddTestCase(ExternalTestSuite, "Test Mutex External",            "TestMutexExternal",         TestMutexExternal,          NULL, NULL, NULL);
  AddTestCase(ExternalTestSuite, "Test Op Region External",        "TestOpRegionExternal",      TestOpRegionExternal,       NULL, NULL, NULL);
  AddTestCase(ExternalTestSuite, "Test PowerResource External",    "TestPowerResourceExternal", TestPowerResourceExternal,  NULL, NULL, NULL);
  AddTestCase(ExternalTestSuite, "Test Processor External",        "TestProcessorObjExternal",  TestProcessorObjExternal,   NULL, NULL, NULL);
  AddTestCase(ExternalTestSuite, "Test Thermal Zone External",     "TestThermalZoneExternal",   TestThermalZoneExternal,    NULL, NULL, NULL);
  AddTestCase(ExternalTestSuite, "Test Buffer Field External",     "TestBuffFieldExternal",     TestBuffFieldExternal,      NULL, NULL, NULL);
  AddTestCase(ExternalTestSuite, "Test External LPC",              "TestExternalLpc",           TestExternalLpc,            NULL, NULL, NULL);
  AddTestCase(ExternalTestSuite, "Test External EC0",              "TestExternalEc0",           TestExternalEc0,            NULL, NULL, NULL);
  AddTestCase(ExternalTestSuite, "Test External Tree Integer",     "TestExternalTreeInt",       TestExternalTreeInt,        NULL, NULL, NULL);
  AddTestCase(ExternalTestSuite, "Test External Tree Mux",         "TestExternalTreeMux",       TestExternalTreeMux,        NULL, NULL, NULL);
  AddTestCase(ExternalTestSuite, "Test External Tree Method",      "TestExternalTreeMethod",    TestExternalTreeMethod,     NULL, NULL, NULL);
  AddTestCase(ExternalTestSuite, "Test External Method One Arg",   "TestExternalMethod1Arg",    TestExternalMethod1Arg,     NULL, NULL, NULL);
  AddTestCase(ExternalTestSuite, "Test External Method Three Arg", "TestExternalMethod3Arg",    TestExternalMethod3Arg,     NULL, NULL, NULL);
  AddTestCase(ExternalTestSuite, "Test External Method Seven Arg", "TestExternalMethod7Arg",    TestExternalMethod7Arg,     NULL, NULL, NULL);

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

