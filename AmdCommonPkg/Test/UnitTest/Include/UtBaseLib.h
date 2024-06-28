/* Copyright (C) 2021 - 2024 Advanced Micro Devices, Inc. All rights reserved. */
/**
 * @file  UtBaseLib.h
 * @brief
 *
 */

#pragma once

#include <UtBaseLibTypes.h>

#define AMD_UNIT_TEST_VERSION                       "0.9"
#define AMD_UNIT_TEST_MAX_FILENAME_LENGTH           64
#define AMD_UNIT_TEST_MAX_PATH_LENGTH               256
#define AMD_UNIT_TEST_MAX_STRING_LENGTH             120
#define AMD_UNIT_TEST_MAX_CONFIG_FILE_LENGTH        2096

#ifdef __cplusplus
extern "C" {
#endif

AMD_UNIT_TEST_STATUS
UtInitFromArgs (
  AMD_UNIT_TEST_FRAMEWORK *Ut,
  int  argc,
  char *argv[]
  );

AMD_UNIT_TEST_STATUS
UtGetTestStatus (
  AMD_UNIT_TEST_FRAMEWORK *Ut
  );

const char *
UtGetTestName (
  AMD_UNIT_TEST_FRAMEWORK *Ut
  );

void
UtSetTestStatus (
  AMD_UNIT_TEST_FRAMEWORK *Ut,
  AMD_UNIT_TEST_STATUS    Status
  );

const char *
UtGetTestStatusString (
  AMD_UNIT_TEST_FRAMEWORK *Ut
  );

const char *
UtGetTestIteration (
  AMD_UNIT_TEST_FRAMEWORK *Ut
  );

void
UtSetTestContext (
  AMD_UNIT_TEST_FRAMEWORK *Ut,
  AMD_UNIT_TEST_CONTEXT   Context
 );

int
UtRunTest (
  AMD_UNIT_TEST_FRAMEWORK *Ut
  );

void
UtDeinit (
  AMD_UNIT_TEST_FRAMEWORK *Ut
  );

AMD_UNIT_TEST_FRAMEWORK_HANDLE
UtGetActiveFrameworkHandle (
  void
  );

void*
UtGetTestConfig  (
  AMD_UNIT_TEST_FRAMEWORK *Ut,
  const char* key
  );

void
UtAddElementToResult (
  AMD_UNIT_TEST_FRAMEWORK *Ut,
  const char              *Key,
  const char              *Value
  );

#ifdef __cplusplus
}
#endif
