// /*****************************************************************************
//  *
//  * Copyright (C) 2021-2023 Advanced Micro Devices, Inc. All rights reserved.
//  *
//  *****************************************************************************/

#ifndef _PLATFORM_FCH_INIT_H__
#define _PLATFORM_FCH_INIT_H__

#include <FchRegistersCommon.h>
#include <Library/DebugLib.h>
#include <Library/PciSegmentLib.h>
#include <Library/PciLib.h>
#include <Library/BaseLib.h>
#include <IndustryStandard/SmBios.h>
#include <Library/IoLib.h>

/**
  Entry point for Platform FCH initialization.

  @retval  EFI_SUCCESS  returns success code.

**/
EFI_STATUS
EFIAPI
PlatformFchInit (
  IN VOID
  );
#endif    // #ifndef _PLATFORM_FCH_INIT_H__
