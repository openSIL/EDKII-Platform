
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


#define DWORDMEMORY_UNIT_TEST_NAME        "DefDWordMemory Test Application"
#define DWORDMEMORY_UNIT_TEST_VERSION     "0.1"

UINT8   AslDWM_PosAllFixedCacheRW[] = { 0x08,0x54,0x53,0x54,0x41,0x11,0x1F,0x0A,0x1C,0x87,0x17,0x00,0x00,0x0C,0x03,0x00,0x00,0x00,0x00,0x00,0x00,0x0C,0x00,0xFF,0xFF,0x0D,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x02,0x00,0x79,0x00 };

UINT8   AslDWM_SubAllFixedCacheRW[] = { 0x08,0x54,0x53,0x54,0x42,0x11,0x1F,0x0A,0x1C,0x87,0x17,0x00,0x00,0x0E,0x03,0x00,0x00,0x00,0x00,0x00,0x00,0x0C,0x00,0xFF,0xFF,0x0D,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x02,0x00,0x79,0x00 };

UINT8   AslDWM_PosMaxFixedCacheRW[] = { 0x08,0x54,0x53,0x54,0x43,0x11,0x1F,0x0A,0x1C,0x87,0x17,0x00,0x00,0x08,0x03,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xFF,0xFF,0x0D,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x79,0x00 };

UINT8   AslDWM_PosMinFixedCacheRW[] = { 0x08,0x54,0x53,0x54,0x44,0x11,0x1F,0x0A,0x1C,0x87,0x17,0x00,0x00,0x04,0x03,0x00,0x00,0x00,0x00,0x00,0x00,0x0C,0x00,0xFF,0xFF,0x0D,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x79,0x00 };

UINT8   AslDWM_PosAllFixedWrCombRW[] = { 0x08,0x54,0x53,0x54,0x45,0x11,0x1F,0x0A,0x1C,0x87,0x17,0x00,0x00,0x0C,0x05,0x00,0x00,0x00,0x00,0x00,0x00,0x0C,0x00,0xFF,0xFF,0x0D,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x02,0x00,0x79,0x00 };

UINT8   AslDWM_PosAllFixedPrefetchRW[] = { 0x08,0x54,0x53,0x54,0x46,0x11,0x1F,0x0A,0x1C,0x87,0x17,0x00,0x00,0x0C,0x07,0x00,0x00,0x00,0x00,0x00,0x00,0x0C,0x00,0xFF,0xFF,0x0D,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x02,0x00,0x79,0x00 };

UINT8   AslDWM_PosAllFixedNonCacheRW[] = { 0x08,0x54,0x53,0x54,0x47,0x11,0x1F,0x0A,0x1C,0x87,0x17,0x00,0x00,0x0C,0x01,0x00,0x00,0x00,0x00,0x00,0x00,0x0C,0x00,0xFF,0xFF,0x0D,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x02,0x00,0x79,0x00 };

UINT8   AslDWM_PosAllFixedCacheRO[] = { 0x08,0x54,0x53,0x54,0x48,0x11,0x1F,0x0A,0x1C,0x87,0x17,0x00,0x00,0x0C,0x02,0x00,0x00,0x00,0x00,0x00,0x00,0x0C,0x00,0xFF,0xFF,0x0D,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x02,0x00,0x79,0x00 };

UINT8   AslDWM_LargeMinMaxFixed[] = { 0x08,0x54,0x53,0x54,0x49,0x11,0x1F,0x0A,0x1C,0x87,0x17,0x00,0x00,0x0C,0x03,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x02,0xFF,0xFF,0xDF,0xFF,0x00,0x00,0x00,0x00,0x00,0x00,0xE0,0xFD,0x79,0x00 };

UINT8   AslDWM_FixedMaxGranularity[] = { 0x08,0x54,0x53,0x54,0x4A,0x11,0x1F,0x0A,0x1C,0x87,0x17,0x00,0x00,0x08,0x03,0xFF,0xFF,0xFF,0x7F,0x00,0x00,0x00,0x00,0xFF,0xFF,0xFF,0xFF,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x79,0x00 };

UINT8   AslDWM_FixedMinGranularity[] = { 0x08,0x54,0x53,0x54,0x4B,0x11,0x1F,0x0A,0x1C,0x87,0x17,0x00,0x00,0x04,0x03,0xFF,0xFF,0xFF,0x7F,0x00,0x00,0x00,0x80,0xFF,0xFF,0xFF,0xFF,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x79,0x00 };

UINT8   AslDWM_FixedMinMaxTrans[] = { 0x08,0x54,0x53,0x54,0x4C,0x11,0x1F,0x0A,0x1C,0x87,0x17,0x00,0x00,0x0C,0x03,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x02,0xFF,0xFF,0xDF,0xFF,0x00,0x00,0x01,0xFD,0x00,0x00,0xE0,0xFD,0x79,0x00 };

UINT8   AslDWM_FixedMinGranTrans[] = { 0x08,0x54,0x53,0x54,0x4D,0x11,0x1F,0x0A,0x1C,0x87,0x17,0x00,0x00,0x04,0x03,0xFF,0xFF,0xFF,0x7F,0x00,0x00,0x00,0x80,0xFF,0xFF,0xFF,0xFF,0x00,0x00,0x01,0x00,0x00,0x00,0x00,0x00,0x79,0x00 };

/***************************************************************/
/* AML DefDWordMemory Input Validation Tests                    */
/***************************************************************/
UNIT_TEST_STATUS
EFIAPI
DWordMemoryTestInputValidation(
  IN  UNIT_TEST_CONTEXT   Context
)
{
  EFI_STATUS           Status;
  LIST_ENTRY           *ListHead;
  LIST_ENTRY           AmlListHead;

  ListHead = &AmlListHead;

  InitializeListHead (ListHead);

  // ResourceUsage Param Invalid
  Status = AmlOPDWordMemory(0xAA,
                            PosDecode,
                            MinFixed,
                            MaxFixed,
                            Cacheable,
                            ReadWrite,
                            0x00000000,
                            0x08000000,
                            0xFFFFFFFF,
                            0x00000000,
                            0xF8000000,
                            ListHead);
  if (!EFI_ERROR (Status)) {
    DEBUG((DEBUG_INFO, "ResourceUsage Param Invalid\n"));
  }
  UT_ASSERT_EQUAL(EFI_INVALID_PARAMETER, Status);

  // Decode Param Invalid
  Status = AmlOPDWordMemory(ResourceProducer,
                            0x77,
                            MinFixed,
                            MaxFixed,
                            Cacheable,
                            ReadWrite,
                            0x00000000,
                            0x000C0000,
                            0x000DFFFF,
                            0x00000000,
                            0x00020000,
                            ListHead);
  if (!EFI_ERROR (Status)) {
    DEBUG((DEBUG_INFO, "Decode Param Invalid\n"));
  }
  UT_ASSERT_EQUAL(EFI_INVALID_PARAMETER, Status);

  // MinFixed Param Invalid
  Status = AmlOPDWordMemory(ResourceProducer,
                            PosDecode,
                            0xFF,
                            MaxFixed,
                            Cacheable,
                            ReadWrite,
                            0x00000000,
                            0x08000000,
                            0xFFFFFFFF,
                            0x00000000,
                            0xF8000000,
                            ListHead);
  if (!EFI_ERROR (Status)) {
    DEBUG((DEBUG_INFO, "MinFixed Param Invalid\n"));
  }
  UT_ASSERT_EQUAL(EFI_INVALID_PARAMETER, Status);

  // MaxFixed Param Invalid
  Status = AmlOPDWordMemory(ResourceProducer,
                            PosDecode,
                            MinFixed,
                            0xFF,
                            Cacheable,
                            ReadWrite,
                            0x00000000,
                            0x000C0000,
                            0x000DFFFF,
                            0x00000000,
                            0x00020000,
                            ListHead);
  if (!EFI_ERROR (Status)) {
    DEBUG((DEBUG_INFO, "MaxFixed Param Invalid\n"));
  }
  UT_ASSERT_EQUAL(EFI_INVALID_PARAMETER, Status);

  // Cacheable Param Invalid
  Status = AmlOPDWordMemory(ResourceProducer,
                            PosDecode,
                            MinFixed,
                            MaxFixed,
                            0xF1,
                            ReadWrite,
                            0x00000000,
                            0x08000000,
                            0xFFFFFFFF,
                            0x00000000,
                            0xF8000000,
                            ListHead);
  if (!EFI_ERROR (Status)) {
    DEBUG((DEBUG_INFO, "Cacheable Param Invalid\n"));
  }
  UT_ASSERT_EQUAL(EFI_INVALID_PARAMETER, Status);

  // ReadWrite Param Invalid
  Status = AmlOPDWordMemory(ResourceProducer,
                            PosDecode,
                            MinFixed,
                            MaxFixed,
                            Cacheable,
                            0xF1,
                            0x00000000,
                            0x000C0000,
                            0x000DFFFF,
                            0x00000000,
                            0x00020000,
                            ListHead);
  if (!EFI_ERROR (Status)) {
    DEBUG((DEBUG_INFO, "ReadWrite Param Invalid\n"));
  }
  UT_ASSERT_EQUAL(EFI_INVALID_PARAMETER, Status);

  // NULL List Head
  Status = AmlOPDWordMemory(ResourceProducer,
                            PosDecode,
                            MinFixed,
                            MaxFixed,
                            Cacheable,
                            ReadWrite,
                            0x00000000,
                            0x08000000,
                            0xFFFFFFFF,
                            0x00000000,
                            0xF8000000,
                            NULL);
  if (!EFI_ERROR (Status)) {
    DEBUG((DEBUG_INFO, "NULL List Head\n"));
  }
  UT_ASSERT_EQUAL(EFI_INVALID_PARAMETER, Status);

  // Length = 0 Max and Min Fixed            // 0 1 1
  Status = AmlOPDWordMemory(ResourceProducer,
                            PosDecode,
                            MinFixed,
                            MaxFixed,
                            Cacheable,
                            ReadWrite,
                            0x00000000,
                            0x08000000,
                            0xFFFFFFFF,
                            0x00000000,
                            0x00000000,
                            ListHead);
  if (!EFI_ERROR (Status)) {
    DEBUG((DEBUG_INFO, "Length = 0 Max and Min Fixed            // 0 1 1\n"));
  }
  UT_ASSERT_EQUAL(EFI_INVALID_PARAMETER, Status);

  // Length = 0 Max and Min Not Fixed            // 0 0 0
  Status = AmlOPDWordMemory(ResourceProducer,
                            PosDecode,
                            MinNotFixed,
                            MaxNotFixed,
                            Cacheable,
                            ReadWrite,
                            0x00000000,
                            0x00000000,
                            0x00000000,
                            0x00000000,
                            0x00000000,
                            ListHead);
  if (!EFI_ERROR (Status)) {
    DEBUG((DEBUG_INFO, "Length = 0 Max and Min Not Fixed         // 0 0 0\n"));
  }
  UT_ASSERT_EQUAL(EFI_INVALID_PARAMETER, Status);

  // Max address and granularity mismatch     // 0 0 1
  Status = AmlOPDWordMemory(ResourceProducer,
                            PosDecode,
                            MinNotFixed,
                            MaxFixed,
                            Cacheable,
                            ReadWrite,
                            0x00004000,
                            0x00000000,
                            0xFFFFFFFF,
                            0x00000000,
                            0x00000000,
                            ListHead);
  if (!EFI_ERROR (Status)) {
    DEBUG((DEBUG_INFO, "Max address and granularity mismatch     // 0 0 1\n"));
  }
  UT_ASSERT_EQUAL(EFI_INVALID_PARAMETER, Status);

  // Max address and granularity mismatch Max Addr 0     // 0 0 1
  // Should see error here about granularity and max mismatch
  Status = AmlOPDWordMemory(ResourceProducer,
                            PosDecode,
                            MinNotFixed,
                            MaxFixed,
                            Cacheable,
                            ReadWrite,
                            0x07FFFFFF,
                            0x00000000,
                            0x00000000,
                            0x00000000,
                            0x00000000,
                            ListHead);
  if (!EFI_ERROR (Status)) {
    DEBUG((DEBUG_INFO, " Max address and granularity mismatch Max Addr 0     // 0 0 1\n"));
  }
  UT_ASSERT_EQUAL(EFI_INVALID_PARAMETER, Status);

  // Min address and granularity mismatch     // 0 1 0
  Status = AmlOPDWordMemory(ResourceProducer,
                            PosDecode,
                            MinFixed,
                            MaxNotFixed,
                            Cacheable,
                            ReadWrite,
                            0x1FFFFFFF,
                            0x08000000,
                            0x10000000,
                            0x00000000,
                            0x00000000,
                            ListHead);
  if (!EFI_ERROR (Status)) {
    DEBUG((DEBUG_INFO, "Min address and granularity mismatch     // 0 1 0\n"));
  }
  UT_ASSERT_EQUAL(EFI_INVALID_PARAMETER, Status);

  // Range Greater than window
  Status = AmlOPDWordMemory(ResourceProducer,
                            PosDecode,
                            MinNotFixed,
                            MaxNotFixed,
                            Cacheable,
                            ReadWrite,
                            0x00000000,
                            0x00000000,
                            0x00000000,
                            0x00000000,
                            0x00020000,
                            ListHead);
  if (!EFI_ERROR (Status)) {
    DEBUG((DEBUG_INFO, "Length Greater than Window\n"));
  }
  UT_ASSERT_EQUAL(EFI_INVALID_PARAMETER, Status);

  // Min addr more than max addr
  Status = AmlOPDWordMemory(ResourceProducer,
                            PosDecode,
                            MinFixed,
                            MaxNotFixed,
                            Cacheable,
                            ReadWrite,
                            0x00000000,
                            0x08000000,
                            0x00000000,
                            0x00000000,
                            0x00000000,
                            ListHead);
  if (!EFI_ERROR (Status)) {
    DEBUG((DEBUG_INFO, "Min addr more than max addr\n"));
  }
  UT_ASSERT_EQUAL(EFI_INVALID_PARAMETER, Status);

  // MaxFixed, NoMaxAddr Set
  Status = AmlOPDWordMemory(ResourceProducer,
                            PosDecode,
                            MinNotFixed,
                            MaxFixed,
                            Cacheable,
                            ReadWrite,
                            0x00000000,
                            0x00000000,
                            0x00000000,
                            0x00000000,
                            0x00000000,
                            ListHead);
  if (!EFI_ERROR (Status)) {
    DEBUG((DEBUG_INFO, "MaxFixed, NoMaxAddr Set\n"));
  }
  UT_ASSERT_EQUAL(EFI_INVALID_PARAMETER, Status);

  // Max/MinNotFixed, Region Length Granularity Mismatch // >0 0 0
  Status = AmlOPDWordMemory(ResourceProducer,
                            PosDecode,
                            MinNotFixed,
                            MaxNotFixed,
                            Cacheable,
                            ReadWrite,
                            0x0003FFFF,
                            0x00000000,
                            0x00000000,
                            0x00003000,
                            0x00020000,
                            ListHead);
  if (!EFI_ERROR (Status)) {
    DEBUG((DEBUG_INFO, "Max/MinNotFixed, Region Length Granularity Mismatch // >0 0 0\n"));
  }
  UT_ASSERT_EQUAL(EFI_INVALID_PARAMETER, Status);

  // Max/MinFixed, Region Length Set Address Gran not 0  // >0 1 1
  Status = AmlOPDWordMemory(ResourceProducer,
                            PosDecode,
                            MinFixed,
                            MaxFixed,
                            Cacheable,
                            ReadWrite,
                            0x0003FFFF,
                            0x000C0000,
                            0x000DFFFF,
                            0x00000000,
                            0x00020000,
                            ListHead);
  if (!EFI_ERROR (Status)) {
    DEBUG((DEBUG_INFO, "Max/MinFixed, Region Length Set Address Gran not 0  // >0 1 1\n"));
  }
  UT_ASSERT_EQUAL(EFI_INVALID_PARAMETER, Status);

  // Max/MinFixed, Region Length Set Address Bad Length  // >0 1 1
  Status = AmlOPDWordMemory(ResourceProducer,
                            PosDecode,
                            MinFixed,
                            MaxFixed,
                            Cacheable,
                            ReadWrite,
                            0x00000000,
                            0x000C0000,
                            0x000DFFFF,
                            0x00000000,
                            0x00002000,
                            ListHead);
  if (!EFI_ERROR (Status)) {
    DEBUG((DEBUG_INFO, "Max/MinFixed, Region Length Set Address Bad Length  // >0 1 1\n"));
  }
  UT_ASSERT_EQUAL(EFI_INVALID_PARAMETER, Status);

  // MaxNotFixed, MinAddr > MaxAddr
  Status = AmlOPDWordMemory(ResourceProducer,
                            PosDecode,
                            MinFixed,
                            MaxNotFixed,
                            Cacheable,
                            ReadWrite,
                            0x00000000,
                            0x08000000,
                            0x00000000,
                            0x00000000,
                            0x00000000,
                            ListHead);
  if (!EFI_ERROR (Status)) {
    DEBUG((DEBUG_INFO, "MaxNotFixed, MinAddr > MaxAddr\n"));
  }
  UT_ASSERT_EQUAL(EFI_INVALID_PARAMETER, Status);

  // MaxNotFixed, Granularity Mismatch
  Status = AmlOPDWordMemory(ResourceProducer,
                            PosDecode,
                            MinFixed,
                            MaxNotFixed,
                            Cacheable,
                            ReadWrite,
                            0x7FFFFFFF,
                            0x08000000,
                            0xFFFFFFFF,
                            0x00000000,
                            0x10000000,
                            ListHead);
  if (!EFI_ERROR (Status)) {
    DEBUG((DEBUG_INFO, "MaxNotFixed, Granularity Mismatch\n"));
  }
  UT_ASSERT_EQUAL(EFI_INVALID_PARAMETER, Status);

  // Length Set Max Fixed
  Status = AmlOPDWordMemory(ResourceProducer,
                            PosDecode,
                            MinNotFixed,
                            MaxFixed,
                            Cacheable,
                            ReadWrite,
                            0x00000000,
                            0x00000000,
                            0x007FFFFF,
                            0x00000000,
                            0x00040000,
                            ListHead);
  if (!EFI_ERROR (Status)) {
    DEBUG((DEBUG_INFO, "Length Set Max Fixed Min Not  // >0 0 1\n"));
  }
  UT_ASSERT_EQUAL(EFI_INVALID_PARAMETER, Status);

  // Length Set Max Min Fixed
  Status = AmlOPDWordMemory(ResourceProducer,
                            PosDecode,
                            MinFixed,
                            MaxNotFixed,
                            Cacheable,
                            ReadWrite,
                            0x00000000,
                            0x00100000,
                            0x007FFFFF,
                            0x00000000,
                            0x00500000,
                            ListHead);
  if (!EFI_ERROR (Status)) {
    DEBUG((DEBUG_INFO, "Length Set Min Fixed Max Not  // >0 1 0\n"));
  }
  UT_ASSERT_EQUAL(EFI_INVALID_PARAMETER, Status);

  Status = AmlOPDWordMemory(ResourceProducer,
                            SubDecode,
                            MinFixed,
                            MaxFixed,
                            Cacheable,
                            ReadWrite,
                            0x00010000,
                            0x000C0000,
                            0x000DFFFF,
                            0x00000000,
                            0x00020000,
                            ListHead);
  if (!EFI_ERROR (Status)) {
    DEBUG((DEBUG_INFO, "Granularity Mismatch?\n"));
  }
  UT_ASSERT_EQUAL(EFI_INVALID_PARAMETER, Status);

  if (!EFI_ERROR (Status)) {
    AmlDebugPrintLinkedObjects (ListHead);
  }
  AmlFreeObjectList (ListHead);

  return UNIT_TEST_PASSED;
}

/***************************************************************/
/* AML DefDWordMemory Happy Path Tests                        */
/***************************************************************/

/***************************************************************/
// Name(TSTA, ResourceTemplate()
/***************************************************************/
UNIT_TEST_STATUS
EFIAPI
TestDWM_PosAllFixedCacheRW(
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

      Status = AmlOPDWordMemory(ResourceProducer,
                                PosDecode,
                                MinFixed,
                                MaxFixed,
                                Cacheable,
                                ReadWrite,
                                0x00000000,
                                0x000C0000,
                                0x000DFFFF,
                                0x00000000,
                                0x00020000,
                                ListHead);
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
  UT_ASSERT_MEM_EQUAL(AslDWM_PosAllFixedCacheRW, MainObject->Data, sizeof(AslDWM_PosAllFixedCacheRW));
  UT_ASSERT_EQUAL(MainObject->DataSize, sizeof(AslDWM_PosAllFixedCacheRW));

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
TestDWM_SubAllFixedCacheRW(
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

      Status = AmlOPDWordMemory(ResourceProducer,
                                SubDecode,
                                MinFixed,
                                MaxFixed,
                                Cacheable,
                                ReadWrite,
                                0x00000000,
                                0x000C0000,
                                0x000DFFFF,
                                0x00000000,
                                0x00020000,
                                ListHead);
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
  UT_ASSERT_MEM_EQUAL(AslDWM_SubAllFixedCacheRW, MainObject->Data, sizeof(AslDWM_SubAllFixedCacheRW));
  UT_ASSERT_EQUAL(MainObject->DataSize, sizeof(AslDWM_SubAllFixedCacheRW));

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
TestDWM_PosMaxFixedCacheRW(
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

      Status = AmlOPDWordMemory(ResourceProducer,
                                PosDecode,
                                MinNotFixed,
                                MaxFixed,
                                Cacheable,
                                ReadWrite,
                                0x00000000,
                                0x00000000,
                                0x000DFFFF,
                                0x00000000,
                                0x00000000,
                                ListHead);
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
  UT_ASSERT_MEM_EQUAL(AslDWM_PosMaxFixedCacheRW, MainObject->Data, sizeof(AslDWM_PosMaxFixedCacheRW));
  UT_ASSERT_EQUAL(MainObject->DataSize, sizeof(AslDWM_PosMaxFixedCacheRW));

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
TestDWM_PosMinFixedCacheRW(
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

      Status = AmlOPDWordMemory(ResourceProducer,
                                PosDecode,
                                MinFixed,
                                MaxNotFixed,
                                Cacheable,
                                ReadWrite,
                                0x00000000,
                                0x000C0000,
                                0x000DFFFF,
                                0x00000000,
                                0x00000000,
                                ListHead);
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
  UT_ASSERT_MEM_EQUAL(AslDWM_PosMinFixedCacheRW, MainObject->Data, sizeof(AslDWM_PosMinFixedCacheRW));
  UT_ASSERT_EQUAL(MainObject->DataSize, sizeof(AslDWM_PosMinFixedCacheRW));

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
TestDWM_PosAllFixedWrCombRW(
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

      Status = AmlOPDWordMemory(ResourceProducer,
                                PosDecode,
                                MinFixed,
                                MaxFixed,
                                WriteCombining,
                                ReadWrite,
                                0x00000000,
                                0x000C0000,
                                0x000DFFFF,
                                0x00000000,
                                0x00020000,
                                ListHead);
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
  UT_ASSERT_MEM_EQUAL(AslDWM_PosAllFixedWrCombRW, MainObject->Data, sizeof(AslDWM_PosAllFixedWrCombRW));
  UT_ASSERT_EQUAL(MainObject->DataSize, sizeof(AslDWM_PosAllFixedWrCombRW));

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
TestDWM_PosAllFixedPrefetchRW(
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

      Status = AmlOPDWordMemory(ResourceProducer,
                                PosDecode,
                                MinFixed,
                                MaxFixed,
                                Prefetchable,
                                ReadWrite,
                                0x00000000,
                                0x000C0000,
                                0x000DFFFF,
                                0x00000000,
                                0x00020000,
                                ListHead);
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
  UT_ASSERT_MEM_EQUAL(AslDWM_PosAllFixedPrefetchRW, MainObject->Data, sizeof(AslDWM_PosAllFixedPrefetchRW));
  UT_ASSERT_EQUAL(MainObject->DataSize, sizeof(AslDWM_PosAllFixedPrefetchRW));

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
TestDWM_PosAllFixedNonCacheRW(
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

      Status = AmlOPDWordMemory(ResourceProducer,
                                PosDecode,
                                MinFixed,
                                MaxFixed,
                                NonCacheable,
                                ReadWrite,
                                0x00000000,
                                0x000C0000,
                                0x000DFFFF,
                                0x00000000,
                                0x00020000,
                                ListHead);
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
  UT_ASSERT_MEM_EQUAL(AslDWM_PosAllFixedNonCacheRW, MainObject->Data, sizeof(AslDWM_PosAllFixedNonCacheRW));
  UT_ASSERT_EQUAL(MainObject->DataSize, sizeof(AslDWM_PosAllFixedNonCacheRW));

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
TestDWM_PosAllFixedCacheRO(
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

      Status = AmlOPDWordMemory(ResourceProducer,
                                PosDecode,
                                MinFixed,
                                MaxFixed,
                                Cacheable,
                                ReadOnly,
                                0x00000000,
                                0x000C0000,
                                0x000DFFFF,
                                0x00000000,
                                0x00020000,
                                ListHead);
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
  UT_ASSERT_MEM_EQUAL(AslDWM_PosAllFixedCacheRO, MainObject->Data, sizeof(AslDWM_PosAllFixedCacheRO));
  UT_ASSERT_EQUAL(MainObject->DataSize, sizeof(AslDWM_PosAllFixedCacheRO));

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
TestDWM_LargeMinMaxFixed(
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

      Status = AmlOPDWordMemory(ResourceProducer,
                                PosDecode,
                                MinFixed,
                                MaxFixed,
                                Cacheable,
                                ReadWrite,
                                0x00000000,
                                0x02000000,
                                0xFFDFFFFF,
                                0x00000000,
                                0xFDE00000,
                                ListHead);
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
  UT_ASSERT_MEM_EQUAL(AslDWM_LargeMinMaxFixed, MainObject->Data, sizeof(AslDWM_LargeMinMaxFixed));
  UT_ASSERT_EQUAL(MainObject->DataSize, sizeof(AslDWM_LargeMinMaxFixed));

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
TestDWM_FixedMaxGranularity(
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

      Status = AmlOPDWordMemory(ResourceProducer,
                                PosDecode,
                                MinNotFixed,
                                MaxFixed,
                                Cacheable,
                                ReadWrite,
                                0x7FFFFFFF,
                                0x00000000,
                                0xFFFFFFFF,
                                0x00000000,
                                0x00000000,
                                ListHead);
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
  UT_ASSERT_MEM_EQUAL(AslDWM_FixedMaxGranularity, MainObject->Data, sizeof(AslDWM_FixedMaxGranularity));
  UT_ASSERT_EQUAL(MainObject->DataSize, sizeof(AslDWM_FixedMaxGranularity));

  if (!EFI_ERROR (Status)) {
    AmlDebugPrintLinkedObjects (ListHead);
  }
  AmlFreeObjectList (ListHead);

  return UNIT_TEST_PASSED;
}

/***************************************************************/
// Name(TSTK, ResourceTemplate()
/***************************************************************/
UNIT_TEST_STATUS
EFIAPI
TestDWM_FixedMinGranularity(
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

    // ResourceTemplate()
    Status = AmlResourceTemplate(AmlStart, ListHead);
    UT_ASSERT_NOT_EFI_ERROR(Status);

      Status = AmlOPDWordMemory(ResourceProducer,
                                PosDecode,
                                MinFixed,
                                MaxNotFixed,
                                Cacheable,
                                ReadWrite,
                                0x7FFFFFFF,
                                0x80000000,
                                0xFFFFFFFF,
                                0x00000000,
                                0x00000000,
                                ListHead);
      UT_ASSERT_NOT_EFI_ERROR(Status);

    Status = AmlResourceTemplate(AmlClose, ListHead);
    UT_ASSERT_NOT_EFI_ERROR(Status);

  // TSTK Name Close
  Status = AmlName(AmlClose, "TSTK", ListHead);
  UT_ASSERT_NOT_EFI_ERROR(Status);

  // Get first Child Node
  Node = GetFirstNode(ListHead);
  MainObject = AML_OBJECT_INSTANCE_FROM_LINK (Node);
  UT_ASSERT_NOT_NULL(MainObject->Data);
  UT_ASSERT_MEM_EQUAL(AslDWM_FixedMinGranularity, MainObject->Data, sizeof(AslDWM_FixedMinGranularity));
  UT_ASSERT_EQUAL(MainObject->DataSize, sizeof(AslDWM_FixedMinGranularity));

  if (!EFI_ERROR (Status)) {
    AmlDebugPrintLinkedObjects (ListHead);
  }
  AmlFreeObjectList (ListHead);

  return UNIT_TEST_PASSED;
}

/***************************************************************/
// Name(TSTL, ResourceTemplate()
/***************************************************************/
UNIT_TEST_STATUS
EFIAPI
TestDWM_FixedMinMaxTrans(
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

  // TSTL Name Start
  Status = AmlName(AmlStart, "TSTL", ListHead);
  UT_ASSERT_NOT_EFI_ERROR(Status);

    // ResourceTemplate()
    Status = AmlResourceTemplate(AmlStart, ListHead);
    UT_ASSERT_NOT_EFI_ERROR(Status);

      Status = AmlOPDWordMemory(ResourceProducer,
                                PosDecode,
                                MinFixed,
                                MaxFixed,
                                Cacheable,
                                ReadWrite,
                                0x00000000,
                                0x02000000,
                                0xFFDFFFFF,
                                0xFD010000,
                                0xFDE00000,
                                ListHead);
      UT_ASSERT_NOT_EFI_ERROR(Status);

    Status = AmlResourceTemplate(AmlClose, ListHead);
    UT_ASSERT_NOT_EFI_ERROR(Status);

  // TSTL Name Close
  Status = AmlName(AmlClose, "TSTL", ListHead);
  UT_ASSERT_NOT_EFI_ERROR(Status);

  // Get first Child Node
  Node = GetFirstNode(ListHead);
  MainObject = AML_OBJECT_INSTANCE_FROM_LINK (Node);
  UT_ASSERT_NOT_NULL(MainObject->Data);
  UT_ASSERT_MEM_EQUAL(AslDWM_FixedMinMaxTrans, MainObject->Data, sizeof(AslDWM_FixedMinMaxTrans));
  UT_ASSERT_EQUAL(MainObject->DataSize, sizeof(AslDWM_FixedMinMaxTrans));

  if (!EFI_ERROR (Status)) {
    AmlDebugPrintLinkedObjects (ListHead);
  }
  AmlFreeObjectList (ListHead);

  return UNIT_TEST_PASSED;
}

/***************************************************************/
// Name(TSTM, ResourceTemplate()
/***************************************************************/
UNIT_TEST_STATUS
EFIAPI
TestDWM_FixedMinGranTrans(
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

    // ResourceTemplate()
    Status = AmlResourceTemplate(AmlStart, ListHead);
    UT_ASSERT_NOT_EFI_ERROR(Status);

      Status = AmlOPDWordMemory(ResourceProducer,
                                PosDecode,
                                MinFixed,
                                MaxNotFixed,
                                Cacheable,
                                ReadWrite,
                                0x7FFFFFFF,
                                0x80000000,
                                0xFFFFFFFF,
                                0x00010000,
                                0x00000000,
                                ListHead);
      UT_ASSERT_NOT_EFI_ERROR(Status);

    Status = AmlResourceTemplate(AmlClose, ListHead);
    UT_ASSERT_NOT_EFI_ERROR(Status);

  // TSTM Name Close
  Status = AmlName(AmlClose, "TSTM", ListHead);
  UT_ASSERT_NOT_EFI_ERROR(Status);

  // Get first Child Node
  Node = GetFirstNode(ListHead);
  MainObject = AML_OBJECT_INSTANCE_FROM_LINK (Node);
  UT_ASSERT_NOT_NULL(MainObject->Data);
  UT_ASSERT_MEM_EQUAL(AslDWM_FixedMinGranTrans, MainObject->Data, sizeof(AslDWM_FixedMinGranTrans));
  UT_ASSERT_EQUAL(MainObject->DataSize, sizeof(AslDWM_FixedMinGranTrans));

  if (!EFI_ERROR (Status)) {
    AmlDebugPrintLinkedObjects (ListHead);
  }
  AmlFreeObjectList (ListHead);

  return UNIT_TEST_PASSED;
}

/***************************************************************/
// Creates and runs the DWordMemory Test Suite
/***************************************************************/
EFI_STATUS
EFIAPI
DWordMemoryTestMain (
  VOID
  )
{
  EFI_STATUS                  Status;
  UNIT_TEST_FRAMEWORK_HANDLE  Framework;
  UNIT_TEST_SUITE_HANDLE      DWordMemoryTestSuite;

  DEBUG((DEBUG_INFO, "%a v%a\n", DWORDMEMORY_UNIT_TEST_NAME, DWORDMEMORY_UNIT_TEST_VERSION));

  //
  // Start setting up the test framework for running the tests.
  //
  Status = InitUnitTestFramework (&Framework, DWORDMEMORY_UNIT_TEST_NAME, gEfiCallerBaseName, DWORDMEMORY_UNIT_TEST_VERSION);
  if (EFI_ERROR(Status)) {
    DEBUG((DEBUG_ERROR, "Failed in InitUnitTestFramework. Status = %r\n", Status));
    goto Done;
  }

  ///
  // Test the DWordMemory function
  //
  Status = CreateUnitTestSuite (&DWordMemoryTestSuite, Framework, "DWordMemory Test Suite", "AmdEdk2Common.Lib.AmlLib", NULL, NULL);
  if (EFI_ERROR(Status)) {
    DEBUG((DEBUG_ERROR, "Failed in CreateUnitTestSuite for DWordMemory Test Suite\n"));
    Status = EFI_OUT_OF_RESOURCES;
    goto Done;
  }
  AddTestCase(DWordMemoryTestSuite, "DWordMemory Test Input Validation",  "DWordMemoryTestInputValidation",  DWordMemoryTestInputValidation, NULL, NULL, NULL);

  AddTestCase(DWordMemoryTestSuite, "Name(TSTA, ResourceTemplate()",  "TestDWM_PosAllFixedCacheRW",  TestDWM_PosAllFixedCacheRW, NULL, NULL, NULL);
  AddTestCase(DWordMemoryTestSuite, "Name(TSTB, ResourceTemplate()",  "TestDWM_SubAllFixedCacheRW",  TestDWM_SubAllFixedCacheRW, NULL, NULL, NULL);
  AddTestCase(DWordMemoryTestSuite, "Name(TSTC, ResourceTemplate()",  "TestDWM_PosMaxFixedCacheRW",  TestDWM_PosMaxFixedCacheRW, NULL, NULL, NULL);
  AddTestCase(DWordMemoryTestSuite, "Name(TSTD, ResourceTemplate()",  "TestDWM_PosMinFixedCacheRW",  TestDWM_PosMinFixedCacheRW, NULL, NULL, NULL);
  AddTestCase(DWordMemoryTestSuite, "Name(TSTE, ResourceTemplate()",  "TestDWM_PosAllFixedWrCombRW",  TestDWM_PosAllFixedWrCombRW, NULL, NULL, NULL);
  AddTestCase(DWordMemoryTestSuite, "Name(TSTF, ResourceTemplate()",  "TestDWM_PosAllFixedPrefetchRW",  TestDWM_PosAllFixedPrefetchRW, NULL, NULL, NULL);
  AddTestCase(DWordMemoryTestSuite, "Name(TSTG, ResourceTemplate()",  "TestDWM_PosAllFixedNonCacheRW",  TestDWM_PosAllFixedNonCacheRW, NULL, NULL, NULL);
  AddTestCase(DWordMemoryTestSuite, "Name(TSTH, ResourceTemplate()",  "TestDWM_PosAllFixedCacheRO",  TestDWM_PosAllFixedCacheRO, NULL, NULL, NULL);
  AddTestCase(DWordMemoryTestSuite, "Name(TSTI, ResourceTemplate()",  "TestDWM_LargeMinMaxFixed",  TestDWM_LargeMinMaxFixed, NULL, NULL, NULL);
  AddTestCase(DWordMemoryTestSuite, "Name(TSTJ, ResourceTemplate()",  "TestDWM_FixedMaxGranularity",  TestDWM_FixedMaxGranularity, NULL, NULL, NULL);
  AddTestCase(DWordMemoryTestSuite, "Name(TSTK, ResourceTemplate()",  "TestDWM_FixedMinGranularity",  TestDWM_FixedMinGranularity, NULL, NULL, NULL);
  AddTestCase(DWordMemoryTestSuite, "Name(TSTL, ResourceTemplate()",  "TestDWM_FixedMinMaxTrans",  TestDWM_FixedMinMaxTrans, NULL, NULL, NULL);
  AddTestCase(DWordMemoryTestSuite, "Name(TSTM, ResourceTemplate()",  "TestDWM_FixedMinGranTrans",  TestDWM_FixedMinGranTrans, NULL, NULL, NULL);

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
