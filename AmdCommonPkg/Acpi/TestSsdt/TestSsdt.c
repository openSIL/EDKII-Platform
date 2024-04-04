/*****************************************************************************
 *
 * Copyright (C) 2020-2024 Advanced Micro Devices, Inc. All rights reserved.
 *
 *****************************************************************************/

#include <Library/AmlLib.h>
#include <Library/MemoryAllocationLib.h>
#include <Library/DebugLib.h>
#include <Library/BaseLib.h>
#include <Library/BaseMemoryLib.h>
#include <IndustryStandard/Acpi.h>
#include <Uefi.h>
#include <Protocol/AcpiTable.h>

#include <Library/UefiBootServicesTableLib.h>

#define OEM_REVISION_NUMBER      1
#define CREATOR_REVISION         2
/**
  This is a base driver to test any AML code needed.  Don't commit any
  additional code to this driver.

  DefinitionBlock (
  "",
  "SSDT",
  0x02,
  "AMD   ",   // OEM ID (6 byte string)
  "TESTSSDT", // OEM table ID  (8 byte string)
  0x00        // OEM version of SSDT table (4 byte Integer)
  )
  {

  }// End of ASL File

  **/
EFI_STATUS
EFIAPI
InstallTestSsdt (
  IN      EFI_HANDLE         ImageHandle,
  IN      EFI_SYSTEM_TABLE   *SystemTable
)
{
  EFI_STATUS                  Status;
  LIST_ENTRY                  *ListHead;
  AML_OBJECT_INSTANCE         *MainObject;
  EFI_ACPI_TABLE_PROTOCOL     *AcpiTableProtocol;
  UINTN                       TableHandle;
  LIST_ENTRY                  *Node;

  DEBUG ((DEBUG_INFO, "%a: Entry\n", __FUNCTION__));

  Status = EFI_SUCCESS;
  ZeroMem (&MainObject, sizeof (MainObject));
  ListHead = &MainObject->Link;
  TableHandle = 0;

  // Get Acpi Table Protocol
  Status = gBS->LocateProtocol (
                &gEfiAcpiTableProtocolGuid,
                NULL,
                &AcpiTableProtocol
                );
  if(EFI_ERROR (Status) || (AcpiTableProtocol == NULL)) {
    return EFI_NOT_FOUND;
  }

  InitializeListHead (ListHead);

  Status = AmlDefinitionBlock (AmlStart,               // START: DefinitionBlock (SSDT)
                               "SSDT",
                               EFI_ACPI_6_3_SECONDARY_SYSTEM_DESCRIPTION_TABLE_REVISION,
                               "AMD ",                 // OEM ID
                               "TESTSSDT",             // OEM Table ID
                               OEM_REVISION_NUMBER,
                               "AMD ",                 // Creator ID
                               CREATOR_REVISION,
                               ListHead);
  // *** Add code to test here ****
  Status |= AmlDefinitionBlock (AmlClose,               // START: DefinitionBlock (SSDT)
                                "SSDT",
                                EFI_ACPI_6_3_SECONDARY_SYSTEM_DESCRIPTION_TABLE_REVISION,
                                "AMD ",                 // OEM ID
                                "TESTSSDT",             // OEM Table ID
                                OEM_REVISION_NUMBER,
                                "AMD ",                 // Creator ID
                                CREATOR_REVISION,
                                ListHead);

  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_ERROR, "%a: ERROR Return Status failure\n", __FUNCTION__));
    ASSERT (FALSE);
  }

  // This should not be in a production driver
  if (!EFI_ERROR (Status)) {
    AmlDebugPrintLinkedObjects (ListHead);
  }

  // Get Main Object from List
  Node = GetNextNode(ListHead, ListHead);
#if 0
  MainObject = AML_OBJECT_INSTANCE_FROM_LINK(Node);

  //
  // Install ACPI table
  //
  Status = AcpiTableProtocol->InstallAcpiTable (
                              AcpiTableProtocol,
                              (EFI_ACPI_DESCRIPTION_HEADER*)&MainObject->Data[0],
                              MainObject->DataSize,
                              &TableHandle
                              );
#endif
  AmlFreeObjectList (ListHead);

  // This is a test driver, halt here.
  DEBUG ((DEBUG_ERROR, "%a: Halting now!!!!!\n", __FUNCTION__));
  CpuDeadLoop ();
  return Status;
}
