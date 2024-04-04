/*****************************************************************************
 *
 * Copyright (C) 2020-2024 Advanced Micro Devices, Inc. All rights reserved.
 *
 *****************************************************************************/

#include <Library/BaseLib.h>
#include <Uefi.h>
#include "AmlTests.h"
#include <Library/BaseMemoryLib.h>

#include <Library/DebugLib.h>

/**
  Entry point for AML Unit Test Driver

  @param ImageHandle  Image handle of this driver.
  @param SystemTable  Pointer to standard EFI system table.

  @retval         EFI_SUCCESS
  @retval         Error status
**/
EFI_STATUS
EFIAPI
UefiTestMain (
  VOID
  )
{
  EFI_STATUS     Status;
  Status = EFI_SUCCESS;

// INSERT HERE

  Status = AliasTestMain();
  if(EFI_ERROR(Status)) {
    DEBUG ((DEBUG_ERROR, "%a: ERROR in Alias Unit Test\n", __FUNCTION__));
  }

  Status = EisaIdTestMain();
  if(EFI_ERROR(Status)) {
    DEBUG ((DEBUG_ERROR, "%a: ERROR in EisaId Unit Test\n", __FUNCTION__));
  }

  Status = QWordMemoryTestMain();
  if(EFI_ERROR(Status)) {
    DEBUG ((DEBUG_ERROR, "%a: ERROR in QWordMemory Unit Test\n", __FUNCTION__));
  }

  Status = DWordMemoryTestMain();
  if(EFI_ERROR(Status)) {
    DEBUG ((DEBUG_ERROR, "%a: ERROR in DWordMemory Unit Test\n", __FUNCTION__));
  }

  Status = DmaTestMain();
  if(EFI_ERROR(Status)) {
    DEBUG ((DEBUG_ERROR, "%a: ERROR in Dma Unit Test\n", __FUNCTION__));
  }

  Status = PackageTestMain();
  if(EFI_ERROR(Status)) {
    DEBUG ((DEBUG_ERROR, "%a: ERROR in Package Unit Test\n", __FUNCTION__));
  }

  Status = DefDataIntegerTestMain();
  if(EFI_ERROR(Status)) {
    DEBUG ((DEBUG_ERROR, "%a: ERROR in DefDataInteger Unit Test\n", __FUNCTION__));
  }

  Status = DefDataStringTestMain();
  if(EFI_ERROR(Status)) {
    DEBUG ((DEBUG_ERROR, "%a: ERROR in DefDataString Unit Test\n", __FUNCTION__));
  }

  Status = DefNameStringTestMain();
  if(EFI_ERROR(Status)) {
    DEBUG ((DEBUG_ERROR, "%a: ERROR in DefNameString Unit Test\n", __FUNCTION__));
  }

  Status = DefReturnTestMain();
  if(EFI_ERROR(Status)) {
    DEBUG ((DEBUG_ERROR, "%a: ERROR in DefReturn Unit Test\n", __FUNCTION__));
  }

  Status = DefExternalTestMain();
  if(EFI_ERROR(Status)) {
    DEBUG ((DEBUG_ERROR, "%a: ERROR in DefExternal Unit Test\n", __FUNCTION__));
  }

  Status = DefMethodTestMain();
  if(EFI_ERROR(Status)) {
    DEBUG ((DEBUG_ERROR, "%a: ERROR in DefMethod Unit Test\n", __FUNCTION__));
  }

  return Status;
}

/**
  Standard UEFI entry point for target based unit test execution from DXE, SMM,
  UEFI Shell.
**/
EFI_STATUS
EFIAPI
DxeEntryPoint (
  IN EFI_HANDLE        ImageHandle,
  IN EFI_SYSTEM_TABLE  *SystemTable
  )
{
  return UefiTestMain ();
}

INT32
main (
  INT32 Argc,
  CHAR8 *Argv[]
  )
{
  return UefiTestMain ();
}

