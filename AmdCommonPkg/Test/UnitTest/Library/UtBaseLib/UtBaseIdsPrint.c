/* Copyright (C) 2021 - 2024 Advanced Micro Devices, Inc. All rights reserved. */
/**
 * @file  UtBaseSilPrint.c
 * @brief
 *
 */

#include <stdio.h>
#include <UtBaseLib.h>
#include <string.h>
#include "Log.h"
#include <UtLogLib.h>

#include <Uefi.h>
#include <Library/IdsLib.h>

void
IdsLibDebugPrint (
  UINT64      Flag,
  const CHAR8 *Format,
  ...
  )
{
  VA_LIST variadicArgs;
  VA_START(variadicArgs, Format);
  log_log_sil (AMD_UNIT_TEST_LOG_INFO, "", 0, Format, variadicArgs);
  VA_END(variadicArgs);
}

void
IdsAgesaTestPoint (
  AGESA_POST_CODE   TestPoint,
  AMD_CONFIG_PARAMS *StdHeader
  )
{
  return;
}

void
IdsAssert (
  UINT32 FileCode
  )
{
  return;
}
