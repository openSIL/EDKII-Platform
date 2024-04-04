/*****************************************************************************
 *
 * Copyright (C) 2020-2022 Advanced Micro Devices, Inc. All rights reserved.
 *
 *****************************************************************************/
// test_file_name EisaIdTest.c
// suite_name EisaId
DefinitionBlock (
  "DefEisaId.aml",
  "SSDT",
  0x02, // DSDT revision.
        // A Revision field value greater than or equal to 2 signifies that integers
        // declared within the Definition Block are to be evaluated as 64-bit values
  "AMD   ",   // OEM ID (6 byte string)
  "AMD_EDK2",// OEM table ID  (8 byte string)
  0x00 // OEM version of DSDT table (4 byte Integer)
)
{
  // test_name EisaIdPnp0C02
  // test_comment EISAID ("PNP0C02")
  Name (_HID, EISAID ("PNP0C02"))
}