/*****************************************************************************
 *
 * Copyright (C) 2008-2023 Advanced Micro Devices, Inc. All rights reserved.
 *
 *******************************************************************************
 */
#include <PiSmm.h>

#include <Register/Cpuid.h>
#include <Library/BaseLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/SmmServicesTableLib.h>
#include <Library/DebugLib.h>
#include <Library/AmdPspBaseLibV2.h>
#include <Protocol/SmmBase2.h>
#include <Protocol/SpiSmmNorFlash.h>

STATIC EFI_SPI_NOR_FLASH_PROTOCOL *mSpiNorFlashProtocol;
STATIC UINT32 mSpiRomMmioBase;

// *** Rome Hack Start ***
UINT32      mSpiHackOffset;

/*
 * Temporary function to Adjust LBA based on where in the SPI part the BIOS
 * image resides.  Assumes multiple 16MB images in SPI ROM larger than 16MB
 */
VOID
TempSetmSpiHackOffset (
  VOID
  )
{
  UINT32  CpuFamily;
  UINT32  CpuModel;
  CPUID_VERSION_INFO_EAX  VersionInfoEax;
  //
  // The assumed NV layout is: Var+Ftw+Spare
  //
  //
  // Check for Rome-RevA# (Revision prior to B0)
  //
  //  Revision Letter (ASCII) = CPUID_1.EAX[7:4] (BaseModel) + 'A'
  //  Revision Number (ASCII) = CPUID_1.EAX[3:0] (Stepping) + '0'
  //
  mSpiHackOffset = 0;
  AsmCpuid (CPUID_VERSION_INFO, &VersionInfoEax.Uint32, NULL, NULL, NULL);
  CpuFamily = VersionInfoEax.Bits.ExtendedFamilyId
              + VersionInfoEax.Bits.FamilyId;
  CpuModel = (VersionInfoEax.Bits.ExtendedModelId << 4)
            + VersionInfoEax.Bits.Model;
  if (CpuFamily == 0x17 && CpuModel == 0x30) {
    mSpiHackOffset = (mSpiNorFlashProtocol->FlashSize - SIZE_16MB);
  }
  DEBUG((EFI_D_INFO, "%a - mNvSpiHackOffset = %X\n", __FUNCTION__, mSpiHackOffset));
}
// *** Rome Hack End ***

/**
 * Retrieve block size of Flash Device
 *
 * @param[in]       BlockSize       Size of each block
 *
 * @retval EFI_SUCCESS      Initial success
 * @retval Others           Error happens during initialize
 */
EFI_STATUS
EFIAPI
PspGetFlashBlockSize (
     OUT   UINTN                                       *BlockSize
  )
{
  *BlockSize = FixedPcdGet32 (PcdFlashNvStorageBlockSize);
  return EFI_SUCCESS;
}

/**
 * Read data from Flash device
 *
 * @param[in]       FlashAddress Physical flash address
 * @param[in, out]  NumBytes     Number in Byte; return Bytes been read on output
 * @param[in]       Buffer       Buffer contain the read data (Allocated by caller)
 *
 * @retval EFI_SUCCESS      Initial success
 * @retval Others           Error happens during initialize
 */
EFI_STATUS
EFIAPI
PspReadFlash (
  IN       UINTN                                        FlashAddress,
  IN OUT   UINTN                                        *NumBytes,
     OUT   UINT8                                        *Buffer
  )
{
  EFI_STATUS Status;
  UINT32 SpiOffset;

  SpiOffset = (UINT32)FlashAddress - mSpiRomMmioBase;
  SpiOffset += mSpiHackOffset;

  Status = mSpiNorFlashProtocol->ReadData (
      mSpiNorFlashProtocol,
      SpiOffset,
      (UINT32)*NumBytes,
      Buffer
      );
  DEBUG ((EFI_D_INFO , "PspReadFlash %x %x - %r\n", FlashAddress, *NumBytes, Status));

  return Status;
}


/**
 * Write data to Flash device
 *
 * @param[in]       FlashAddress Physical flash address
 * @param[in, out]  NumBytes     Number in Byte; return Bytes been written on output
 * @param[in]       Buffer       Buffer contain the written data (Allocated by caller)
 *
 * @retval EFI_SUCCESS      Initial success
 * @retval Others           Error happens during initialize
 */
EFI_STATUS
EFIAPI
PspWriteFlash (
  IN       UINTN                                    FlashAddress,
  IN OUT   UINTN                                    *NumBytes,
  IN       UINT8                                    *Buffer
  )
{
  EFI_STATUS Status;
  UINT32 SpiOffset;

  SpiOffset = (UINT32)FlashAddress - mSpiRomMmioBase;
  SpiOffset += mSpiHackOffset;

  Status = mSpiNorFlashProtocol->WriteData (
      mSpiNorFlashProtocol,
      SpiOffset,
      (UINT32)*NumBytes,
      Buffer
      );
  DEBUG ((EFI_D_INFO , "PspWriteFlash %x %x - %r\n", FlashAddress, *NumBytes, Status));

  return Status;
}

/**
 * Erase Flash region according to input in unit of block size
 *
 *
 * @param[in]       FlashAddress Physical flash address
 * @param[in, out]  NumBytes     Number in Byte; return Bytes been erased on output
 *
 * @retval EFI_SUCCESS      Initial success
 * @retval Others           Error happens during initialize
 */
EFI_STATUS
EFIAPI
PspEraseFlash (
  IN       UINTN                                    FlashAddress,
  IN OUT   UINTN                                    *NumBytes
  )
{
  EFI_STATUS Status;
  UINT32 SpiOffset;

  SpiOffset = (UINT32)FlashAddress - mSpiRomMmioBase;
  SpiOffset += mSpiHackOffset;

  Status = mSpiNorFlashProtocol->Erase (
      mSpiNorFlashProtocol,
      SpiOffset,
      (UINT32)(*NumBytes) / SIZE_4KB
      );
  DEBUG ((EFI_D_INFO , "PspEraseFlash %x %x - %r\n", FlashAddress, *NumBytes, Status));

  return Status;
}

/**
 * Increment the Monotonic counter by 1 inside the Serial Flash Device
 *
 *
 * @param[in]       CounterAddr   CounterAddr for IncMc command
 * @param[in]       CounterData   CounterData for IncMc command
 * @param[in]       Signature     Point to the 32 bytes Signature for IncMc command
 *
 * @retval EFI_SUCCESS      Command success
 * @retval Others           Error happens during handling the command
 */
EFI_STATUS
EFIAPI
PspRpmcIncMc (
  IN UINT8   CounterAddr,
  IN UINT32  CounterData,
  IN UINT8   *Signature
  )
{
  EFI_STATUS  Status;

  Status = EFI_NOT_FOUND;
  return (Status);
}

/**
 * Request the Monotonic counter value inside the Serial Flash Device
 *
 *
 * @param[in]       CounterAddr   CounterAddr for ReqMc command
 * @param[in]       Tag           Tag for ReqMc command
 * @param[in]       Signature     Point to the 32 bytes Signature for ReqMc command
 * @param[in, out]  CounterData   Point to the output CounterData for ReqMc command
  *
 * @retval EFI_SUCCESS      Command success, CounterData is valid
 * @retval Others           Error happens during handling the command
 */
EFI_STATUS
EFIAPI
PspRpmcReqMc (
  IN   UINT8   CounterAddr,
  IN   UINT8   *Tag,
  IN   UINT8   *Signature,
  IN  OUT UINT32  *CounterData,
  IN  OUT UINT8   *OutputSignature
  )
{
  EFI_STATUS  Status;

  Status = EFI_NOT_FOUND;
  return (Status);
}

/**
 * Initial Psp Storage required information
 *
 *
 * @param[in]  SystemTable Point to EFI system table structure
 *
 * @retval EFI_SUCCESS    Initial success
 * @retval Others         Error happens during initialize
 */
EFI_STATUS
EFIAPI
AmdPspFlashAccLibSmmConstructor (
  IN EFI_HANDLE        ImageHandle,
  IN EFI_SYSTEM_TABLE  *SystemTable
  )
{
  EFI_STATUS                     Status;
  EFI_SMM_BASE2_PROTOCOL         *SmmBase2;
  BOOLEAN                        InSmm = FALSE;

  DEBUG ((EFI_D_INFO , "AmdPspFlashAccLibSmmConstructor Entry\n"));

  Status = gBS->LocateProtocol (&gEfiSmmBase2ProtocolGuid, NULL, (VOID**) &SmmBase2);
  if (EFI_ERROR (Status)) {
    DEBUG ((EFI_D_INFO , "   Failed to find SMM base2 protocol - returning immediately\n"));
    return EFI_SUCCESS;
  }
  Status = SmmBase2->InSmm (SmmBase2, &InSmm);
  if (EFI_ERROR (Status)) {
    DEBUG ((EFI_D_INFO , "   Failed to determine if in SMM - returning immediately\n"));
    return EFI_SUCCESS;
  }
  if (!InSmm) {
    DEBUG ((EFI_D_INFO , "   Not in SMM - returning immediately\n"));
    return EFI_SUCCESS;
  }
  DEBUG ((EFI_D_INFO , "   In SMM - continuing constructor\n"));

  // Retrieve SPI NOR flash driver
  Status = gSmst->SmmLocateProtocol( &gEfiSpiSmmNorFlashProtocolGuid,
      NULL,
      (VOID **)&mSpiNorFlashProtocol);
  if (EFI_ERROR(Status)) {
    DEBUG ((EFI_D_INFO ,
          "%a: Unable to locate gEfiSpiSmmNorFlashProtocolGuid\n",
          __FUNCTION__));
    return Status;
  }
  mSpiRomMmioBase = FixedPcdGet32 (PcdFlashAreaBaseAddress);
  TempSetmSpiHackOffset ();

  return EFI_SUCCESS;
}




