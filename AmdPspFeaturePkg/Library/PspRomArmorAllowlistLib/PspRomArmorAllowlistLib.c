/*****************************************************************************
 *
 * Copyright (C) 2008-2023 Advanced Micro Devices, Inc. All rights reserved.
 *
 *******************************************************************************
 */
/**
 * @file
 *
 * Platform ROM Armor Allowlist table
 *
 */

#include <Library/DebugLib.h>
#include <Library/AmdPspRomArmorLib.h>
#include <Library/PlatformPspRomArmorAllowlistLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/PcdLib.h>

SPI_ALLOW_LIST gRomArmorAllowlist = {
  9,    // AllowedCmdCount
  1,    // AllowedRegionCount
  {
    // SPI part will be in 4-Byte mode for 3-Byte commands
    {0x01,0x01,0x9F,0x0,0x0,0x03,0x03,0x0,0x00000000},   // CS=1, Freq=1, Opcode=ReadID, Min/MaxTX, MinMaxRX, No Addr Check (Example of limiting number of bytes read)
    {0x01,0x01,0x05,0x0,0x0,0x01,0x03,0x0,0x00000000},   // CS=1, Freq=1, Opcode=Read Status, Min/MaxTX, Min/MaxRX, No Addr Check
    {0x01,0x01,0x03,0x04,0x04,0x01,0x44,0x0,0x00000000}, // CS=1, Freq=1, Opcode=Read, Min/MaxTX, Min/MaxRX, No Addr Check
    {0x01,0x01,0x0B,0x05,0x05,0x01,0x43,0x0,0x00000000}, // CS=1, Freq=1, Opcode=Fast Read, Min/MaxTX adds 2 dummy bytes, Min/MaxRX, No Addr Check
    {0x01,0x01,0x06,0x0,0x0,0x0,0x0,0x0,0x00000000},     // CS=1, Freq=1, Opcode=Write Enable, No TX/RX, No Addr Check
    {0x01,0x01,0x02,0x05,0x48,0x0,0x0,0x2,0x00000100},   // CS=1, Freq=1, Opcode=page program, Min/MaxTX, Min/MaxRX, AddrCheck=2, ImpactSize=256
    {0x01,0x01,0x20,0x04,0x04,0x0,0x0,0x2,0x00001000},   // CS=1, Freq=1, Opcode=Sector Erase, Min/MaxTX, Min/MaxRX, AddrCheck=2, ImpactSize=4096
    {0x01,0x01,0x52,0x04,0x04,0x0,0x0,0x2,0x00008000},   // CS=1, Freq=1, Opcode=Sector Erase, Min/MaxTX, Min/MaxRX, AddrCheck=2, ImpactSize=32KB
    {0x01,0x01,0xD8,0x04,0x04,0x0,0x0,0x2,0x00010000},   // CS=1, Freq=1, Opcode=Sector Erase, Min/MaxTX, Min/MaxRX, AddrCheck=2, ImpactSize=64KB
    {0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x00000000},        // empty
    {0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x00000000},        // empty
    {0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x00000000},        // empty
    {0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x00000000},        // empty
    {0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x00000000},        // empty
    {0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x00000000},        // empty
    {0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x00000000},        // empty
    {0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x00000000},        // empty
    {0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x00000000},        // empty
    {0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x00000000},        // empty
    {0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x00000000},        // empty
    {0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x00000000},        // empty
    {0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x00000000},        // empty
    {0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x00000000},        // empty
    {0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x00000000},        // empty
    {0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x00000000},        // empty
    {0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x00000000},        // empty
    {0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x00000000},        // empty
    {0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x00000000},        // empty
    {0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x00000000},        // empty
    {0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x00000000},        // empty
    {0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x00000000},        // empty
    {0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x0,0x00000000}         // empty
  },
  {
    {0x00000000, 0x01FFFFFF},                     // Start/End Address
    {0x00000000, 0x00000000},                     // empty
    {0x00000000, 0x00000000},                     // empty
    {0x00000000, 0x00000000},                     // empty
    {0x00000000, 0x00000000},                     // empty
    {0x00000000, 0x00000000},                     // empty
    {0x00000000, 0x00000000},                     // empty
    {0x00000000, 0x00000000},                     // empty
    {0x00000000, 0x00000000},                     // empty
    {0x00000000, 0x00000000},                     // empty
    {0x00000000, 0x00000000},                     // empty
    {0x00000000, 0x00000000},                     // empty
    {0x00000000, 0x00000000},                     // empty
    {0x00000000, 0x00000000},                     // empty
    {0x00000000, 0x00000000},                     // empty
    {0x00000000, 0x00000000}                      // empty
  }
};

/*----------------------------------------------------------------------------------------*/
/*
 *  Return allocated and filled AMD PSP ROM Armor Allow list Table
 *
 *
 * @param[in]  PlatformSpiAllowlist   Pointer to allow list table
 *
 * @return    EFI_SUCCESS
 * @return    EFI_OUT_OF_RESOURCES      Buffer to return couldn't be allocated
 */
EFI_STATUS
EFIAPI
GetPspRomArmorAllowlist (
  IN       SPI_ALLOW_LIST     **PlatformSpiAllowlist
  )
{
  SPI_ALLOW_LIST    *SpiAllowlist;

  SpiAllowlist = AllocateZeroPool (sizeof (SPI_ALLOW_LIST));
  *PlatformSpiAllowlist = SpiAllowlist;
  if (SpiAllowlist == NULL) {
    return EFI_OUT_OF_RESOURCES;
  }
  CopyMem (SpiAllowlist, &gRomArmorAllowlist, sizeof (SPI_ALLOW_LIST));

  return EFI_SUCCESS;
}
