/******************************************************************************
 * Copyright (C) 2021-2022 Advanced Micro Devices, Inc. All rights reserved.
 *
 *******************************************************************************
 **/

#include <Base.h>
#include <Protocol/IpmiProtocol.h>
#include <Library/DebugLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Uefi/UefiBaseType.h>
#include <Uefi/UefiSpec.h>

#include <IndustryStandard/Ipmi.h>
#include <Library/IpmiBaseLib.h>

IPMI_PROTOCOL *mIpmiProtocol = NULL;

/**
  This service enables submitting commands via Ipmi.

  @param[in]      This                  This point for IPMI_PROTOCOL structure.
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
AmdIpmiSubmitCommandDxe (
  IN     IPMI_PROTOCOL  *This,
  IN     UINT8          NetFunction,
  IN     UINT8          Command,
  IN     UINT8          *RequestData,
  IN     UINT32         RequestDataSize,
     OUT UINT8          *ResponseData,
  IN OUT UINT32         *ResponseDataSize
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

/**
  Standard EFI driver point.  This driver installs IPMI_PROTOCOL
  if not already installed.

  @param   ImageHandle          Handle for the image of this driver
  @param   SystemTable          Pointer to the EFI System Table

  @retval  EFI_SUCCESS          IPMI PROTOCOL successfully installed.
           EFI_ALREADY_STARTED  If IPMI_PROTOCOL is already installed.
           Others               Failed to install the protocol.

**/

EFI_STATUS
EFIAPI
AmdIpmiDxeInit (
  IN EFI_HANDLE         ImageHandle,
  IN EFI_SYSTEM_TABLE   *SystemTable
  )
{
  EFI_STATUS  Status;
  IPMI_PROTOCOL *IpmiProtocol;

  Status = gBS->LocateProtocol (
            &gIpmiProtocolGuid,
            NULL,
            (VOID **) &IpmiProtocol
            );
  if (Status == EFI_SUCCESS) {
    DEBUG ((
      DEBUG_ERROR,
      "%a: IPMI PROTOCOL already Installed.\n",
      __FUNCTION__
      ));
    return EFI_ALREADY_STARTED;
  }
  Status = gBS->AllocatePool (
            EfiBootServicesData,
            sizeof (IPMI_PROTOCOL),
            (VOID **)&mIpmiProtocol
            );
  if (EFI_ERROR (Status)) {
    return Status;
  }
  mIpmiProtocol->IpmiSubmitCommand = AmdIpmiSubmitCommandDxe;
  // Install Protocol
  Status = gBS->InstallMultipleProtocolInterfaces (
            &ImageHandle,
            &gIpmiProtocolGuid,
            mIpmiProtocol,
            NULL
            );
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "%a: Cannot install IPMI PROTOCOL - %r\n", Status));
    gBS->FreePool (mIpmiProtocol);
    mIpmiProtocol = NULL;
  }
  return Status;
}

/**
  Unload handle for AMD IPMI DXE module.

  Disconnect the driver specified by ImageHandle from all the devices
  in the handle database.
  Uninstall all the protocols installed in the driver entry point.

  @param[in] ImageHandle           The drivers' driver image.

  @retval    EFI_SUCCESS           The image is unloaded.
  @retval    Others                Failed to unload the image.

**/
EFI_STATUS
EFIAPI
AmdIpmiDxeUnload (
  IN EFI_HANDLE   ImageHandle
  )
{
  EFI_STATUS Status;
  Status = gBS->UninstallMultipleProtocolInterfaces (
                  ImageHandle,
                  &gIpmiProtocolGuid,
                  &mIpmiProtocol,
                  NULL
                  );
  if (EFI_ERROR (Status)) {
    DEBUG ((
      DEBUG_ERROR,
      "%a: Protocol uninstall error. Status = %r.\n",
      __FUNCTION__,
      Status
      ));
    return (Status);
  }
  if (mIpmiProtocol != NULL) {
    gBS->FreePool (mIpmiProtocol);
    mIpmiProtocol = NULL;
  }
  return (Status);
}
