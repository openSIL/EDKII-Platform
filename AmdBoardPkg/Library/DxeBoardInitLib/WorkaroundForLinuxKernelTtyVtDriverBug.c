/******************************************************************************
 * Copyright (C) 2020-2022 Advanced Micro Devices, Inc. All rights reserved.
 *
 *
 ***************************************************************************/

#include "WorkaroundForLinuxKernelTtyVtDriverBug.h"

/**
  This function applies a workaround for a Linux kernel bug.

  Linux kernel versions older than v5.6-RC1 contain a bug in the TTY/VT driver
  (drivers/tty/vt/vt.c) that results in a hang (null pointer dereference) when
  Legacy VGA memory is not allocated.

  As a workaround, this function allocates 128 KB of Memory Mapped IO to the
  default Legacy EGA/VGA memory address (0xA0000).

  Kernel bug fix commit details for the TTY/VT driver:
    https://github.com/torvalds/linux/commit/805ece2a58085c33c0c087be049b77e94c12080a
    "vt: Initialize conswitchp to dummy_con if unset

    If the arch setup code hasn't initialized conswitchp yet, set it to
    dummy_con in con_init. This will allow us to drop the dummy_con
    initialization that's done in almost every architecture.

    Signed-off-by: Arvind Sankar <nivedita@alum.mit.edu>
    Link: https://lore.kernel.org/r/20191218214506.49252-3-nivedita@alum.mit.edu
    Signed-off-by: Greg Kroah-Hartman <gregkh@linuxfoundation.org>"

  @retval  EFI_SUCCESS  MMIO at Legacy VGA region has been allocated.
  @retval  !EFI_SUCCESS Error allocating the legacy VGA region.

**/
EFI_STATUS
EFIAPI
WorkaroundForLinuxKernelTtyVtDriverBug (
  VOID
)
{
  EFI_STATUS           Status;
  EFI_PHYSICAL_ADDRESS VgaMemAddress;

  VgaMemAddress = (EFI_PHYSICAL_ADDRESS) VGA_MEM_BASE;
  Status = gBS->AllocatePages (AllocateAddress,
                               EfiMemoryMappedIO,
                               EFI_SIZE_TO_PAGES (VGA_MEM_SIZE),
                               &VgaMemAddress
                               );
  return Status;
}