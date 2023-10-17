/*****************************************************************************
 * Copyright (C) 2022 - 2023 Advanced Micro Devices, Inc. All rights reserved.
 *
*****************************************************************************
*/

#include <Library/BaseMemoryLib.h>
#include <Uefi/UefiBaseType.h>
#include <FchEspiGenoaLib.h>


/**
 * ClearEspiIOForlegacyIO - clear espi IO range if it is equal to legacy Uart IO range.
 *
 * @param[in]  VOID
 */
VOID
ClearEspiIOForlegacyIO (
  VOID
  )
{
  UINT16  LegacyUartIO[5];

  LegacyUartIO[0] = 0x3F8;
  LegacyUartIO[1] = 0;
  LegacyUartIO[2] = 0;
  LegacyUartIO[3] = 0;
  LegacyUartIO[4] = 0;
  // clear espi IO decode which is conflicted with legacy uart IO Range
  ClsEspiIOForlegacyIO (LegacyUartIO);

}
