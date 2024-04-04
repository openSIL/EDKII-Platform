/*
 * Copyright (C) 2023-2024 Advanced Micro Devices, Inc. All rights reserved.
 */

#include <Uefi/UefiBaseType.h>
#include <PiPei.h>
#include <Library/PcdLib.h>
#include <Library/DebugLib.h>

/**
 * @brief   Entry point for Platform PEI.
 *
 * @param   FileHandle Pointer to the FFS file header.
 * @param   PeiServices Pointer to the PEI services table.
 *
 * @retval  EFI_STATUS EFI_SUCCESS
 *          EFI_STATUS respective failure status.
**/
EFI_STATUS
EFIAPI
AmdPlatformPeiInit (
  IN       EFI_PEI_FILE_HANDLE  FileHandle,
  IN CONST EFI_PEI_SERVICES     **PeiServices
  )
{
  EFI_STATUS                   Status;
  UINT64                       PcieBaseSize;
  UINT8                        BusRange;

  Status = EFI_SUCCESS;

  // Gather the value of PcdPciExpressBaseSize from MSR
  BusRange =  RShiftU64 (AsmReadMsr64 (0xC0010058), 2) & 0xF;
  PcieBaseSize = LShiftU64 (1, BusRange) * SIZE_1MB;

  //PcdSet64S (PcdPciExpressBaseSize, (UINT64)PcieBaseSize);

  return Status;
}