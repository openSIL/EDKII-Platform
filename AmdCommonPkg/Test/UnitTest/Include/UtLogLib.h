/* Copyright (C) 2021 - 2024 Advanced Micro Devices, Inc. All rights reserved. */
/**
 * @file  UtLogLib.h
 * @brief
 *
 */

#pragma once

#include <stdio.h>
#include <string.h>

typedef enum {
  AMD_UNIT_TEST_LOG_TRACE,
  AMD_UNIT_TEST_LOG_DEBUG,
  AMD_UNIT_TEST_LOG_INFO,
  AMD_UNIT_TEST_LOG_WARN,
  AMD_UNIT_TEST_LOG_ERROR,
  AMD_UNIT_TEST_LOG_FATAL
} AMD_UNIT_TEST_LOG;
