/*****************************************************************************
 *
 * Copyright (C) 2021-2024 Advanced Micro Devices, Inc. All rights reserved.
 *******************************************************************************
 **/

/**
 * @file AmdHiiConfigRouting.h
 *
 * @brief AMD HiiBlockToConfig, HiiConfigToBlock definition
 *
 * @details Provide optimized implementation of HII_CONFIG_ROUTING Protocol
 * functions HiiBlockToConfig and HiiConfigToBlock.
 *
 */

#ifndef _AMD_HII_CONFIG_ROUTING_H_
#define _AMD_HII_CONFIG_ROUTING_H_

#include <Library/DebugLib.h>
#include <Library/BaseMemoryLib.h>
#include <Protocol/HiiConfigRouting.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/UefiDriverEntryPoint.h>
#include <Library/UefiBootServicesTableLib.h>

#define MAX_STRING_LENGTH 1024

/**
  This helper function is to be called by drivers to map configuration data
  stored in byte array ("block") formats such as UEFI Variables into current
  configuration strings.

  @param  This                    A pointer to the EFI_HII_CONFIG_ROUTING_PROTOCOL
                                  instance.
  @param  ConfigRequest           A null-terminated Unicode string in
                                  <ConfigRequest> format.
  @param  Block                   Array of bytes defining the block's
                                  configuration.
  @param  BlockSize               Length in bytes of Block.
  @param  Config                  Filled-in configuration string. String allocated
                                  by  the function. Returned only if call is
                                  successful.
  @param  Progress                A pointer to a string filled in with the offset
                                  of  the most recent & before the first failing
                                  name/value pair (or the beginning of the string
                                  if the failure is in the first name / value pair)
                                  or the terminating NULL if all was successful.

  @retval EFI_SUCCESS             The request succeeded. Progress points to the
                                  null terminator at the end of the ConfigRequest
                                  string.
  @retval EFI_OUT_OF_RESOURCES    Not enough memory to allocate Config.
                                  Progress points to the first character of
                                  ConfigRequest.
  @retval EFI_INVALID_PARAMETER   Passing in a NULL for the ConfigRequest or
                                  Block parameter would result in this type of
                                  error. Progress points to the first character
				  of ConfigRequest.
  @retval EFI_NOT_FOUND           Target for the specified routing data was not
                                  found. Progress points to the "G" in "GUID" of
                                  the errant routing data.
  @retval EFI_DEVICE_ERROR        Block not large enough. Progress undefined.
  @retval EFI_INVALID_PARAMETER   Encountered non <BlockName> formatted string.
                                  Block is left updated and Progress points at
                                  the '&' preceding the first non-<BlockName>.

**/
EFI_STATUS
EFIAPI
HiiBlockToConfig (
  IN  CONST EFI_HII_CONFIG_ROUTING_PROTOCOL  *This,
  IN  CONST EFI_STRING                       ConfigRequest,
  IN  CONST UINT8                            *Block,
  IN  CONST UINTN                            BlockSize,
  OUT EFI_STRING                             *Config,
  OUT EFI_STRING                             *Progress
  );

/**
  This helper function is to be called by drivers to map configuration strings
  to configurations stored in byte array ("block") formats such as UEFI Variables.

  @param  This                    A pointer to the EFI_HII_CONFIG_ROUTING_PROTOCOL
                                  instance.
  @param  ConfigResp              A null-terminated Unicode string in <ConfigResp>
                                  format.
  @param  Block                   A possibly null array of bytes representing the
                                  current block. Only bytes referenced in the
                                  ConfigResp string in the block are modified. If
                                  this parameter is null or if the *BlockSize
                                  parameter is (on input) shorter than required by
                                  the Configuration string, only the BlockSize
                                  parameter is updated and an appropriate status
                                  (see below) is returned.
  @param  BlockSize               The length of the Block in units of UINT8. On
                                  input, this is the size of the Block. On output,
                                  if successful, contains the largest index of
				  the modified byte in the Block, or the required
				  buffer.
                                  size if the Block is not large enough.
  @param  Progress                On return, points to an element of the ConfigResp
                                  string filled in with the offset of the most
                                  recent '&' before the first failing name/value
                                  pair (or the beginning of the string if the
                                  failure is in the first name/value pair) or
                                  the terminating NULL if all was successful.

  @retval EFI_SUCCESS             The request succeeded. Progress points to the
                                  null terminator at the end of the ConfigResp
                                  string.
  @retval EFI_OUT_OF_RESOURCES    Not enough memory to allocate Config. Progress
                                  points to the first character of ConfigResp.
  @retval EFI_INVALID_PARAMETER   Passing in a NULL for the ConfigResp or Block
                                  parameter would result in this type of error.
				  Progress points to the first character of
                                  ConfigResp.
  @retval EFI_NOT_FOUND           Target for the specified routing data was not
                                  found. Progress points to the "G" in "GUID" of
                                  the errant routing data.
  @retval EFI_INVALID_PARAMETER   Encountered non <BlockName> formatted name/
                                  value pair. Block is left updated and Progress
				  points at the '&' preceding the first
                                  non-<BlockName>.
  @retval EFI_BUFFER_TOO_SMALL    Block not large enough. Progress undefined.
                                  BlockSize is updated with the required buffer
				  size.

**/
EFI_STATUS
EFIAPI
HiiConfigToBlock (
  IN     CONST EFI_HII_CONFIG_ROUTING_PROTOCOL *This,
  IN     CONST EFI_STRING                      ConfigResp,
  IN OUT UINT8                                 *Block,
  IN OUT UINTN                                 *BlockSize,
  OUT    EFI_STRING                            *Progress
  );

#endif //_AMD_HII_CONFIG_ROUTING_H_
