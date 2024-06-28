/* Copyright (C) 2021 - 2024 Advanced Micro Devices, Inc. All rights reserved. */
/**
 * @file  UtBaseSilPrint.c
 * @brief
 *
 */

#include <stdio.h>
#include <UtBaseLib.h>
#include <string.h>
#include <malloc.h>
#include "Log.h"
#include <UtLogLib.h>

void
SilTracePoint (
    size_t      MsgLevel,
    const char  *MsgPrefix,
    const char  *Message,
    const char  *Function,
    size_t      Line,
    ...)
{
  AMD_UNIT_TEST_LOG  LogType;
  VA_LIST            variadicArgs;
  char*              EntryString = "Entry ";
  char*              ExitString  = "Exit ";
  size_t MessageLen = strlen(Message) + strlen(MsgPrefix) + strlen(EntryString) + 1;
  char *MessageWithPrefix = malloc(MessageLen);
  strcpy_s(MessageWithPrefix, MessageLen, MsgPrefix);
  switch (MsgLevel)
  {
  case 0x01:
    // SIL_TRACE_ERROR
    LogType = AMD_UNIT_TEST_LOG_ERROR;
    break;
  case 0x02:
    // SIL_TRACE_WARNING
    LogType = AMD_UNIT_TEST_LOG_WARN;
    break;
  case 0x04:
    // SIL_TRACE_ENTRY
    LogType = AMD_UNIT_TEST_LOG_INFO;
    strcpy_s(MessageWithPrefix, MessageLen, EntryString);
    break;
  case 0x08:
    // SIL_TRACE_EXIT
    LogType = AMD_UNIT_TEST_LOG_INFO;
    strcpy_s(MessageWithPrefix, MessageLen, ExitString);
    break;
  case 0x10:
    // SIL_TRACE_INFO
    LogType = AMD_UNIT_TEST_LOG_INFO;
    break;
  default:
    LogType = AMD_UNIT_TEST_LOG_INFO;
    break;
  }
  strcat_s(MessageWithPrefix, MessageLen, Message);
  MessageWithPrefix[strcspn(MessageWithPrefix, "\n")] = 0;
  VA_START(variadicArgs, Line);
  log_log_sil (LogType, Function, Line, MessageWithPrefix, variadicArgs);
  free (MessageWithPrefix);
  VA_END(variadicArgs);
}