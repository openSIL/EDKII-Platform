// /*****************************************************************************
//  *
//  * Copyright (C) 2021-2022 Advanced Micro Devices, Inc. All rights reserved.
//  *
//  *****************************************************************************/

#include "PlatformFchInit.h"
#define SPI_BASE 0xFEC10000ul

VOID
EnableLpcWideIoPort2 (
  IN VOID)
{

  if (FixedPcdGet8 (PcdIpmiInterfaceType) == IPMIDeviceInfoInterfaceTypeKCS) {
    DEBUG ((DEBUG_INFO, "Enabling wide io port 2.\n"));
    /*
     * Pleaser refer AMD PPR Vol 3 for respective Family/Model SoC
     * for detail information.
     */

    /*
     * Offset 0x090 (FCH::ITF::LPC::WIDE_IO_2)
     * IO_Base_Address_2. 16-bit PCI I/O base address for
     * wide generic port range.
     */
    PciWrite16 (PCI_SEGMENT_LIB_ADDRESS (0,
                                        FCH_LPC_BUS,
                                        FCH_LPC_DEV,
                                        FCH_LPC_FUNC,
                                        0x90),
                FixedPcdGet16 (PcdIpmiKCSPort));

    /*
     * Offset 0x048 (FCH::ITF::LPC::IO_MEM_PORT_DECODE_ENABLE)
     * Enables Wide IO port 2 (defined in registers 90/91h) enable.
     */
    PciWrite8 (PCI_SEGMENT_LIB_ADDRESS (0,
                                        FCH_LPC_BUS,
                                        FCH_LPC_DEV,
                                        FCH_LPC_FUNC,
                                        (FCH_LPC_REG48 + 3)
                                        ),
                0x2);

    /*
     * Offset 0x074 (FCH::ITF::LPC::ALTERNATIVE_WIDE_IO_RANGE_ENABLE)
     * Alternative_Wide_Io_2_Range_Enable to I/O address defined in
     * reg0x90 and reg0x91.
     */
    PciWrite8 (PCI_SEGMENT_LIB_ADDRESS (0,
                                        FCH_LPC_BUS,
                                        FCH_LPC_DEV,
                                        FCH_LPC_FUNC,
                                        FCH_LPC_REG74),
                0x8); // Enable BIT3 for Alternative_Wide_Io_2_Range_Enable
  }
}

VOID
EnableSpiTpm (
  IN VOID
)
{
  // Set TPM Decode
  PciAndThenOr8 (
    PCI_SEGMENT_LIB_ADDRESS (0,
      FCH_LPC_BUS,
      FCH_LPC_DEV,
      FCH_LPC_FUNC,
      FCH_LPC_REG7C
    ),
    0xFF,
    0x85
  );

  // Set RouteTpm2Spi
  PciAndThenOr8 (
    PCI_SEGMENT_LIB_ADDRESS (0,
      FCH_LPC_BUS,
      FCH_LPC_DEV,
      FCH_LPC_FUNC,
      FCH_LPC_REGA0
    ),
    0xFF,
    0x08
  );

  // Set AGPIO76 As SPI_TPM_CS_L
  MmioWrite8 (
    ACPI_MMIO_BASE + IOMUX_BASE + 0x4C,
    ((MmioRead8 (ACPI_MMIO_BASE + IOMUX_BASE + 0x4C) & 0xFF) | 0x01)
  );

  // Set drv_spiclk_earlier
  MmioWrite16 (
    SPI_BASE + FCH_SPI_MMIO_REG00,
    ((MmioRead16 (SPI_BASE + FCH_SPI_MMIO_REG00) & 0xFFFF) | 0x2000)
  );

  // Set SPI speed to 16.6MHz
  MmioWrite32 (
    SPI_BASE + FCH_SPI_MMIO_REG20,
    ((MmioRead32 (SPI_BASE + FCH_SPI_MMIO_REG20) | ~((UINT32) (0xFFFF << 16))) & (0x3333 << 16))
  );
}

/**
  Entry point for Platform FCH initialization.

  @retval  EFI_SUCCESS  returns success code.

**/
EFI_STATUS
EFIAPI
PlatformFchInit (
  IN VOID
  )
{
  EnableLpcWideIoPort2 ();
  EnableSpiTpm ();
  return EFI_SUCCESS;
}
