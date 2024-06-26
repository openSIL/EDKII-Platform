/*****************************************************************************
 *
 * Copyright (C) 2019-2024 Advanced Micro Devices, Inc. All rights reserved.
 *
 *****************************************************************************/

/* This file includes code originally published under the following license. */

/** @file
  Min DSDT Driver

Copyright (c) 2018, Intel Corporation. All rights reserved.<BR>
This program and the accompanying materials are licensed and made available under
the terms and conditions of the BSD License that accompanies this distribution.
The full text of the license may be found at
http://opensource.org/licenses/bsd-license.php.

THE PROGRAM IS DISTRIBUTED UNDER THE BSD LICENSE ON AN "AS IS" BASIS,
WITHOUT WARRANTIES OR REPRESENTATIONS OF ANY KIND, EITHER EXPRESS OR IMPLIED.

**/

#include <Base.h>
#include <Uefi.h>
#include <IndustryStandard/Acpi.h>
#include <IndustryStandard/AcpiAml.h>
#include <Library/UefiLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/UefiRuntimeServicesTableLib.h>
#include <Library/DebugLib.h>
#include <Library/BaseMemoryLib.h>
#include <Library/IoLib.h>
#include <Library/PcdLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/PrintLib.h>
#include <Library/AcpiSdtParserLib.h>
#include <Protocol/FirmwareVolume2.h>
#include <Protocol/AcpiTable.h>
#include <Library/BoardAcpiTableLib.h>

typedef struct _ROOT_BRIDGE_DESCRIPTOR {
  UINTN   Socket;
  UINTN   Die;
  UINTN   RootBridge;
  UINTN   SocketRootBridge;
  UINTN   FabricID;
  UINTN   BusBase;
  UINTN   BusLimit;
  UINTN   PhysicalRootBridgeNumber;
} ROOT_BRIDGE_DESCRIPTOR;

//
// Function implementations
//

/**
  Locate the first instance of a protocol.  If the protocol requested is an
  FV protocol, then it will return the first FV that contains the ACPI table
  storage file.

  @param[in]  Protocol           The protocol to find.
  @param[in]  FfsGuid            The FFS that contains the ACPI table.
  @param[out] Instance           Return pointer to the first instance of the protocol.

  @retval EFI_SUCCESS           The function completed successfully.
  @retval EFI_NOT_FOUND         The protocol could not be located.
  @retval EFI_OUT_OF_RESOURCES  There are not enough resources to find the protocol.
**/
EFI_STATUS
LocateSupportProtocol (
  IN     EFI_GUID                      *Protocol,
  IN     EFI_GUID                      *FfsGuid,
     OUT VOID                          **Instance
  )
{
  EFI_STATUS              Status;
  EFI_HANDLE              *HandleBuffer;
  UINTN                   NumberOfHandles;
  EFI_FV_FILETYPE         FileType;
  UINT32                  FvStatus;
  EFI_FV_FILE_ATTRIBUTES  Attributes;
  UINTN                   Size;
  UINTN                   Index;

  //
  // Locate protocol.
  //
  Status = gBS->LocateHandleBuffer (
                  ByProtocol,
                  Protocol,
                  NULL,
                  &NumberOfHandles,
                  &HandleBuffer
                  );
  if (EFI_ERROR (Status)) {
    //
    // Defined errors at this time are not found and out of resources.
    //
    return Status;
  }

  //
  // Looking for FV with ACPI storage file
  //
  for (Index = 0; Index < NumberOfHandles; Index++) {

    //
    // Get the protocol on this handle
    // This should not fail because of LocateHandleBuffer
    //
    Status = gBS->HandleProtocol (
                    HandleBuffer[Index],
                    Protocol,
                    Instance
                    );
    ASSERT_EFI_ERROR (Status);

    //
    // See if it has the ACPI storage file
    //
    Size      = 0;
    FvStatus  = 0;
    Status = ((EFI_FIRMWARE_VOLUME2_PROTOCOL *) (*Instance))->ReadFile (
                                                              *Instance,
                                                              FfsGuid,
                                                              NULL,
                                                              &Size,
                                                              &FileType,
                                                              &Attributes,
                                                              &FvStatus
                                                              );

    //
    // If we found it, then we are done
    //
    if (Status == EFI_SUCCESS) {
      break;
    }
  }

  //
  // Our exit status is determined by the success of the previous operations
  // If the protocol was found, Instance already points to it.
  //
  //
  // Free any allocated buffers
  //
  FreePool (HandleBuffer);

  return Status;
}

/**
  Publish ACPI table from FV.

  @param[in]  FfsGuid            The FFS that contains the ACPI table.

  @retval EFI_SUCCESS           The function completed successfully.
**/
EFI_STATUS
PublishAcpiTablesFromFv (
  IN EFI_GUID *FfsGuid
  )
{
  EFI_STATUS                        Status;
  EFI_FIRMWARE_VOLUME2_PROTOCOL     *FwVol;
  EFI_ACPI_COMMON_HEADER            *CurrentTable;
  UINT32                            FvStatus;
  UINTN                             Size;
  UINTN                             TableHandle;
  INTN                              Instance;
  EFI_ACPI_TABLE_PROTOCOL           *AcpiTable;
  EFI_ACPI_TABLE_VERSION            Version;

  Instance      = 0;
  TableHandle   = 0;
  CurrentTable  = NULL;
  FwVol         = NULL;

  Status = gBS->LocateProtocol (&gEfiAcpiTableProtocolGuid, NULL, (VOID **)&AcpiTable);
  ASSERT_EFI_ERROR (Status);


  //
  // Locate the firmware volume protocol
  //
  Status = LocateSupportProtocol (
            &gEfiFirmwareVolume2ProtocolGuid,
            FfsGuid,
            (VOID **) &FwVol
            );
  ASSERT_EFI_ERROR (Status);

  //
  // Read tables from the storage file.
  //

  while (Status == EFI_SUCCESS) {
    Status = FwVol->ReadSection (
                      FwVol,
                      FfsGuid,
                      EFI_SECTION_RAW,
                      Instance,
                      (VOID **) &CurrentTable,
                      &Size,
                      &FvStatus
                      );

    if (EFI_ERROR (Status)) {
      continue;
    }
    if (!EFI_ERROR (VerifySdtTableHeader (CurrentTable, (UINT32)Size, "SSDT"))) {
      // Might need mods here
    } else if (!EFI_ERROR (VerifySdtTableHeader (CurrentTable, (UINT32)Size, "DSDT"))) {
      // Might need mods here
    } else {
      continue;
    }
    BoardUpdateAcpiTable (CurrentTable, &Version);
    //
    // Add the table
    //
    TableHandle = 0;
    Status = AcpiTable->InstallAcpiTable (
                            AcpiTable,
                            CurrentTable,
                            CurrentTable->Length,
                            &TableHandle
                            );

    if (CurrentTable) {
      Status = gBS->FreePool (CurrentTable);
    }
    //
    // Increment the instance
    //
    Instance++;
    CurrentTable = NULL;
  }
  //
  // Finished
  //
  return EFI_SUCCESS;
}

/**
  ACPI Platform driver installation function.

  @param[in] ImageHandle     Handle for this drivers loaded image protocol.
  @param[in] SystemTable     EFI system table.

  @retval EFI_SUCCESS        The driver installed without error.
  @retval EFI_ABORTED        The driver encountered an error and could not complete installation of
                             the ACPI tables.

**/
EFI_STATUS
EFIAPI
InstallAcpiBoard (
  IN EFI_HANDLE         ImageHandle,
  IN EFI_SYSTEM_TABLE   *SystemTable
  )
{
  EFI_STATUS  Status;

  Status = PublishAcpiTablesFromFv (&gEfiCallerIdGuid);
  ASSERT_EFI_ERROR (Status);

  return EFI_SUCCESS;
}
