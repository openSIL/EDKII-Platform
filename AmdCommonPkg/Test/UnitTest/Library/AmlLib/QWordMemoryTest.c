
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


#define QWORDMEMORY_UNIT_TEST_NAME        "DefQWordMemory Test Application"
#define QWORDMEMORY_UNIT_TEST_VERSION     "0.1"

UINT8   AslQWM_PosAllFixedCacheRW[] = { 0x08,0x54,0x53,0x54,0x41,0x11,0x33,0x0A,0x30,0x8A,0x2B,0x00,0x00,0x0C,0x03,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x0C,0x00,0x00,0x00,0x00,0x00,0xFF,0xFF,0x0D,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x02,0x00,0x00,0x00,0x00,0x00,0x79,0x00 };

UINT8   AslQWM_SubAllFixedCacheRW[] = { 0x08,0x54,0x53,0x54,0x42,0x11,0x33,0x0A,0x30,0x8A,0x2B,0x00,0x00,0x0E,0x03,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x0C,0x00,0x00,0x00,0x00,0x00,0xFF,0xFF,0x0D,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x02,0x00,0x00,0x00,0x00,0x00,0x79,0x00 };

UINT8   AslQWM_PosMaxFixedCacheRW[] = { 0x08,0x54,0x53,0x54,0x43,0x11,0x33,0x0A,0x30,0x8A,0x2B,0x00,0x00,0x08,0x03,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xFF,0xFF,0x0D,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x79,0x00 };

UINT8   AslQWM_PosMinFixedCacheRW[] = { 0x08,0x54,0x53,0x54,0x44,0x11,0x33,0x0A,0x30,0x8A,0x2B,0x00,0x00,0x04,0x03,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x0C,0x00,0x00,0x00,0x00,0x00,0xFF,0xFF,0x0D,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x79,0x00 };

UINT8   AslQWM_PosAllFixedWrCombRW[] = { 0x08,0x54,0x53,0x54,0x45,0x11,0x33,0x0A,0x30,0x8A,0x2B,0x00,0x00,0x0C,0x05,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x0C,0x00,0x00,0x00,0x00,0x00,0xFF,0xFF,0x0D,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x02,0x00,0x00,0x00,0x00,0x00,0x79,0x00 };

UINT8   AslQWM_PosAllFixedPrefetchRW[] = { 0x08,0x54,0x53,0x54,0x46,0x11,0x33,0x0A,0x30,0x8A,0x2B,0x00,0x00,0x0C,0x07,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x0C,0x00,0x00,0x00,0x00,0x00,0xFF,0xFF,0x0D,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x02,0x00,0x00,0x00,0x00,0x00,0x79,0x00 };

UINT8   AslQWM_PosAllFixedNonCacheRW[] = { 0x08,0x54,0x53,0x54,0x47,0x11,0x33,0x0A,0x30,0x8A,0x2B,0x00,0x00,0x0C,0x01,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x0C,0x00,0x00,0x00,0x00,0x00,0xFF,0xFF,0x0D,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x02,0x00,0x00,0x00,0x00,0x00,0x79,0x00 };

UINT8   AslQWM_PosAllFixedCacheRO[] = { 0x08,0x54,0x53,0x54,0x48,0x11,0x33,0x0A,0x30,0x8A,0x2B,0x00,0x00,0x0C,0x02,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x0C,0x00,0x00,0x00,0x00,0x00,0xFF,0xFF,0x0D,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x02,0x00,0x00,0x00,0x00,0x00,0x79,0x00 };

UINT8   AslQWM_LargeMinMaxFixed[] = { 0x08,0x54,0x53,0x54,0x49,0x11,0x33,0x0A,0x30,0x8A,0x2B,0x00,0x00,0x0C,0x03,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x02,0x00,0x00,0x00,0x00,0xFF,0xFF,0xDF,0xFF,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xE0,0xFD,0x00,0x00,0x00,0x00,0x79,0x00 };

UINT8   AslQWM_FixedMaxGranularity[] = { 0x08,0x54,0x53,0x54,0x4A,0x11,0x33,0x0A,0x30,0x8A,0x2B,0x00,0x00,0x08,0x03,0xFF,0xFF,0xFF,0x7F,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xFF,0xFF,0xFF,0xFF,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x79,0x00 };

UINT8   AslQWM_FixedMinGranularity[] = { 0x08,0x54,0x53,0x54,0x4B,0x11,0x33,0x0A,0x30,0x8A,0x2B,0x00,0x00,0x04,0x03,0xFF,0xFF,0xFF,0x7F,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x80,0x00,0x00,0x00,0x00,0xFF,0xFF,0xFF,0xFF,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x79,0x00 };

UINT8   AslQWM_FixedMinMaxTrans[] = { 0x08,0x54,0x53,0x54,0x4C,0x11,0x33,0x0A,0x30,0x8A,0x2B,0x00,0x00,0x0C,0x03,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x02,0x00,0x00,0x00,0x00,0xFF,0xFF,0xDF,0xFF,0x00,0x00,0x00,0x00,0x00,0x00,0x01,0xFD,0x00,0x00,0x00,0x00,0x00,0x00,0xE0,0xFD,0x00,0x00,0x00,0x00,0x79,0x00 };

UINT8   AslQWM_FixedMinGranTrans[] = { 0x08,0x54,0x53,0x54,0x4D,0x11,0x33,0x0A,0x30,0x8A,0x2B,0x00,0x00,0x04,0x03,0xFF,0xFF,0xFF,0x7F,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x80,0x00,0x00,0x00,0x00,0xFF,0xFF,0xFF,0xFF,0x00,0x00,0x00,0x00,0x00,0x00,0x01,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x79,0x00 };

/***************************************************************/
/* AML DefQWordMemory Input Validation Tests                    */
/***************************************************************/
UNIT_TEST_STATUS
EFIAPI
QWordMemoryTestInputValidation(
  IN  UNIT_TEST_CONTEXT   Context
)
{
  EFI_STATUS           Status;
  LIST_ENTRY           *ListHead;
  LIST_ENTRY           AmlListHead;

  ListHead = &AmlListHead;

  InitializeListHead (ListHead);

  // ResourceUsage Param Invalid
  Status = AmlOPQWordMemory(0xAA,
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
  Status = AmlOPQWordMemory(ResourceProducer,
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
  Status = AmlOPQWordMemory(ResourceProducer,
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
  Status = AmlOPQWordMemory(ResourceProducer,
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
  Status = AmlOPQWordMemory(ResourceProducer,
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
  Status = AmlOPQWordMemory(ResourceProducer,
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
  Status = AmlOPQWordMemory(ResourceProducer,
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
  Status = AmlOPQWordMemory(ResourceProducer,
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
  Status = AmlOPQWordMemory(ResourceProducer,
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
  Status = AmlOPQWordMemory(ResourceProducer,
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
  Status = AmlOPQWordMemory(ResourceProducer,
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
  Status = AmlOPQWordMemory(ResourceProducer,
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
  Status = AmlOPQWordMemory(ResourceProducer,
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
  Status = AmlOPQWordMemory(ResourceProducer,
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
  Status = AmlOPQWordMemory(ResourceProducer,
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
  Status = AmlOPQWordMemory(ResourceProducer,
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
  Status = AmlOPQWordMemory(ResourceProducer,
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
  Status = AmlOPQWordMemory(ResourceProducer,
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
  Status = AmlOPQWordMemory(ResourceProducer,
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
  Status = AmlOPQWordMemory(ResourceProducer,
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
  Status = AmlOPQWordMemory(ResourceProducer,
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
  Status = AmlOPQWordMemory(ResourceProducer,
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

  Status = AmlOPQWordMemory(ResourceProducer,
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
/* AML DefQWordMemory Happy Path Tests                        */
/***************************************************************/

/***************************************************************/
// Name(TSTA, ResourceTemplate()
/***************************************************************/
UNIT_TEST_STATUS
EFIAPI
TestQWM_PosAllFixedCacheRW(
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

      Status = AmlOPQWordMemory(ResourceProducer,
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
  UT_ASSERT_MEM_EQUAL(AslQWM_PosAllFixedCacheRW, MainObject->Data, sizeof(AslQWM_PosAllFixedCacheRW));
  UT_ASSERT_EQUAL(MainObject->DataSize, sizeof(AslQWM_PosAllFixedCacheRW));

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
TestQWM_SubAllFixedCacheRW(
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

      Status = AmlOPQWordMemory(ResourceProducer,
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
  UT_ASSERT_MEM_EQUAL(AslQWM_SubAllFixedCacheRW, MainObject->Data, sizeof(AslQWM_SubAllFixedCacheRW));
  UT_ASSERT_EQUAL(MainObject->DataSize, sizeof(AslQWM_SubAllFixedCacheRW));

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
TestQWM_PosMaxFixedCacheRW(
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

      Status = AmlOPQWordMemory(ResourceProducer,
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
  UT_ASSERT_MEM_EQUAL(AslQWM_PosMaxFixedCacheRW, MainObject->Data, sizeof(AslQWM_PosMaxFixedCacheRW));
  UT_ASSERT_EQUAL(MainObject->DataSize, sizeof(AslQWM_PosMaxFixedCacheRW));

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
TestQWM_PosMinFixedCacheRW(
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

      Status = AmlOPQWordMemory(ResourceProducer,
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
  UT_ASSERT_MEM_EQUAL(AslQWM_PosMinFixedCacheRW, MainObject->Data, sizeof(AslQWM_PosMinFixedCacheRW));
  UT_ASSERT_EQUAL(MainObject->DataSize, sizeof(AslQWM_PosMinFixedCacheRW));

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
TestQWM_PosAllFixedWrCombRW(
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

      Status = AmlOPQWordMemory(ResourceProducer,
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
  UT_ASSERT_MEM_EQUAL(AslQWM_PosAllFixedWrCombRW, MainObject->Data, sizeof(AslQWM_PosAllFixedWrCombRW));
  UT_ASSERT_EQUAL(MainObject->DataSize, sizeof(AslQWM_PosAllFixedWrCombRW));

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
TestQWM_PosAllFixedPrefetchRW(
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

      Status = AmlOPQWordMemory(ResourceProducer,
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
  UT_ASSERT_MEM_EQUAL(AslQWM_PosAllFixedPrefetchRW, MainObject->Data, sizeof(AslQWM_PosAllFixedPrefetchRW));
  UT_ASSERT_EQUAL(MainObject->DataSize, sizeof(AslQWM_PosAllFixedPrefetchRW));

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
TestQWM_PosAllFixedNonCacheRW(
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

      Status = AmlOPQWordMemory(ResourceProducer,
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
  UT_ASSERT_MEM_EQUAL(AslQWM_PosAllFixedNonCacheRW, MainObject->Data, sizeof(AslQWM_PosAllFixedNonCacheRW));
  UT_ASSERT_EQUAL(MainObject->DataSize, sizeof(AslQWM_PosAllFixedNonCacheRW));

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
TestQWM_PosAllFixedCacheRO(
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

      Status = AmlOPQWordMemory(ResourceProducer,
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
  UT_ASSERT_MEM_EQUAL(AslQWM_PosAllFixedCacheRO, MainObject->Data, sizeof(AslQWM_PosAllFixedCacheRO));
  UT_ASSERT_EQUAL(MainObject->DataSize, sizeof(AslQWM_PosAllFixedCacheRO));

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
TestQWM_LargeMinMaxFixed(
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

      Status = AmlOPQWordMemory(ResourceProducer,
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
  UT_ASSERT_MEM_EQUAL(AslQWM_LargeMinMaxFixed, MainObject->Data, sizeof(AslQWM_LargeMinMaxFixed));
  UT_ASSERT_EQUAL(MainObject->DataSize, sizeof(AslQWM_LargeMinMaxFixed));

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
TestQWM_FixedMaxGranularity(
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

      Status = AmlOPQWordMemory(ResourceProducer,
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
  UT_ASSERT_MEM_EQUAL(AslQWM_FixedMaxGranularity, MainObject->Data, sizeof(AslQWM_FixedMaxGranularity));
  UT_ASSERT_EQUAL(MainObject->DataSize, sizeof(AslQWM_FixedMaxGranularity));

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
TestQWM_FixedMinGranularity(
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

      Status = AmlOPQWordMemory(ResourceProducer,
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
  UT_ASSERT_MEM_EQUAL(AslQWM_FixedMinGranularity, MainObject->Data, sizeof(AslQWM_FixedMinGranularity));
  UT_ASSERT_EQUAL(MainObject->DataSize, sizeof(AslQWM_FixedMinGranularity));

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
TestQWM_FixedMinMaxTrans(
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

      Status = AmlOPQWordMemory(ResourceProducer,
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
  UT_ASSERT_MEM_EQUAL(AslQWM_FixedMinMaxTrans, MainObject->Data, sizeof(AslQWM_FixedMinMaxTrans));
  UT_ASSERT_EQUAL(MainObject->DataSize, sizeof(AslQWM_FixedMinMaxTrans));

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
TestQWM_FixedMinGranTrans(
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

      Status = AmlOPQWordMemory(ResourceProducer,
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
  UT_ASSERT_MEM_EQUAL(AslQWM_FixedMinGranTrans, MainObject->Data, sizeof(AslQWM_FixedMinGranTrans));
  UT_ASSERT_EQUAL(MainObject->DataSize, sizeof(AslQWM_FixedMinGranTrans));

  if (!EFI_ERROR (Status)) {
    AmlDebugPrintLinkedObjects (ListHead);
  }
  AmlFreeObjectList (ListHead);

  return UNIT_TEST_PASSED;
}

/***************************************************************/
// Creates and runs the QWordMemory Test Suite
/***************************************************************/
EFI_STATUS
EFIAPI
QWordMemoryTestMain (
  VOID
  )
{
  EFI_STATUS                  Status;
  UNIT_TEST_FRAMEWORK_HANDLE  Framework;
  UNIT_TEST_SUITE_HANDLE      QWordMemoryTestSuite;

  DEBUG((DEBUG_INFO, "%a v%a\n", QWORDMEMORY_UNIT_TEST_NAME, QWORDMEMORY_UNIT_TEST_VERSION));

  //
  // Start setting up the test framework for running the tests.
  //
  Status = InitUnitTestFramework (&Framework, QWORDMEMORY_UNIT_TEST_NAME, gEfiCallerBaseName, QWORDMEMORY_UNIT_TEST_VERSION);
  if (EFI_ERROR(Status)) {
    DEBUG((DEBUG_ERROR, "Failed in InitUnitTestFramework. Status = %r\n", Status));
    goto Done;
  }

  ///
  // Test the QWordMemory function
  //
  Status = CreateUnitTestSuite (&QWordMemoryTestSuite, Framework, "QWordMemory Test Suite", "AmdEdk2Common.Lib.AmlLib", NULL, NULL);
  if (EFI_ERROR(Status)) {
    DEBUG((DEBUG_ERROR, "Failed in CreateUnitTestSuite for QWordMemory Test Suite\n"));
    Status = EFI_OUT_OF_RESOURCES;
    goto Done;
  }
  AddTestCase(QWordMemoryTestSuite, "QWordMemory Test Input Validation",  "QWordMemoryTestInputValidation",  QWordMemoryTestInputValidation, NULL, NULL, NULL);

  AddTestCase(QWordMemoryTestSuite, "Name(TSTA, ResourceTemplate()",  "TestQWM_PosAllFixedCacheRW",  TestQWM_PosAllFixedCacheRW, NULL, NULL, NULL);
  AddTestCase(QWordMemoryTestSuite, "Name(TSTB, ResourceTemplate()",  "TestQWM_SubAllFixedCacheRW",  TestQWM_SubAllFixedCacheRW, NULL, NULL, NULL);
  AddTestCase(QWordMemoryTestSuite, "Name(TSTC, ResourceTemplate()",  "TestQWM_PosMaxFixedCacheRW",  TestQWM_PosMaxFixedCacheRW, NULL, NULL, NULL);
  AddTestCase(QWordMemoryTestSuite, "Name(TSTD, ResourceTemplate()",  "TestQWM_PosMinFixedCacheRW",  TestQWM_PosMinFixedCacheRW, NULL, NULL, NULL);
  AddTestCase(QWordMemoryTestSuite, "Name(TSTE, ResourceTemplate()",  "TestQWM_PosAllFixedWrCombRW",  TestQWM_PosAllFixedWrCombRW, NULL, NULL, NULL);
  AddTestCase(QWordMemoryTestSuite, "Name(TSTF, ResourceTemplate()",  "TestQWM_PosAllFixedPrefetchRW",  TestQWM_PosAllFixedPrefetchRW, NULL, NULL, NULL);
  AddTestCase(QWordMemoryTestSuite, "Name(TSTG, ResourceTemplate()",  "TestQWM_PosAllFixedNonCacheRW",  TestQWM_PosAllFixedNonCacheRW, NULL, NULL, NULL);
  AddTestCase(QWordMemoryTestSuite, "Name(TSTH, ResourceTemplate()",  "TestQWM_PosAllFixedCacheRO",  TestQWM_PosAllFixedCacheRO, NULL, NULL, NULL);
  AddTestCase(QWordMemoryTestSuite, "Name(TSTI, ResourceTemplate()",  "TestQWM_LargeMinMaxFixed",  TestQWM_LargeMinMaxFixed, NULL, NULL, NULL);
  AddTestCase(QWordMemoryTestSuite, "Name(TSTJ, ResourceTemplate()",  "TestQWM_FixedMaxGranularity",  TestQWM_FixedMaxGranularity, NULL, NULL, NULL);
  AddTestCase(QWordMemoryTestSuite, "Name(TSTK, ResourceTemplate()",  "TestQWM_FixedMinGranularity",  TestQWM_FixedMinGranularity, NULL, NULL, NULL);
  AddTestCase(QWordMemoryTestSuite, "Name(TSTL, ResourceTemplate()",  "TestQWM_FixedMinMaxTrans",  TestQWM_FixedMinMaxTrans, NULL, NULL, NULL);
  AddTestCase(QWordMemoryTestSuite, "Name(TSTM, ResourceTemplate()",  "TestQWM_FixedMinGranTrans",  TestQWM_FixedMinGranTrans, NULL, NULL, NULL);


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
