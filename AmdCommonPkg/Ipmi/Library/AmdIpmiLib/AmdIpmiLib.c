/******************************************************************************
 * Copyright (C) 2021-2024 Advanced Micro Devices, Inc. All rights reserved.
 *
 *******************************************************************************
 **/

#include <Base.h>
#include <Uefi/UefiBaseType.h>
#include <Uefi/UefiSpec.h>
#include <Library/IpmiLib.h>

#include "AmdIpmiKCS.h"

/**
  This service enables submitting commands via Ipmi.

  @param[in]      NetFunction           Net function of the command.
  @param[in]      Command               IPMI Command.
  @param[in]      RequestData           Command Request Data.
  @param[in]      RequestDataSize       Size of Command Request Data.
  @param[out]     ResponseData          Command Response Data. The completion
                                        code is the first byte of response
                                        data.
  @param[in, out] ResponseDataSize      Size of Command Response Data.

  @retval         EFI_SUCCESS           The command byte stream was
                                        successfully submit to the device
                                        and a response was successfully
                                        received.
  @retval         EFI_NOT_FOUND         The command was not successfully sent
                                        to the device or a response was not
                                        successfully received from the device.
  @retval         EFI_NOT_READY         Ipmi Device is not ready for Ipmi
                                        command access.
  @retval         EFI_DEVICE_ERROR      Ipmi Device hardware error.
  @retval         EFI_TIMEOUT           The command time out.
  @retval         EFI_UNSUPPORTED       The command was not successfully sent
                                        to the device.
  @retval         EFI_OUT_OF_RESOURCES  The resource allcation is out of
                                        resource or data size error.
**/

EFI_STATUS
EFIAPI
IpmiSubmitCommand (
  IN     UINT8          NetFunction,
  IN     UINT8          Command,
  IN     UINT8          *RequestData,
  IN     UINT32         RequestDataSize,
     OUT UINT8          *ResponseData,
  IN OUT UINT32         *ResponseDataSize
  )
{
  if (ResponseData == NULL) {
    return EFI_INVALID_PARAMETER;
  }
  if (ResponseDataSize == NULL) {
    return EFI_INVALID_PARAMETER;
  }
  switch (PcdGet8 (PcdIpmiInterfaceType)) {
    case IPMIDeviceInfoInterfaceTypeKCS:
      return AmdIpmiSubmitCommandKCS (
        NetFunction,
        Command,
        RequestData,
        RequestDataSize,
        ResponseData,
        ResponseDataSize
      );
      break;
    default:
      return EFI_UNSUPPORTED;
      break;
  }
}

/**
  Initialize the global varible.

  @retval EFI_SUCCESS - Always return success

**/
EFI_STATUS
EFIAPI
InitializeIpmiBase (
  VOID
  )
{
  return EFI_SUCCESS;
}
