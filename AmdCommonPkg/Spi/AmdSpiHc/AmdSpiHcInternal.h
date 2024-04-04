/*****************************************************************************
 *
 * Copyright (C) 2018-2022 Advanced Micro Devices, Inc. All rights reserved.
 *
 *****************************************************************************/

#ifndef __AMD_SPI_HC_INTERNAL_H__
#define __AMD_SPI_HC_INTERNAL_H__

#include <Uefi/UefiBaseType.h>
#include "AmdSpiHc.h"
#include "AmdSpiHcInstance.h"
#include <Fch/FchIsaReg.h>

#define FCH_LPC_DMA_SPI_BUSY  BIT0

#define FCH_SPI_MMIO_REG04       0x04        // SPI_RestrictedCmd
#define FCH_SPI_FRAME_SIZE_SUPPORT_MASK (1 << (8 - 1))
#define FCH_SPI_LOCK_CONTROLLER   0x00
#define FCH_SPI_UNLOCK_CONTROLLER   BIT0 | BIT1

/**
  Check that SPI Conroller is Not Busy

  @param[in] Instance   SpiHostControllerInstance;

  @retval EFI_SUCCESS       Spi Execute command executed properly
  @retval EFI_DEVICE_ERROR  Spi Execute command failed
**/
EFI_STATUS
EFIAPI
FchSpiControllerNotBusy (
  SPI_HOST_CONTROLLER_INSTANCE *Instance
  );

/**
  Execute SPI command

  @param[in] Instance   SpiHostControllerInstance;

  @retval EFI_SUCCESS   Spi Execute command executed properly
  @retval others        Spi Execute command failed
**/
EFI_STATUS
EFIAPI
FchSpiExecute (
  SPI_HOST_CONTROLLER_INSTANCE *Instance
  );

/**
  Block SPI Flash Write Enable Opcode.  This will block anything that requires
  the Opcode equivalent to the SPI Flash Memory Write Enable Opcode.

  RestrictedCmd0..3 (SPIx04[31:0]) will be locked (write protected) when
  SPIx00[23:22] not equal 11b, so you can write SPIx00[23:22]=00b to lock them.
  Once SPIx00[23:22] = 00b, they can only be written in SMM,
  to clear RestrictedCmd0..3, get into SMM, write SPIx00[23:22]=11b,
  then you can clear RestrictedCmd0..3 (SPIx04)
**/
EFI_STATUS
EFIAPI
InternalFchSpiBlockOpcode (
  IN CONST EFI_PHYSICAL_ADDRESS  HcAddress,
  IN UINT8 Opcode
  );

/**
  Un-Block SPI Flash Write Enable Opcode.

  RestrictedCmd0..3 (SPIx04[31:0]) will be locked (write protected) when
  SPIx00[23:22] not equal 11b, so you can write SPIx00[23:22]=00b to lock them.
  Once SPIx00[23:22] = 00b, they can only be written in SMM,
  to clear RestrictedCmd0..3, get into SMM, write SPIx00[23:22]=11b,
  then you can clear RestrictedCmd0..3 (SPIx04)
**/
EFI_STATUS
EFIAPI
InternalFchSpiUnblockOpcode (
  IN CONST EFI_PHYSICAL_ADDRESS  HcAddress,
  IN UINT8 Opcode
  );

/**
  Un-Block any blocked SPI Opcodes.

  RestrictedCmd0..3 (SPIx04[31:0]) will be locked (write protected) when
  SPIx00[23:22] not equal 11b, so you can write SPIx00[23:22]=00b to lock them.
  Once SPIx00[23:22] = 00b, they can only be written in SMM,
  to clear RestrictedCmd0..3, get into SMM, write SPIx00[23:22]=11b,
  then you can clear RestrictedCmd0..3 (SPIx04)
**/
EFI_STATUS
EFIAPI
InternalFchSpiUnblockAllOpcodes (
  IN CONST EFI_PHYSICAL_ADDRESS  HcAddress
  );

/**
  Lock SPI host controller registers.

  RestrictedCmd0..3 (SPIx04[31:0]) will be locked (write protected) when
  SPIx00[23:22] not equal 11b, so you can write SPIx00[23:22]=00b to lock them.
  Once SPIx00[23:22] = 00b, they can only be written in SMM,
  to clear RestrictedCmd0..3, get into SMM, write SPIx00[23:22]=11b,
  then you can clear RestrictedCmd0..3 (SPIx04)
**/
EFI_STATUS
EFIAPI
InternalFchSpiLockSpiHostControllerRegisters (
  IN CONST EFI_PHYSICAL_ADDRESS  HcAddress
  );

/**
  Unlock SPI host controller registers.  This unlock function will only work in
  SMM.

  RestrictedCmd0..3 (SPIx04[31:0]) will be locked (write protected) when
  SPIx00[23:22] not equal 11b, so you can write SPIx00[23:22]=00b to lock them.
  Once SPIx00[23:22] = 00b, they can only be written in SMM,
  to clear RestrictedCmd0..3, get into SMM, write SPIx00[23:22]=11b,
  then you can clear RestrictedCmd0..3 (SPIx04)
**/
EFI_STATUS
EFIAPI
InternalFchSpiUnlockSpiHostControllerRegisters (
  IN CONST EFI_PHYSICAL_ADDRESS  HcAddress
  );

#endif // __AMD_SPI_HC_INTERNAL_H__
