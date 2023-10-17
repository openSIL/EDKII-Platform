/*****************************************************************************
 *
 * Copyright (C) 2019-2023 Advanced Micro Devices, Inc. All rights reserved.
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
#include <FabricResourceManagerCmn.h>
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
#include <Protocol/FabricResourceManagerServicesProtocol.h>
#include <Protocol/FabricTopologyServices2.h>
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

/*
  Given the passed in PCIx device location, fill in the bus number resources for
  this root bridge.

  @param[in] SystemRootBridge     System wide root bridge 0..n
  @param[in] RootBridges          Array of root bridges including bus numbers
  @param[in] AcpiTable            Pointer to PCIx device in ACPI table
  @param[in] Size                 Size of remaining ACPI table

  @retval    EFI_SUCCESS          Filled bus number resources in ACPI table
  @retval    others               Failed to fill bus number resources
*/
EFI_STATUS
EFIAPI
FillInAcpiDeviceBusResources (
  IN UINTN                          SystemRootBridge,
  IN ROOT_BRIDGE_DESCRIPTOR         *RootBridges,
  IN UINT8                          *AcpiTable,
  IN UINT32                         Size
  )
{
  EFI_STATUS                        Status;
  UINT8                             *Object;
  UINT32                            RemainingSize;
  EFI_ACPI_WORD_ADDRESS_SPACE_DESCRIPTOR *Descriptor;
  /*
    Bus Resource Template Format:
    Name (R0B0, ResourceTemplate() {
      WordBusNumber (ResourceProducer, MinFixed, MaxFixed,,
                      0x0, 0x0, 0x0, 0x0, 0x0,,, RBB0)
    })
  */
  if ((UINT16)RootBridges[SystemRootBridge].BusLimit == 0) {
    return EFI_NOT_FOUND;
  }
  Object = (UINT8 *)AcpiTable;
  RemainingSize = Size;

  Status = LocateAcpiDefNameDefBufferByteList (
              (VOID **)&Object, &RemainingSize, "R0B0");
  if (EFI_ERROR (Status)) {
    ASSERT (FALSE);
    return Status;
  }

  Descriptor = (EFI_ACPI_WORD_ADDRESS_SPACE_DESCRIPTOR *)Object;
  if (Descriptor->Header.Header.Byte != ACPI_WORD_ADDRESS_SPACE_DESCRIPTOR) {
    ASSERT (FALSE);
    return EFI_NOT_FOUND;
  }
  if (RootBridges[SystemRootBridge].BusLimit > 0) {
    Descriptor->AddrRangeMin = (UINT16)RootBridges[SystemRootBridge].BusBase;
    Descriptor->AddrRangeMax = (UINT16)RootBridges[SystemRootBridge].BusLimit;
    Descriptor->AddrLen = Descriptor->AddrRangeMax -
                          Descriptor->AddrRangeMin + 1;
  }
  return EFI_SUCCESS;
}

/*
  Given the passed in PCIx device location, fill in the IO resources for
  this root bridge.

  @param[in] SystemRootBridge     System wide root bridge 0..n
  @param[in] AmdResources         buffer with root bridge resources
  @param[in] RootBridges          Array of root bridges including bus numbers
  @param[in] AcpiTable            Pointer to PCIx device in ACPI table
  @param[in] Size                 Size of remaining ACPI table

  @retval    EFI_SUCCESS          Filled IO resources in ACPI table
  @retval    others               Failed to fill IO resources
*/
EFI_STATUS
EFIAPI
FillInAcpiDeviceIoResources (
  IN UINTN                          SystemRootBridge,
  IN FABRIC_RESOURCE_FOR_EACH_RB    *AmdResources,
  IN ROOT_BRIDGE_DESCRIPTOR         *RootBridges,
  IN UINT8                          *AcpiTable,
  IN UINT32                         Size
  )
{
  EFI_STATUS                        Status;
  UINTN                             Socket;
  UINTN                             SocketRootBridge;
  UINT8                             *Object;
  UINT32                            RemainingSize;
  EFI_ACPI_WORD_ADDRESS_SPACE_DESCRIPTOR *Descriptor;
  /*
    // IO > 0x1000: To be updated during boot
    Name (R0I0, ResourceTemplate () {
      WordIO (ResourceProducer, MinFixed, MaxFixed,,,
              0x0, 0x0, 0x0, 0x0, 0x0,,, RII0)
    })
  */
  if ((UINT16)RootBridges[SystemRootBridge].BusLimit == 0) {
    return (EFI_NOT_FOUND);
  }
  Object = (UINT8 *)AcpiTable;
  RemainingSize = Size;
  Status = LocateAcpiDefNameDefBufferByteList (
              (VOID **)&Object, &RemainingSize, "R0I0");
  if (EFI_ERROR (Status)) {
    ASSERT (FALSE);
    return Status;
  }

  // Word Address Space Descriptor
  Descriptor = (EFI_ACPI_WORD_ADDRESS_SPACE_DESCRIPTOR *)Object;
  if (Descriptor->Header.Header.Byte != ACPI_WORD_ADDRESS_SPACE_DESCRIPTOR) {
    ASSERT (FALSE);
    return EFI_NOT_FOUND;
  }
  Socket = RootBridges[SystemRootBridge].Socket;
  SocketRootBridge = RootBridges[SystemRootBridge].SocketRootBridge;
  if (AmdResources->IO[Socket][SocketRootBridge].Size > 0) {
    Descriptor->AddrRangeMin = (UINT16)AmdResources->IO[Socket][SocketRootBridge].Base;
    Descriptor->AddrLen = (UINT16)AmdResources->IO[Socket][SocketRootBridge].Size;
    Descriptor->AddrRangeMax = Descriptor->AddrRangeMin +
                                Descriptor->AddrLen - 1;
  }
  return EFI_SUCCESS;
}

/*
  Given the passed in PCIx device location, fill in the Memory resources for
  this root bridge.

  @param[in] SystemRootBridge     System wide root bridge 0..n
  @param[in] AmdResources         buffer with root bridge resources
  @param[in] RootBridges          Array of root bridges including bus numbers
  @param[in] AcpiTable            Pointer to PCIx device in ACPI table
  @param[in] Size                 Size of remaining ACPI table

  @retval    EFI_SUCCESS          Filled Memory resources in ACPI table
  @retval    others               Failed to fill memory resources
*/
EFI_STATUS
EFIAPI
FillInAcpiDeviceMemoryResources (
  IN UINTN                          SystemRootBridge,
  IN FABRIC_RESOURCE_FOR_EACH_RB    *AmdResources,
  IN ROOT_BRIDGE_DESCRIPTOR         *RootBridges,
  IN UINT8                          *AcpiTable,
  IN UINT32                         Size
  )
{
  EFI_STATUS                        Status;
  UINTN                             Socket;
  UINTN                             SocketRootBridge;
  UINT8                             *Object;
  UINT32                            RemainingSize;
  EFI_ACPI_QWORD_ADDRESS_SPACE_DESCRIPTOR *Descriptor;
  UINTN                             ResourceIndex;
  CHAR8                             NameString[5];
  /*
    // Prefetch MMIO < 4GB: To be updated during boot
    Name (R0M0, ResourceTemplate () {
      QWordMemory (ResourceProducer, PosDecode, MinFixed, MaxFixed,
                    NonCacheable, ReadWrite,
                    0x0, 0x0, 0x0, 0x0, 0x0,,, RMM0)
    })
    // Non-Prefetch MMIO < 4GB: To be updated during boot
    Name (R0M1, ResourceTemplate () {
      QWordMemory (ResourceProducer, PosDecode, MinFixed, MaxFixed,
                    NonCacheable, ReadWrite,
                    0x0, 0x0, 0x0, 0x0, 0x0,,, RMM1)
    })
    // Prefetch MMIO > 4GB: To be updated during boot
    Name (R0M2, ResourceTemplate () {
      QWordMemory (ResourceProducer, PosDecode, MinFixed, MaxFixed,
                    NonCacheable, ReadWrite,
                    0x0, 0x0, 0x0, 0x0, 0x0,,, RMM2)
    })
    // Non-Prefetch MMIO > 4GB: To be updated during boot
    Name (R0M3, ResourceTemplate () {
      QWordMemory (ResourceProducer, PosDecode, MinFixed, MaxFixed,
                    NonCacheable, ReadWrite,
                    0x0, 0x0, 0x0, 0x0, 0x0,,, RMM3)
    })
  */
  if ((UINT16)RootBridges[SystemRootBridge].BusLimit == 0) {
    return EFI_NOT_FOUND;
  }
  Socket = RootBridges[SystemRootBridge].Socket;
  SocketRootBridge = RootBridges[SystemRootBridge].SocketRootBridge;
  for (ResourceIndex = 0; ResourceIndex <= 3; ResourceIndex++) {
    Object = (UINT8 *)AcpiTable;
    RemainingSize = Size;
    AsciiSPrint (NameString, 5, "R0M%1d", ResourceIndex);
    Status = LocateAcpiDefNameDefBufferByteList (
                (VOID **)&Object, &RemainingSize, NameString);
    if (EFI_ERROR (Status)) {
      ASSERT (FALSE);
      return Status;
    }
    // QWord Address Space Descriptor
    Descriptor = (EFI_ACPI_QWORD_ADDRESS_SPACE_DESCRIPTOR *)Object;
    if (Descriptor->Header.Header.Byte != ACPI_QWORD_ADDRESS_SPACE_DESCRIPTOR) {
      ASSERT (FALSE);
      return EFI_NOT_FOUND;
    }

    switch (ResourceIndex)
    {
      case 0:
        if (AmdResources->NonPrefetchableMmioSizeBelow4G[Socket][SocketRootBridge].Size > 0) {
          Descriptor->AddrRangeMin = AmdResources->NonPrefetchableMmioSizeBelow4G[Socket][SocketRootBridge].Base;
          Descriptor->AddrLen = AmdResources->NonPrefetchableMmioSizeBelow4G[Socket][SocketRootBridge].Size;
          Descriptor->AddrRangeMax = Descriptor->AddrRangeMin +
            Descriptor->AddrLen - 1;
        }
        break;
      case 1:
        if (AmdResources->PrefetchableMmioSizeBelow4G[Socket][SocketRootBridge].Size > 0) {
          Descriptor->AddrRangeMin = AmdResources->PrefetchableMmioSizeBelow4G[Socket][SocketRootBridge].Base;
          Descriptor->AddrLen = AmdResources->PrefetchableMmioSizeBelow4G[Socket][SocketRootBridge].Size;
          Descriptor->AddrRangeMax = Descriptor->AddrRangeMin +
            Descriptor->AddrLen - 1;
        }
        break;
      case 2:
        if (AmdResources->NonPrefetchableMmioSizeAbove4G[Socket][SocketRootBridge].Size > 0) {
          Descriptor->AddrRangeMin = AmdResources->NonPrefetchableMmioSizeAbove4G[Socket][SocketRootBridge].Base;
          Descriptor->AddrLen = AmdResources->NonPrefetchableMmioSizeAbove4G[Socket][SocketRootBridge].Size;
          Descriptor->AddrRangeMax = Descriptor->AddrRangeMin +
            Descriptor->AddrLen - 1;
        }
        break;
      case 3:
        if (AmdResources->PrefetchableMmioSizeAbove4G[Socket][SocketRootBridge].Size > 0) {
          Descriptor->AddrRangeMin = AmdResources->PrefetchableMmioSizeAbove4G[Socket][SocketRootBridge].Base;
          Descriptor->AddrLen = AmdResources->PrefetchableMmioSizeAbove4G[Socket][SocketRootBridge].Size;
          Descriptor->AddrRangeMax = Descriptor->AddrRangeMin +
            Descriptor->AddrLen - 1;
        }
        break;
    }
  }
  return EFI_SUCCESS;
}

/*
  Sort the collected PCI root bridges in ascending bus order

  @param[in] RootBridges          Array of root bridges including bus numbers
  @param[in] TotalRootBridges     Total number of root bridges in the system

  @retval    EFI_SUCCESS          Filled Memory resources in ACPI table
  @retval    others               Failed to fill memory resources
*/
VOID
SortRootBridges (
  ROOT_BRIDGE_DESCRIPTOR            *RootBridges,
  UINTN                             TotalRootBridges
  )
{
  ROOT_BRIDGE_DESCRIPTOR            RootBridgeTemp;
  UINTN                             InnerLoop;
  UINTN                             OuterLoop;
  // Sort RootBridgesArray
  for (OuterLoop = 0;
       OuterLoop < TotalRootBridges - 1;
       OuterLoop++) {
    for (InnerLoop = OuterLoop + 1;
        InnerLoop < TotalRootBridges;
        InnerLoop++) {
      if (RootBridges[OuterLoop].BusLimit == 0 ||
          RootBridges[OuterLoop].BusBase > RootBridges[InnerLoop].BusBase) {
        // Swap lower one down
        CopyMem (&RootBridgeTemp,
                 &RootBridges[OuterLoop],
                 sizeof (ROOT_BRIDGE_DESCRIPTOR));
        CopyMem (&RootBridges[OuterLoop],
                 &RootBridges[InnerLoop],
                 sizeof (ROOT_BRIDGE_DESCRIPTOR));
        CopyMem (&RootBridges[InnerLoop],
                 &RootBridgeTemp,
                 sizeof (ROOT_BRIDGE_DESCRIPTOR));
      }
    }
  }
}

/*
  Collect the PCI root bridges for the system in an array

  @param[in]      FabricTopology    Protocol to retrive AMD PCI resources
  @param[in,out]  RootBridges       Array of root bridges including bus numbers
  @param[in,out]  TotalRootBridges  Total number of root bridges in the system

  @retval    EFI_SUCCESS          Filled Memory resources in ACPI table
  @retval    others               Failed to fill memory resources
*/
EFI_STATUS
EFIAPI
CollectRootBridges(
  IN      AMD_FABRIC_TOPOLOGY_SERVICES2_PROTOCOL  *FabricTopology,
  IN OUT  ROOT_BRIDGE_DESCRIPTOR                  **RootBridges,
  IN OUT  UINTN                                   *TotalRootBridges
  )
{
  EFI_STATUS  Status;
  UINTN       TotalSockets;
  UINTN       TotalDies;
  UINTN       SocketDies;
  UINTN       SocketRootBridges;
  UINTN       SocketRootBridge;
  UINTN       DieRootBridges;
  UINTN       Socket;
  UINTN       Die;
  UINTN       RootBridge;
  UINTN       SystemRootBridge;
  ROOT_BRIDGE_DESCRIPTOR  *LocalRootBridges;


  Status = FabricTopology->GetSystemInfo (FabricTopology, &TotalSockets, &TotalDies,
                                 TotalRootBridges, NULL, NULL);
  if (EFI_ERROR (Status)) {
    return Status;
  }
  // Get root bridge location and bus information
  SystemRootBridge = 0;
  LocalRootBridges = AllocateZeroPool (sizeof (ROOT_BRIDGE_DESCRIPTOR)
                                       * *TotalRootBridges);
  if (LocalRootBridges == NULL) {
    return EFI_OUT_OF_RESOURCES;
  }
  *RootBridges = LocalRootBridges;
  for (Socket = 0; Socket < TotalSockets; Socket++) {
    Status = FabricTopology->GetProcessorInfo (FabricTopology, Socket,
                                               &SocketDies, &SocketRootBridges);
    if (EFI_ERROR (Status)) {
      return Status;
    }
    SocketRootBridge = 0;
    for (Die = 0; Die < SocketDies; Die++) {
      Status = FabricTopology->GetDieInfo (FabricTopology, Socket, Die,
                                           &DieRootBridges, NULL, NULL);
      if (EFI_ERROR (Status)) {
        return Status;
      }
      for ( RootBridge = 0; RootBridge < DieRootBridges; RootBridge++) {
        LocalRootBridges[SystemRootBridge].Socket = Socket;
        LocalRootBridges[SystemRootBridge].Die = Die;
        LocalRootBridges[SystemRootBridge].RootBridge = RootBridge;
        LocalRootBridges[SystemRootBridge].SocketRootBridge = SocketRootBridge;
        FabricTopology->GetRootBridgeInfo (
                          FabricTopology, Socket, Die, RootBridge,
                          &(LocalRootBridges[SystemRootBridge].FabricID),
                          &(LocalRootBridges[SystemRootBridge].BusBase),
                          &(LocalRootBridges[SystemRootBridge].BusLimit),
                          &(LocalRootBridges[SystemRootBridge].PhysicalRootBridgeNumber),
                          NULL, NULL);
        SystemRootBridge++;
        SocketRootBridge++;
      }
    }
  }
  SortRootBridges (*RootBridges, *TotalRootBridges);
  return EFI_SUCCESS;
}

/*
  Fill In Bus, Memory, and IO resources for each root bridge in the system.

  @param[in]  FfsGuid            The FFS that contains the ACPI table.

  @retval EFI_SUCCESS           The function completed successfully.
*/
EFI_STATUS
EFIAPI
FillInPciResources (
  IN UINT8      *AcpiTable,
  IN UINT32     Size
  )
{
  EFI_STATUS                        Status;
  AMD_FABRIC_TOPOLOGY_SERVICES2_PROTOCOL *FabricTopology;
  FABRIC_RESOURCE_MANAGER_PROTOCOL  *FabricResource;
  FABRIC_RESOURCE_FOR_EACH_RB       AmdResources;
  ROOT_BRIDGE_DESCRIPTOR            *RootBridges;
  UINTN                             SystemRootBridge;
  UINTN                             TotalRootBridges;
  UINT8                             *CurrentLocation;
  UINT32                            RemainingSize;
  CHAR8                             NameString[5];

  Status = gBS->LocateProtocol (&gAmdFabricTopologyServices2ProtocolGuid,
                                NULL, (VOID **)&FabricTopology);
  ASSERT_EFI_ERROR (Status);

  Status = gBS->LocateProtocol (&gAmdFabricResourceManagerServicesProtocolGuid,
                                NULL, (VOID **)&FabricResource);
  ASSERT_EFI_ERROR (Status);

  Status = FabricResource->FabricGetAvailableResource(FabricResource,
                                                      &AmdResources);
  ASSERT_EFI_ERROR (Status);

  Status = CollectRootBridges (FabricTopology, &RootBridges, &TotalRootBridges);
  if (EFI_ERROR (Status)) {
    return Status;
  }

  for (SystemRootBridge = 0; SystemRootBridge < TotalRootBridges; SystemRootBridge++) {
    CurrentLocation = AcpiTable;
    RemainingSize = Size;

    AsciiSPrint (NameString, 5, "PCI%1d", SystemRootBridge);
    Status = LocateAcpiDefDeviceTermList (
                (VOID **)&CurrentLocation, &RemainingSize, NameString);
    if (EFI_ERROR (Status)) {
      DEBUG ((DEBUG_ERROR, "%a: AcpiDevice %a Not Found\n", __FUNCTION__, NameString));
      continue;
    }
    Status = FillInAcpiDeviceBusResources (
                SystemRootBridge, RootBridges,
                CurrentLocation, RemainingSize
                );
    ASSERT_EFI_ERROR (Status);
    Status = FillInAcpiDeviceMemoryResources (
                SystemRootBridge, &AmdResources, RootBridges,
                CurrentLocation, RemainingSize
                );
    ASSERT_EFI_ERROR (Status);
    Status = FillInAcpiDeviceIoResources (
                SystemRootBridge, &AmdResources, RootBridges,
                CurrentLocation, RemainingSize
                );

    ASSERT_EFI_ERROR (Status);
  }
  if (RootBridges != NULL) {
    FreePool (RootBridges);
  }
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
