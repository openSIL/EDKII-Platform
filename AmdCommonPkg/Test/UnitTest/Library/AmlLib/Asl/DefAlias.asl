/*****************************************************************************
 *
 * Copyright (C) 2020-2023 Advanced Micro Devices, Inc. All rights reserved.
 *
 *****************************************************************************/
// test_file_name AliasTest.c
// suite_name Alias
DefinitionBlock (
  "DefAlias.aml",
  "SSDT",
  0x02, // DSDT revision.
        // A Revision field value greater than or equal to 2 signifies that integers
        // declared within the Definition Block are to be evaluated as 64-bit values
  "AMD   ",   // OEM ID (6 byte string)
  "AMD_EDK2",// OEM table ID  (8 byte string)
  0x00 // OEM version of DSDT table (4 byte Integer)
)
{
  //
  // Add named objects that we can alias
  //
  External(\_SB, DeviceObj)
  External(\_SB.PCI0, DeviceObj)
  External(\_SB.PCI0.TST0, UnknownObj)
  External(\_SB.PCI0.TS, IntObj)
  External(\_SB.PCI0.LPCB.MTH0, MethodObj)
  External(\_SB.PCI0.LPCB.EC0, DeviceObj)
  External(\_SB.PCI0.LPCB.EC0.MTH1, MethodObj, {StrObj})

  // test_name AliasSb
  // test_comment Alias(\_SB, TSTA)
  Alias(\_SB, TSTA)

  // test_name AliasLongNameStrings
  // test_comment Alias(\_SB.PCI0.TST0, \_SB.PCI0.XXX)
  Alias(\_SB.PCI0.TST0, \_SB.PCI0.XXX)

  // test_name AliasLongToShort
  // test_comment Alias(\_SB.PCI0.LPCB.EC0.MTH1, MTH1)
  Alias(\_SB.PCI0.LPCB.EC0.MTH1, MTH1)

  // test_name AliasShortToShort
  // test_comment Alias(MTH1, FORK)
  Alias(MTH1, FORK)

  // test_name AliasSameName
  // test_comment Alias(\_SB.PCI0.TST0, \_SB.PCI0.TST0)
  Alias(\_SB.PCI0.TST0, \_SB.PCI0.TST0)

  // test_name AliasFilledName
  // test_comment Alias(\_SB.PCI0.TS, TS)
  Alias(\_SB.PCI0.TS, TS)
}