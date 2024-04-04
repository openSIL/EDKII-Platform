// /*****************************************************************************
//  *
//  * Copyright (C) 2020-2024 Advanced Micro Devices, Inc. All rights reserved.
//  *
//  *****************************************************************************/

/* This file includes code originally published under the following license. */

/** @file
  This contains the installation function for the driver.

Copyright (c) 2005 - 2012, Intel Corporation. All rights reserved.<BR>
This program and the accompanying materials
are licensed and made available under the terms and conditions of the BSD License
which accompanies this distribution.  The full text of the license may be found at
http://opensource.org/licenses/bsd-license.php

THE PROGRAM IS DISTRIBUTED UNDER THE BSD LICENSE ON AN "AS IS" BASIS,
WITHOUT WARRANTIES OR REPRESENTATIONS OF ANY KIND, EITHER EXPRESS OR IMPLIED.

**/

#include "LegacyPic.h"

UINT8                     mPrimaryBase             = 0xff;
UINT8                     mSecondaryBase              = 0xff;
UINT16                    mProtectedModeMask      = 0xffff;
UINT16                    mProtectedModeEdgeLevel = 0x0000;

/**
  Write to mask and edge/level triggered registers of primary and secondary PICs.

  @param[in]  Mask       low byte for primary PIC mask register,
                         high byte for secondary PIC mask register.
  @param[in]  EdgeLevel  low byte for primary PIC edge/level triggered register,
                         high byte for secondary PIC edge/level triggered register.

**/
VOID
Interrupt8259WriteMask (
  IN UINT16  Mask,
  IN UINT16  EdgeLevel
  )
{
  IoWrite8 (LEGACY_8259_MASK_REGISTER_PRIMARY, (UINT8) Mask);
  IoWrite8 (LEGACY_8259_MASK_REGISTER_SECONDARY, (UINT8) (Mask >> 8));
  IoWrite8 (LEGACY_8259_EDGE_LEVEL_TRIGGERED_REGISTER_PRIMARY, (UINT8) EdgeLevel);
  IoWrite8 (LEGACY_8259_EDGE_LEVEL_TRIGGERED_REGISTER_SECONDARY, (UINT8) (EdgeLevel >> 8));
}

/**
  Sets the base address for the 8259 primary and secondary PICs.

  @param[in]  This        Indicates the EFI_LEGACY_8259_PROTOCOL instance.
  @param[in]  PrimaryBase  Interrupt vectors for IRQ0-IRQ7.
  @param[in]  SecondaryBase   Interrupt vectors for IRQ8-IRQ15.

  @retval  EFI_SUCCESS       The 8259 PIC was programmed successfully.
  @retval  EFI_DEVICE_ERROR  There was an error while writing to the 8259 PIC.

**/
EFI_STATUS
EFIAPI
Interrupt8259SetVectorBase (
  IN UINT8                     PrimaryBase,
  IN UINT8                     SecondaryBase
  )
{
  UINT8   Mask;
  EFI_TPL OriginalTpl;

  OriginalTpl = gBS->RaiseTPL (TPL_HIGH_LEVEL);
  //
  // Set vector base for secondary PIC
  //
  if (SecondaryBase != mSecondaryBase) {
    mSecondaryBase = SecondaryBase;

    //
    // Initialization sequence is needed for setting vector base.
    //

    //
    // Preserve interrtup mask register before initialization sequence
    // because it will be cleared during initialization
    //
    Mask = IoRead8 (LEGACY_8259_MASK_REGISTER_SECONDARY);

    //
    // ICW1: cascade mode, ICW4 write required
    //
    IoWrite8 (LEGACY_8259_CONTROL_REGISTER_SECONDARY, 0x11);

    //
    // ICW2: new vector base (must be multiple of 8)
    //
    IoWrite8 (LEGACY_8259_MASK_REGISTER_SECONDARY, mSecondaryBase);

    //
    // ICW3: secondary indentification code must be 2
    //
    IoWrite8 (LEGACY_8259_MASK_REGISTER_SECONDARY, 0x02);

    //
    // ICW4: fully nested mode, non-buffered mode, normal EOI, IA processor
    //
    IoWrite8 (LEGACY_8259_MASK_REGISTER_SECONDARY, 0x01);

    //
    // Restore interrupt mask register
    //
    IoWrite8 (LEGACY_8259_MASK_REGISTER_SECONDARY, Mask);
  }

  //
  // Set vector base for primary PIC
  //
  if (PrimaryBase != mPrimaryBase) {
    mPrimaryBase = PrimaryBase;

    //
    // Initialization sequence is needed for setting vector base.
    //

    //
    // Preserve interrtup mask register before initialization sequence
    // because it will be cleared during initialization
    //
    Mask = IoRead8 (LEGACY_8259_MASK_REGISTER_PRIMARY);

    //
    // ICW1: cascade mode, ICW4 write required
    //
    IoWrite8 (LEGACY_8259_CONTROL_REGISTER_PRIMARY, 0x11);

    //
    // ICW2: new vector base (must be multiple of 8)
    //
    IoWrite8 (LEGACY_8259_MASK_REGISTER_PRIMARY, mPrimaryBase);

    //
    // ICW3: secondary PIC is cascaded on IRQ2
    //
    IoWrite8 (LEGACY_8259_MASK_REGISTER_PRIMARY, 0x04);

    //
    // ICW4: fully nested mode, non-buffered mode, normal EOI, IA processor
    //
    IoWrite8 (LEGACY_8259_MASK_REGISTER_PRIMARY, 0x01);

    //
    // Restore interrupt mask register
    //
    IoWrite8 (LEGACY_8259_MASK_REGISTER_PRIMARY, Mask);
  }

  IoWrite8 (LEGACY_8259_CONTROL_REGISTER_PRIMARY, LEGACY_8259_EOI);
  IoWrite8 (LEGACY_8259_CONTROL_REGISTER_SECONDARY, LEGACY_8259_EOI);

  gBS->RestoreTPL (OriginalTpl);

  return EFI_SUCCESS;
}

/**
  Issues the End of Interrupt (EOI) commands to PICs.

  @param[in]  This  Indicates the EFI_LEGACY_8259_PROTOCOL instance.
  @param[in]  Irq   The interrupt for which to issue the EOI command.

  @retval  EFI_SUCCESS            The EOI command was issued.
  @retval  EFI_INVALID_PARAMETER  The Irq is not valid.

**/
EFI_STATUS
EFIAPI
Interrupt8259EndOfInterrupt (
  IN EFI_8259_IRQ              Irq
  )
{
  if ((UINT32)Irq > Efi8259Irq15) {
    return EFI_INVALID_PARAMETER;
  }

  if (Irq >= Efi8259Irq8) {
    IoWrite8 (LEGACY_8259_CONTROL_REGISTER_SECONDARY, LEGACY_8259_EOI);
  }

  IoWrite8 (LEGACY_8259_CONTROL_REGISTER_PRIMARY, LEGACY_8259_EOI);

  return EFI_SUCCESS;
}

/**
  Entry point for Legacy PIC initialization.

  @param[in]  ImageHandle  ImageHandle of the loaded driver.
  @param[in]  SystemTable  Pointer to the EFI System Table.

  @retval  EFI_SUCCESS  One or more of the drivers returned a success code.
  @retval  !EFI_SUCCESS  Error initializing the Legacy PIC.

**/
EFI_STATUS
EFIAPI
LegacyPicInit (
  IN EFI_HANDLE        ImageHandle,
  IN EFI_SYSTEM_TABLE  *SystemTable
  )
{
  EFI_STATUS   Status = EFI_SUCCESS;
  EFI_8259_IRQ Irq;

  //
  // Clear all pending interrupt
  //
  for (Irq = Efi8259Irq0; Irq <= Efi8259Irq15; Irq++) {
    Interrupt8259EndOfInterrupt (Irq);
  }

  //
  // Set the 8259 Primary base to 0x68 and the 8259 Secondary base to 0x70
  //
  Status = Interrupt8259SetVectorBase (PROTECTED_MODE_BASE_VECTOR_PRIMARY, PROTECTED_MODE_BASE_VECTOR_SECONDARY);

  //
  // Set all 8259 interrupts to edge triggered and disabled
  //
  Interrupt8259WriteMask (mProtectedModeMask, mProtectedModeEdgeLevel);


  // Do things to initialize the PIC

  return Status;
}
