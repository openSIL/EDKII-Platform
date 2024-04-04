/*****************************************************************************
 *
 * Copyright (C) 2008-2024 Advanced Micro Devices, Inc. All rights reserved.
 *
 *******************************************************************************
 */
/**
 * @file
 *
 * Platform ROM Armor AllowList table
 *
 */

#include <Library/AmdPspRomArmorLib.h>
/*
 *  Return allocated and filled AMD PSP ROM Armor allow list Table
 *
 *
 * @param[in]  PlatformSpiAllowList   Pointer to allow list table
 *
 * @return    EFI_SUCCESS
 * @return    EFI_OUT_OF_RESOURCES      Buffer to return couldn't be allocated
 */
EFI_STATUS
EFIAPI
GetPspRomArmorAllowList (
  IN       SPI_ALLOW_LIST     **PlatformSpiAllowList
  );
