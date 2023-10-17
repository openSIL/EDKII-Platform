/*****************************************************************************
 *
 * Copyright (C) 2021-2023 Advanced Micro Devices, Inc. All rights reserved.
 *
 *******************************************************************************
 */
#include <Uefi.h>
#include <FchPT.h>
#include <Library/DebugLib.h>
#include "PlatformFchInit.h"
/*********************************************************************************
 * Name: FchPlatformPTPeiInit
 *
 * Description
 *   Platform hook to update PT parameters
 *
 * Input
 *   FchPTData : a pointer to PT data structure
 *
 * Output
 *   none if the reset is successful
 *
 *********************************************************************************/
EFI_STATUS
EFIAPI
FchPlatformPTPeiInit (
  IN      VOID                          *FchPTData
  )
{
//  FCH_PT                *Promontory;

//  Promontory  = (FCH_PT *)FchPTData;

  return EFI_SUCCESS;
}

/*********************************************************************************
 * Name: FchPlatformOemPeiInit
 *
 * Description
 *   Platform Oem hook
 *
 * Input
 *   FchData
 *
 * Output
 *
 *
 *********************************************************************************/
EFI_STATUS
EFIAPI
FchPlatformOemPeiInit (
  IN      VOID                          *FchData
  )
{
  // AMD EDKII OVERRIDE START
  DEBUG ((DEBUG_INFO, "Entered %a Platform/OEM FCH Hook.\n", __FUNCTION__));
  PlatformFchInit ();
  // AMD EDKII OVERRIDE END
  return EFI_SUCCESS;
}
