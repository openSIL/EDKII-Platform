/* Copyright (C) 2021 - 2024 Advanced Micro Devices, Inc. All rights reserved. */
/**
 * @file  UtBaseLibTypes.h
 * @brief
 *
 */

#pragma once

#include <Uefi.h>
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <cJSON.h>

#define AMD_UNIT_TEST_FRAMEWORK_HANDLE         void*
#define AMD_UNIT_TEST_CONTEXT                  void*
#define UNUSED(X)                              ((void)(X))

typedef enum {
  AMD_UNIT_TEST_PASSED,
  AMD_UNIT_TEST_FAILED,
  AMD_UNIT_TEST_ABORTED,
  AMD_UNIT_TEST_STATUS_NOT_SET
} AMD_UNIT_TEST_STATUS;

typedef
AMD_UNIT_TEST_STATUS
(EFIAPI *AMD_UNIT_TEST_PREREQUISITE)(
  IN AMD_UNIT_TEST_CONTEXT  Context
  );

typedef
void
(EFIAPI *AMD_UNIT_TEST_FUNCTION)(
  IN AMD_UNIT_TEST_CONTEXT  Context
  );

typedef
AMD_UNIT_TEST_STATUS
(EFIAPI *AMD_UNIT_TEST_CLEANUP)(
  IN AMD_UNIT_TEST_CONTEXT  Context
  );

typedef struct {
  AMD_UNIT_TEST_CLEANUP      CleanUpFunc;
  AMD_UNIT_TEST_FUNCTION     TestFunc;
  AMD_UNIT_TEST_PREREQUISITE PrereqFunc;
  AMD_UNIT_TEST_CONTEXT      Context;
} AMD_UNIT_TEST_WRAPPER;

typedef void (*AMD_UNIT_TEST_LOGGER) (
  int         MsgLevel,
  const char  *file,
  int         line,
  const char  *fmt,
  ...
);

typedef struct {
  char                       *TestName;
  char                       *TestIteration;
  char                       *TestConfigFile;
  char                       *TestOutpath;
  void*                      TestContext;
  FILE                       *LogFile;
  FILE                       *ResultFile;
  FILE                       *ConfigFile;
  cJSON                      *TestConfigRoot;
  cJSON                      *TestConfigIteration;
  cJSON                      *TestResultRoot;
  AMD_UNIT_TEST_STATUS       TestStatus;
  AMD_UNIT_TEST_LOGGER       Log;
} AMD_UNIT_TEST_FRAMEWORK;
