/******************************************************************************
 * Copyright (C) 2021-2022 Advanced Micro Devices, Inc. All rights reserved.
 *
 *******************************************************************************
 **/

#include <PiPei.h>
#include <Library/DebugLib.h>
#include <Library/PeiServicesLib.h>
#include <Ppi/IpmiPpi.h>
#include <Library/IpmiBaseLib.h>

EFI_STATUS
EFIAPI
AmdPeiIpmiSubmitCommandPei (
  IN     PEI_IPMI_PPI                      *This,
  IN     UINT8                             NetFunction,
  IN     UINT8                             Command,
  IN     UINT8                             *RequestData,
  IN     UINT32                            RequestDataSize,
     OUT UINT8                             *ResponseData,
  IN OUT UINT32                            *ResponseDataSize
  );

GLOBAL_REMOVE_IF_UNREFERENCED PEI_IPMI_PPI  mPeiIpmiPpi = { AmdPeiIpmiSubmitCommandPei };

GLOBAL_REMOVE_IF_UNREFERENCED EFI_PEI_PPI_DESCRIPTOR mPpiIpmi = {
  (EFI_PEI_PPI_DESCRIPTOR_PPI | EFI_PEI_PPI_DESCRIPTOR_TERMINATE_LIST),
  &gPeiIpmiPpiGuid,
  &mPeiIpmiPpi
};

/**
  Main entry for IPMI PEIM driver.

  This routine registers the IPMI PPI.

  @param  FileHandle  Handle of the file being invoked.
  @param  PeiServices Describes the list of possible PEI Services.

  @retval EFI_SUCCESS       The entry point is executed successfully.
  @retval other             Some error occurs when executing this entry point.

**/
EFI_STATUS
EFIAPI
AmdIpmiPeiEntry (
  IN       EFI_PEI_FILE_HANDLE  FileHandle,
  IN CONST EFI_PEI_SERVICES     **PeiServices
  )
{
  return PeiServicesInstallPpi (&mPpiIpmi);
}

/**
  This service enables submitting commands via Ipmi.

  @param[in]      This                  This point for PEI_IPMI_PPI structure.
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
AmdPeiIpmiSubmitCommandPei (
  IN     PEI_IPMI_PPI                      *This,
  IN     UINT8                             NetFunction,
  IN     UINT8                             Command,
  IN     UINT8                             *RequestData,
  IN     UINT32                            RequestDataSize,
     OUT UINT8                             *ResponseData,
  IN OUT UINT32                            *ResponseDataSize
  )
{
    return IpmiSubmitCommand (
    NetFunction,
    Command,
    RequestData,
    RequestDataSize,
    ResponseData,
    ResponseDataSize
  );
}
