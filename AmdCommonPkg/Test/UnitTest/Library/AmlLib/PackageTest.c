
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


#define PACKAGE_UNIT_TEST_NAME        "DefPackage Test Application"
#define PACKAGE_UNIT_TEST_VERSION     "0.1"

UINT8   AslPkgNoArgStr[] = { 0x08,0x54,0x53,0x54,0x41,0x12,0x05,0x01,0x0D,0x41,0x00 };

UINT8   AslPkgNoArgNum[] = { 0x08,0x54,0x53,0x54,0x42,0x12,0x04,0x01,0x0A,0x2D };

UINT8   AslPkgNoArgRef[] = { 0x08,0x54,0x53,0x54,0x43,0x12,0x0A,0x02,0x54,0x53,0x54,0x41,0x54,0x53,0x54,0x42 };

UINT8   AslPkgNoArgPkg[] = { 0x08,0x54,0x53,0x54,0x44,0x12,0x1E,0x04,0x12,0x04,0x01,0x0A,0x04,0x12,0x0A,0x06,0x0D,0x41,0x00,0x0D,0x42,0x00,0x0A,0x2A,0x12,0x02,0x43,0x12,0x08,0x01,0x0D,0x43,0x41,0x52,0x50,0x00 };

UINT8   AslPkgMultiTypeArgs[] = { 0x08,0x54,0x53,0x54,0x45,0x12,0x16,0x04,0x0D,0x41,0x00,0x0A,0x07,0x54,0x53,0x54,0x42,0x12,0x0A,0x02,0x12,0x04,0x01,0x0A,0x0A,0x12,0x02,0x04 };

UINT8   AslPkgSmallNumNoVar[] = { 0x08,0x54,0x53,0x54,0x46,0x12,0x02,0x01 };

UINT8   AslPkgNumMaxNoArg[] = { 0x08,0x54,0x53,0x54,0x47,0x12,0x02,0xFF };

UINT8   AslVarPkgStr[] = { 0x08,0x54,0x53,0x54,0x49,0x13,0x07,0x0B,0x00,0x01,0x0D,0x41,0x00 };

UINT8   AslVarPkgNoArg[] = { 0x08,0x54,0x53,0x54,0x48,0x13,0x04,0x0B,0x00,0x01 };

UINT8   AslVarPkgRef[] = { 0x08,0x54,0x53,0x54,0x4A,0x13,0x0C,0x0B,0xB4,0x01,0x54,0x53,0x54,0x41,0x54,0x53,0x54,0x42 };

UINT8   AslVarPkgPkg[] = { 0x08,0x54,0x53,0x54,0x4B,0x13,0x20,0x0B,0x09,0x02,0x12,0x04,0x01,0x0A,0x04,0x12,0x0A,0x06,0x0D,0x41,0x00,0x0D,0x42,0x00,0x0A,0x2A,0x12,0x02,0x43,0x12,0x08,0x03,0x0D,0x43,0x41,0x52,0x50,0x00 };

UINT8   AslVarPkgMultiTypeArgs[] = { 0x08,0x54,0x53,0x54,0x4C,0x13,0x18,0x0B,0x00,0x02,0x0D,0x41,0x00,0x0A,0x07,0x54,0x53,0x54,0x42,0x12,0x0A,0x02,0x12,0x04,0x01,0x0A,0x0A,0x12,0x02,0x04 };

/***************************************************************/
/* AML DefPackage Input Validation Tests                    */
/***************************************************************/
UNIT_TEST_STATUS
EFIAPI
PackageTestInputValidation(
  IN  UNIT_TEST_CONTEXT   Context
)
{
  EFI_STATUS           Status;
  LIST_ENTRY           *ListHead;
  LIST_ENTRY           AmlListHead;

  ListHead = &AmlListHead;

  InitializeListHead (ListHead);

  // Invalid Phase Input
  DEBUG((DEBUG_ERROR,"Invalid Phase Input\n"));
  Status = AmlPackage(33, 1, ListHead);
  UT_ASSERT_EQUAL(EFI_INVALID_PARAMETER, Status);

  // NULL List Head
  DEBUG((DEBUG_ERROR,"\n"));
  Status = AmlPackage(AmlStart, 1, NULL);
  UT_ASSERT_EQUAL(EFI_INVALID_PARAMETER, Status);

  // Close Before Start
  //DEBUG((DEBUG_ERROR,"Close Before Start\n"));
  //Status = AmlPackage(AmlClose, 1, ListHead);
  //UT_ASSERT_EQUAL(EFI_NOT_FOUND, Status);

  // More elements in package than indicated by NumArgs
  DEBUG((DEBUG_ERROR,"Too Many Elements in package Start\n"));
  Status = AmlPackage(AmlStart, 1, ListHead);
  UT_ASSERT_EQUAL(EFI_SUCCESS, Status);

    // Data Integer 1
  DEBUG((DEBUG_ERROR,"Data int 1\n"));
    Status = AmlOPDataInteger(1, ListHead);
    UT_ASSERT_NOT_EFI_ERROR(Status);

    // Data Integer 1
  DEBUG((DEBUG_ERROR,"Data int 1\n"));
    Status = AmlOPDataInteger(1, ListHead);
    UT_ASSERT_NOT_EFI_ERROR(Status);

  // Expect invalid parameter in this error case
  DEBUG((DEBUG_ERROR,"Package has invalid parameter\n"));
  Status = AmlPackage(AmlClose, 1, ListHead);
  UT_ASSERT_EQUAL(EFI_INVALID_PARAMETER, Status);

  // Happy Path Start
//  DEBUG((DEBUG_ERROR,"Happy Path Start\n"));
//  Status = AmlPackage(AmlStart, 1, ListHead);
//  UT_ASSERT_EQUAL(EFI_SUCCESS, Status);

  // Happy Path Close
//  DEBUG((DEBUG_ERROR,"Happy Path Close\n"));
//  Status = AmlPackage(AmlClose, 1, ListHead);
//  UT_ASSERT_EQUAL(EFI_SUCCESS, Status);

  if (!EFI_ERROR (Status)) {
    AmlDebugPrintLinkedObjects (ListHead);
  }
  AmlFreeObjectList (ListHead);

  return UNIT_TEST_PASSED;
}

/***************************************************************/
/* AML DefPackage Happy Path Tests                        */
/***************************************************************/

/***************************************************************/
// Name(TSTA, Package() {"A"})
/***************************************************************/
UNIT_TEST_STATUS
EFIAPI
TestPkgNoArgStr(
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

  Status = AmlName(AmlStart, "TSTA", ListHead);
  UT_ASSERT_NOT_EFI_ERROR(Status);

    Status = AmlPackage(AmlStart, 0, ListHead);
    UT_ASSERT_NOT_EFI_ERROR(Status);

      Status = AmlOPDataString("A", ListHead);
      UT_ASSERT_NOT_EFI_ERROR(Status);

    Status = AmlPackage(AmlClose, 0, ListHead);
    UT_ASSERT_NOT_EFI_ERROR(Status);

  Status = AmlName(AmlClose, "TSTA", ListHead);
  UT_ASSERT_NOT_EFI_ERROR(Status);

  // Get first Child Node
  Node = GetFirstNode(ListHead);
  MainObject = AML_OBJECT_INSTANCE_FROM_LINK (Node);
  UT_ASSERT_NOT_NULL(MainObject->Data);
  UT_ASSERT_MEM_EQUAL(AslPkgNoArgStr, MainObject->Data, sizeof(AslPkgNoArgStr));
  UT_ASSERT_EQUAL(MainObject->DataSize, sizeof(AslPkgNoArgStr));

  if (!EFI_ERROR (Status)) {
    AmlDebugPrintLinkedObjects (ListHead);
  }
  AmlFreeObjectList (ListHead);

  return UNIT_TEST_PASSED;
}

/***************************************************************/
// Name(TSTB, Package() {45})
/***************************************************************/
UNIT_TEST_STATUS
EFIAPI
TestPkgNoArgNum(
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

  Status = AmlName(AmlStart, "TSTB", ListHead);
  UT_ASSERT_NOT_EFI_ERROR(Status);
  DEBUG((DEBUG_ERROR,"TSTB\n"));
    Status = AmlPackage(AmlStart, 0, ListHead);
    UT_ASSERT_NOT_EFI_ERROR(Status);
    DEBUG((DEBUG_ERROR,"AmlPackage\n"));

      Status = AmlOPDataInteger(45, ListHead);
      UT_ASSERT_NOT_EFI_ERROR(Status);
      DEBUG((DEBUG_ERROR,"AmlOPDataInteger(45)\n"));

    AmlDebugPrintLinkedObjects (ListHead);
    Status = AmlPackage(AmlClose, 0, ListHead);
    UT_ASSERT_NOT_EFI_ERROR(Status);
    DEBUG((DEBUG_ERROR,"End AmlPackage\n"));

  Status = AmlName(AmlClose, "TSTB", ListHead);
  UT_ASSERT_NOT_EFI_ERROR(Status);
  DEBUG((DEBUG_ERROR,"End TSTB\n"));

  // Get first Child Node
  Node = GetFirstNode(ListHead);
  MainObject = AML_OBJECT_INSTANCE_FROM_LINK (Node);
  UT_ASSERT_NOT_NULL(MainObject->Data);
  UT_ASSERT_MEM_EQUAL(AslPkgNoArgNum, MainObject->Data, sizeof(AslPkgNoArgNum));
  UT_ASSERT_EQUAL(MainObject->DataSize, sizeof(AslPkgNoArgNum));

  if (!EFI_ERROR (Status)) {
    AmlDebugPrintLinkedObjects (ListHead);
  }
  AmlFreeObjectList (ListHead);

  return UNIT_TEST_PASSED;
}

/***************************************************************/
// Name(TSTC, Package() {TSTA, TSTB})
/***************************************************************/
UNIT_TEST_STATUS
EFIAPI
TestPkgNoArgRef(
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

  Status = AmlName(AmlStart, "TSTC", ListHead);
  UT_ASSERT_NOT_EFI_ERROR(Status);

    Status = AmlPackage(AmlStart, 0, ListHead);
    UT_ASSERT_NOT_EFI_ERROR(Status);

      Status = AmlOPNameString("TSTA", ListHead);
      UT_ASSERT_NOT_EFI_ERROR(Status);

      Status = AmlOPNameString("TSTB", ListHead);
      UT_ASSERT_NOT_EFI_ERROR(Status);

    Status = AmlPackage(AmlClose, 0, ListHead);
    UT_ASSERT_NOT_EFI_ERROR(Status);

  Status = AmlName(AmlClose, "TSTC", ListHead);
  UT_ASSERT_NOT_EFI_ERROR(Status);

  // Get first Child Node
  Node = GetFirstNode(ListHead);
  MainObject = AML_OBJECT_INSTANCE_FROM_LINK (Node);
  UT_ASSERT_NOT_NULL(MainObject->Data);
  UT_ASSERT_MEM_EQUAL(AslPkgNoArgRef, MainObject->Data, sizeof(AslPkgNoArgRef));
  UT_ASSERT_EQUAL(MainObject->DataSize, sizeof(AslPkgNoArgRef));

  if (!EFI_ERROR (Status)) {
    AmlDebugPrintLinkedObjects (ListHead);
  }
  AmlFreeObjectList (ListHead);

  return UNIT_TEST_PASSED;
}

/***************************************************************/
// Name(TSTD, Package() {
/***************************************************************/
UNIT_TEST_STATUS
EFIAPI
TestPkgNoArgPkg(
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

  Status = AmlName(AmlStart, "TSTD", ListHead);
  UT_ASSERT_NOT_EFI_ERROR(Status);

    Status = AmlPackage(AmlStart, 0, ListHead);
    UT_ASSERT_NOT_EFI_ERROR(Status);

      // Package() {4}
      Status = AmlPackage(AmlStart, 0, ListHead);
      UT_ASSERT_NOT_EFI_ERROR(Status);

        Status = AmlOPDataInteger(4, ListHead);
        UT_ASSERT_NOT_EFI_ERROR(Status);

      Status = AmlPackage(AmlClose, 0, ListHead);
      UT_ASSERT_NOT_EFI_ERROR(Status);

      // Package(6) {"A", "B", 42}
      Status = AmlPackage(AmlStart, 6, ListHead);
      UT_ASSERT_NOT_EFI_ERROR(Status);

        Status = AmlOPDataString("A", ListHead);
        UT_ASSERT_NOT_EFI_ERROR(Status);

        Status = AmlOPDataString("B", ListHead);
        UT_ASSERT_NOT_EFI_ERROR(Status);

        Status = AmlOPDataInteger(42, ListHead);
        UT_ASSERT_NOT_EFI_ERROR(Status);

      Status = AmlPackage(AmlClose, 6, ListHead);
      UT_ASSERT_NOT_EFI_ERROR(Status);

      // Package(67) {}
      Status = AmlPackage(AmlStart, 67, ListHead);
      UT_ASSERT_NOT_EFI_ERROR(Status);

      Status = AmlPackage(AmlClose,67, ListHead);
      UT_ASSERT_NOT_EFI_ERROR(Status);

      // Package(1) {"CARP"}
      Status = AmlPackage(AmlStart, 1, ListHead);
      UT_ASSERT_NOT_EFI_ERROR(Status);

        Status = AmlOPDataString("CARP", ListHead);
        UT_ASSERT_NOT_EFI_ERROR(Status);

      Status = AmlPackage(AmlClose, 1, ListHead);
      UT_ASSERT_NOT_EFI_ERROR(Status);

    Status = AmlPackage(AmlClose, 0, ListHead);
    UT_ASSERT_NOT_EFI_ERROR(Status);

  Status = AmlName(AmlClose, "TSTD", ListHead);
  UT_ASSERT_NOT_EFI_ERROR(Status);

  // Get first Child Node
  Node = GetFirstNode(ListHead);
  MainObject = AML_OBJECT_INSTANCE_FROM_LINK (Node);
  UT_ASSERT_NOT_NULL(MainObject->Data);
  UT_ASSERT_MEM_EQUAL(AslPkgNoArgPkg, MainObject->Data, sizeof(AslPkgNoArgPkg));
  UT_ASSERT_EQUAL(MainObject->DataSize, sizeof(AslPkgNoArgPkg));

  if (!EFI_ERROR (Status)) {
    AmlDebugPrintLinkedObjects (ListHead);
  }
  AmlFreeObjectList (ListHead);

  return UNIT_TEST_PASSED;
}

/***************************************************************/
// Name(TSTE, Package() {"A", 7, TSTB, Package() {Package() {10}, Package(4) {}}})
/***************************************************************/
UNIT_TEST_STATUS
EFIAPI
TestPkgMultiTypeArgs(
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

  Status = AmlName(AmlStart, "TSTE", ListHead);
  UT_ASSERT_NOT_EFI_ERROR(Status);

    Status = AmlPackage(AmlStart, 0, ListHead);
    UT_ASSERT_NOT_EFI_ERROR(Status);

      Status = AmlOPDataString("A", ListHead);
      UT_ASSERT_NOT_EFI_ERROR(Status);

      Status = AmlOPDataInteger(7, ListHead);
      UT_ASSERT_NOT_EFI_ERROR(Status);

      Status = AmlOPNameString("TSTB", ListHead);
      UT_ASSERT_NOT_EFI_ERROR(Status);

      // Package() {Package() {10}, Package(4) {}}
      Status = AmlPackage(AmlStart, 0, ListHead);
      UT_ASSERT_NOT_EFI_ERROR(Status);

        // Package() {10}
        Status = AmlPackage(AmlStart, 0, ListHead);
        UT_ASSERT_NOT_EFI_ERROR(Status);

          Status = AmlOPDataInteger(10,ListHead);
          UT_ASSERT_NOT_EFI_ERROR(Status);

        Status = AmlPackage(AmlClose, 0, ListHead);
        UT_ASSERT_NOT_EFI_ERROR(Status);

        // Package(4) {}
        Status = AmlPackage(AmlStart, 4, ListHead);
        UT_ASSERT_NOT_EFI_ERROR(Status);

        Status = AmlPackage(AmlClose, 4, ListHead);
        UT_ASSERT_NOT_EFI_ERROR(Status);

      Status = AmlPackage(AmlClose, 0, ListHead);
      UT_ASSERT_NOT_EFI_ERROR(Status);


    Status = AmlPackage(AmlClose, 0, ListHead);
    UT_ASSERT_NOT_EFI_ERROR(Status);

  Status = AmlName(AmlClose, "TSTE", ListHead);
  UT_ASSERT_NOT_EFI_ERROR(Status);

  // Get first Child Node
  Node = GetFirstNode(ListHead);
  MainObject = AML_OBJECT_INSTANCE_FROM_LINK (Node);
  UT_ASSERT_NOT_NULL(MainObject->Data);
  UT_ASSERT_MEM_EQUAL(AslPkgMultiTypeArgs, MainObject->Data, sizeof(AslPkgMultiTypeArgs));
  UT_ASSERT_EQUAL(MainObject->DataSize, sizeof(AslPkgMultiTypeArgs));

  if (!EFI_ERROR (Status)) {
    AmlDebugPrintLinkedObjects (ListHead);
  }
  AmlFreeObjectList (ListHead);

  return UNIT_TEST_PASSED;
}

/***************************************************************/
// Name(TSTF, Package(1) {}
/***************************************************************/
UNIT_TEST_STATUS
EFIAPI
TestPkgSmallNumNoVar(
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

  Status = AmlName(AmlStart, "TSTF", ListHead);
  UT_ASSERT_NOT_EFI_ERROR(Status);

    Status = AmlPackage(AmlStart, 1, ListHead);
    UT_ASSERT_NOT_EFI_ERROR(Status);

    Status = AmlPackage(AmlClose, 1, ListHead);
    UT_ASSERT_NOT_EFI_ERROR(Status);

  Status = AmlName(AmlClose, "TSTF", ListHead);
  UT_ASSERT_NOT_EFI_ERROR(Status);

  // Get first Child Node
  Node = GetFirstNode(ListHead);
  MainObject = AML_OBJECT_INSTANCE_FROM_LINK (Node);
  UT_ASSERT_NOT_NULL(MainObject->Data);
  UT_ASSERT_MEM_EQUAL(AslPkgSmallNumNoVar, MainObject->Data, sizeof(AslPkgSmallNumNoVar));
  UT_ASSERT_EQUAL(MainObject->DataSize, sizeof(AslPkgSmallNumNoVar));

  if (!EFI_ERROR (Status)) {
    AmlDebugPrintLinkedObjects (ListHead);
  }
  AmlFreeObjectList (ListHead);

  return UNIT_TEST_PASSED;
}

/***************************************************************/
// Name(TSTG, Package(255) { })
/***************************************************************/
UNIT_TEST_STATUS
EFIAPI
TestPkgNumMaxNoArg(
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

  Status = AmlName(AmlStart, "TSTG", ListHead);
  UT_ASSERT_NOT_EFI_ERROR(Status);

    Status = AmlPackage(AmlStart, 255, ListHead);
    UT_ASSERT_NOT_EFI_ERROR(Status);

    Status = AmlPackage(AmlClose, 255, ListHead);
    UT_ASSERT_NOT_EFI_ERROR(Status);

  Status = AmlName(AmlClose, "TSTG", ListHead);
  UT_ASSERT_NOT_EFI_ERROR(Status);

  // Get first Child Node
  Node = GetFirstNode(ListHead);
  MainObject = AML_OBJECT_INSTANCE_FROM_LINK (Node);
  UT_ASSERT_NOT_NULL(MainObject->Data);
  UT_ASSERT_MEM_EQUAL(AslPkgNumMaxNoArg, MainObject->Data, sizeof(AslPkgNumMaxNoArg));
  UT_ASSERT_EQUAL(MainObject->DataSize, sizeof(AslPkgNumMaxNoArg));

  if (!EFI_ERROR (Status)) {
    AmlDebugPrintLinkedObjects (ListHead);
  }
  AmlFreeObjectList (ListHead);

  return UNIT_TEST_PASSED;
}

/***************************************************************/
// Name(TSTH, Package(256) { })
/***************************************************************/
UNIT_TEST_STATUS
EFIAPI
TestVarPkgNoArg(
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

  Status = AmlName(AmlStart, "TSTH", ListHead);
  UT_ASSERT_NOT_EFI_ERROR(Status);

    Status = AmlPackage(AmlStart, 256, ListHead);
    UT_ASSERT_NOT_EFI_ERROR(Status);

    Status = AmlPackage(AmlClose, 256, ListHead);
    UT_ASSERT_NOT_EFI_ERROR(Status);

  Status = AmlName(AmlClose, "TSTH", ListHead);
  UT_ASSERT_NOT_EFI_ERROR(Status);

  // Get first Child Node
  Node = GetFirstNode(ListHead);
  MainObject = AML_OBJECT_INSTANCE_FROM_LINK (Node);
  UT_ASSERT_NOT_NULL(MainObject->Data);
  UT_ASSERT_MEM_EQUAL(AslVarPkgNoArg, MainObject->Data, sizeof(AslVarPkgNoArg));
  UT_ASSERT_EQUAL(MainObject->DataSize, sizeof(AslVarPkgNoArg));

  if (!EFI_ERROR (Status)) {
    AmlDebugPrintLinkedObjects (ListHead);
  }
  AmlFreeObjectList (ListHead);

  return UNIT_TEST_PASSED;
}

/***************************************************************/
// Name(TSTI, Package(256) {"A"})
/***************************************************************/
UNIT_TEST_STATUS
EFIAPI
TestVarPkgStr(
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

  Status = AmlName(AmlStart, "TSTI", ListHead);
  UT_ASSERT_NOT_EFI_ERROR(Status);

    Status = AmlPackage(AmlStart, 256, ListHead);
    UT_ASSERT_NOT_EFI_ERROR(Status);

      Status = AmlOPDataString("A", ListHead);
      UT_ASSERT_NOT_EFI_ERROR(Status);

    Status = AmlPackage(AmlClose, 256, ListHead);
    UT_ASSERT_NOT_EFI_ERROR(Status);

  Status = AmlName(AmlClose, "TSTI", ListHead);
  UT_ASSERT_NOT_EFI_ERROR(Status);

  // Get first Child Node
  Node = GetFirstNode(ListHead);
  MainObject = AML_OBJECT_INSTANCE_FROM_LINK (Node);
  UT_ASSERT_NOT_NULL(MainObject->Data);
  UT_ASSERT_MEM_EQUAL(AslVarPkgStr, MainObject->Data, sizeof(AslVarPkgStr));
  UT_ASSERT_EQUAL(MainObject->DataSize, sizeof(AslVarPkgStr));

  if (!EFI_ERROR (Status)) {
    AmlDebugPrintLinkedObjects (ListHead);
  }
  AmlFreeObjectList (ListHead);

  return UNIT_TEST_PASSED;
}

/***************************************************************/
// Name(TSTJ, Package(436) {TSTA, TSTB})
/***************************************************************/
UNIT_TEST_STATUS
EFIAPI
TestVarPkgRef(
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

  Status = AmlName(AmlStart, "TSTJ", ListHead);
  UT_ASSERT_NOT_EFI_ERROR(Status);

    Status = AmlPackage(AmlStart, 436, ListHead);
    UT_ASSERT_NOT_EFI_ERROR(Status);

      Status = AmlOPNameString("TSTA", ListHead);
      UT_ASSERT_NOT_EFI_ERROR(Status);

      Status = AmlOPNameString("TSTB", ListHead);
      UT_ASSERT_NOT_EFI_ERROR(Status);

    Status = AmlPackage(AmlClose, 436, ListHead);
    UT_ASSERT_NOT_EFI_ERROR(Status);

  Status = AmlName(AmlClose, "TSTJ", ListHead);
  UT_ASSERT_NOT_EFI_ERROR(Status);


  // Get first Child Node
  Node = GetFirstNode(ListHead);
  MainObject = AML_OBJECT_INSTANCE_FROM_LINK (Node);
  UT_ASSERT_NOT_NULL(MainObject->Data);
  UT_ASSERT_MEM_EQUAL(AslVarPkgRef, MainObject->Data, sizeof(AslVarPkgRef));
  UT_ASSERT_EQUAL(MainObject->DataSize, sizeof(AslVarPkgRef));

  if (!EFI_ERROR (Status)) {
    AmlDebugPrintLinkedObjects (ListHead);
  }
  AmlFreeObjectList (ListHead);

  return UNIT_TEST_PASSED;
}

/***************************************************************/
// Name(TSTK, Package(521) {
/***************************************************************/
UNIT_TEST_STATUS
EFIAPI
TestVarPkgPkg(
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

  Status = AmlName(AmlStart, "TSTK", ListHead);
  UT_ASSERT_NOT_EFI_ERROR(Status);

    Status = AmlPackage(AmlStart, 521, ListHead);
    UT_ASSERT_NOT_EFI_ERROR(Status);

      // Package() {4}
      Status = AmlPackage(AmlStart, 0, ListHead);
      UT_ASSERT_NOT_EFI_ERROR(Status);

        Status = AmlOPDataInteger(4, ListHead);
        UT_ASSERT_NOT_EFI_ERROR(Status);

      Status = AmlPackage(AmlClose, 0, ListHead);
      UT_ASSERT_NOT_EFI_ERROR(Status);

      // Package(6) {"A", "B", 42}
      Status = AmlPackage(AmlStart, 6, ListHead);
      UT_ASSERT_NOT_EFI_ERROR(Status);

        Status = AmlOPDataString("A", ListHead);
        UT_ASSERT_NOT_EFI_ERROR(Status);

        Status = AmlOPDataString("B", ListHead);
        UT_ASSERT_NOT_EFI_ERROR(Status);

        Status = AmlOPDataInteger(42, ListHead);
        UT_ASSERT_NOT_EFI_ERROR(Status);

      Status = AmlPackage(AmlClose, 6, ListHead);
      UT_ASSERT_NOT_EFI_ERROR(Status);

      // Package(67) {}
      Status = AmlPackage(AmlStart, 67, ListHead);
      UT_ASSERT_NOT_EFI_ERROR(Status);

      Status = AmlPackage(AmlClose,67, ListHead);
      UT_ASSERT_NOT_EFI_ERROR(Status);

      // Package(3) {"CARP"}
      Status = AmlPackage(AmlStart, 3, ListHead);
      UT_ASSERT_NOT_EFI_ERROR(Status);

        Status = AmlOPDataString("CARP", ListHead);
        UT_ASSERT_NOT_EFI_ERROR(Status);

      Status = AmlPackage(AmlClose, 3, ListHead);
      UT_ASSERT_NOT_EFI_ERROR(Status);

    Status = AmlPackage(AmlClose, 521, ListHead);
    UT_ASSERT_NOT_EFI_ERROR(Status);

  Status = AmlName(AmlClose, "TSTK", ListHead);
  UT_ASSERT_NOT_EFI_ERROR(Status);
  // Get first Child Node
  Node = GetFirstNode(ListHead);
  MainObject = AML_OBJECT_INSTANCE_FROM_LINK (Node);
  UT_ASSERT_NOT_NULL(MainObject->Data);
  UT_ASSERT_MEM_EQUAL(AslVarPkgPkg, MainObject->Data, sizeof(AslVarPkgPkg));
  UT_ASSERT_EQUAL(MainObject->DataSize, sizeof(AslVarPkgPkg));

  if (!EFI_ERROR (Status)) {
    AmlDebugPrintLinkedObjects (ListHead);
  }
  AmlFreeObjectList (ListHead);

  return UNIT_TEST_PASSED;
}

/***************************************************************/
// Name(TSTL, Package(512) {"A", 7, TSTB, Package() {Package() {10}, Package(4) {}}})
/***************************************************************/
UNIT_TEST_STATUS
EFIAPI
TestVarPkgMultiTypeArgs(
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

  Status = AmlName(AmlStart, "TSTL", ListHead);
  UT_ASSERT_NOT_EFI_ERROR(Status);

    Status = AmlPackage(AmlStart, 512, ListHead);
    UT_ASSERT_NOT_EFI_ERROR(Status);

      Status = AmlOPDataString("A", ListHead);
      UT_ASSERT_NOT_EFI_ERROR(Status);

      Status = AmlOPDataInteger(7, ListHead);
      UT_ASSERT_NOT_EFI_ERROR(Status);

      Status = AmlOPNameString("TSTB", ListHead);
      UT_ASSERT_NOT_EFI_ERROR(Status);

      // Package() {Package() {10}, Package(4) {}}
      Status = AmlPackage(AmlStart, 0, ListHead);
      UT_ASSERT_NOT_EFI_ERROR(Status);

        // Package() {10}
        Status = AmlPackage(AmlStart, 0, ListHead);
        UT_ASSERT_NOT_EFI_ERROR(Status);

          Status = AmlOPDataInteger(10,ListHead);
          UT_ASSERT_NOT_EFI_ERROR(Status);

        Status = AmlPackage(AmlClose, 0, ListHead);
        UT_ASSERT_NOT_EFI_ERROR(Status);

        // Package(4) {}
        Status = AmlPackage(AmlStart, 4, ListHead);
        UT_ASSERT_NOT_EFI_ERROR(Status);

        Status = AmlPackage(AmlClose, 4, ListHead);
        UT_ASSERT_NOT_EFI_ERROR(Status);

      Status = AmlPackage(AmlClose, 0, ListHead);
      UT_ASSERT_NOT_EFI_ERROR(Status);


    Status = AmlPackage(AmlClose, 512, ListHead);
    UT_ASSERT_NOT_EFI_ERROR(Status);

  Status = AmlName(AmlClose, "TSTL", ListHead);
  UT_ASSERT_NOT_EFI_ERROR(Status);

  // Get first Child Node
  Node = GetFirstNode(ListHead);
  MainObject = AML_OBJECT_INSTANCE_FROM_LINK (Node);
  UT_ASSERT_NOT_NULL(MainObject->Data);
  UT_ASSERT_MEM_EQUAL(AslVarPkgMultiTypeArgs, MainObject->Data, sizeof(AslVarPkgMultiTypeArgs));
  UT_ASSERT_EQUAL(MainObject->DataSize, sizeof(AslVarPkgMultiTypeArgs));

  if (!EFI_ERROR (Status)) {
    AmlDebugPrintLinkedObjects (ListHead);
  }
  AmlFreeObjectList (ListHead);

  return UNIT_TEST_PASSED;
}

/***************************************************************/
// Creates and runs the Package Test Suite
/***************************************************************/
EFI_STATUS
EFIAPI
PackageTestMain (
  VOID
  )
{
  EFI_STATUS                  Status;
  UNIT_TEST_FRAMEWORK_HANDLE  Framework;
  UNIT_TEST_SUITE_HANDLE      PackageTestSuite;

  DEBUG((DEBUG_INFO, "%a v%a\n", PACKAGE_UNIT_TEST_NAME, PACKAGE_UNIT_TEST_VERSION));

  //
  // Start setting up the test framework for running the tests.
  //
  Status = InitUnitTestFramework (&Framework, PACKAGE_UNIT_TEST_NAME, gEfiCallerBaseName, PACKAGE_UNIT_TEST_VERSION);
  if (EFI_ERROR(Status)) {
    DEBUG((DEBUG_ERROR, "Failed in InitUnitTestFramework. Status = %r\n", Status));
    goto Done;
  }

  ///
  // Test the Package function
  //
  Status = CreateUnitTestSuite (&PackageTestSuite, Framework, "Package Test Suite", "AmdEdk2Common.Lib.AmlLib", NULL, NULL);
  if (EFI_ERROR(Status)) {
    DEBUG((DEBUG_ERROR, "Failed in CreateUnitTestSuite for Package Test Suite\n"));
    Status = EFI_OUT_OF_RESOURCES;
    goto Done;
  }
  AddTestCase(PackageTestSuite, "Package Test Input Validation",  "PackageTestInputValidation",  PackageTestInputValidation, NULL, NULL, NULL);

  AddTestCase(PackageTestSuite, "Name(TSTA, Package() {\"A\"})",  "TestPkgNoArgStr",  TestPkgNoArgStr, NULL, NULL, NULL);
  AddTestCase(PackageTestSuite, "Name(TSTB, Package() {45})",  "TestPkgNoArgNum",  TestPkgNoArgNum, NULL, NULL, NULL);
  AddTestCase(PackageTestSuite, "Name(TSTC, Package() {TSTA, TSTB})",  "TestPkgNoArgRef",  TestPkgNoArgRef, NULL, NULL, NULL);
  AddTestCase(PackageTestSuite, "Name(TSTD, Package() {",  "TestPkgNoArgPkg",  TestPkgNoArgPkg, NULL, NULL, NULL);
  AddTestCase(PackageTestSuite, "Name(TSTE, Package() {\"A\", 7, TSTB, Package() {Package() {10}, Package(4) {}}})",  "TestPkgMultiTypeArgs",  TestPkgMultiTypeArgs, NULL, NULL, NULL);
  AddTestCase(PackageTestSuite, "Name(TSTF, Package(1) {}",  "TestPkgSmallNumNoVar",  TestPkgSmallNumNoVar, NULL, NULL, NULL);
  AddTestCase(PackageTestSuite, "Name(TSTG, Package(255) { })",  "TestPkgNumMaxNoArg",  TestPkgNumMaxNoArg, NULL, NULL, NULL);
  AddTestCase(PackageTestSuite, "Name(TSTH, Package(256) { })",  "TestVarPkgNoArg",  TestVarPkgNoArg, NULL, NULL, NULL);
  AddTestCase(PackageTestSuite, "Name(TSTI, Package(256) {\"A\"})",  "TestVarPkgStr",  TestVarPkgStr, NULL, NULL, NULL);
  AddTestCase(PackageTestSuite, "Name(TSTJ, Package(436) {TSTA, TSTB})",  "TestVarPkgRef",  TestVarPkgRef, NULL, NULL, NULL);
  AddTestCase(PackageTestSuite, "Name(TSTK, Package(521) {",  "TestVarPkgPkg",  TestVarPkgPkg, NULL, NULL, NULL);
  AddTestCase(PackageTestSuite, "Name(TSTL, Package(512) {\"A\", 7, TSTB, Package() {Package() {10}, Package(4) {}}})",  "TestVarPkgMultiTypeArgs",  TestVarPkgMultiTypeArgs, NULL, NULL, NULL);

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
