/*****************************************************************************
 *
 * Copyright (C) 2020-2022 Advanced Micro Devices, Inc. All rights reserved.
 *
 *****************************************************************************/

DefinitionBlock (
  "DefExternal.aml",
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
  // Test External Root
  //
  External(\_SB, DeviceObj)

  //
  // Test External PCI0
  //
  External(\_SB.PCI0, DeviceObj)

  //
  // Test Unknown Object
  //
  External(\_SB.PCI0.TST0, UnknownObj)

  //
  // Test Integer Object
  //
  External(\_SB.PCI0.TST1, IntObj)

  //
  // Test String Object
  //
  External(\_SB.PCI0.TST2, StrObj)

  //
  // Test Buffer Object
  //
  External(\_SB.PCI0.TST3, BuffObj)

  //
  // Test Package Object
  //
  External(\_SB.PCI0.TST4, PkgObj)

  //
  // Test Field Unit Object
  //
  External(\_SB.PCI0.TST5, FieldUnitObj)

  //
  // Test Device Object
  //
  External(\_SB.PCI0.TST6, DeviceObj)

  //
  // Test Event Object
  //
  External(\_SB.PCI0.TST7, EventObj)

  //
  // Test Method Object
  //
  External(\_SB.PCI0.TST8, MethodObj)

  //
  // Test Mutex Object
  //
  External(\_SB.PCI0.TST9, MutexObj)

  //
  // Test Op Region Object
  //
  External(\_SB.PCI0.TSTA, OpRegionObj)

  //
  // Test Power Resources Object
  //
  External(\_SB.PCI0.TSTB, PowerResObj)

  //
  // Test Processor Object
  //
  External(\_SB.PCI0.TSTC, ProcessorObj)

  //
  // Test Thermal Zone Object
  //
  External(\_SB.PCI0.TSTD, ThermalZoneObj)

  //
  // Test Buffer Field Object
  //
  External(\_SB.PCI0.TSTE, BuffFieldObj)

  //
  // Test External LPC Object
  //
  External(\_SB.PCI0.LPCB, DeviceObj)

  //
  // Test External Device
  //
  External(\_SB.PCI0.LPCB.EC0, DeviceObj)

  //
  // Test External IntegerObject in Tree
  //
  External(\_SB.PCI0.LPCB.EC0.ECRG, IntObj)

  //
  // Test External Mutex Object in Tree
  //
  External(\_SB.PCI0.LPCB.EC0.ECMX, MutexObj)

  //
  // Test External Method Object in Tree
  //
  External(\_SB.PCI0.LPCB.EC0.MTH0, MethodObj)

  //
  // Test External Method with one Arg
  //
  External(\_SB.PCI0.LPCB.EC0.MTH1, MethodObj, {StrObj})

  //
  // Test External Method with three Args
  //
  External(\_SB.PCI0.LPCB.EC0.MTH2, MethodObj, {StrObj, MethodObj, ProcessorObj})

  //
  // Test External Method Object with Max Args
  //
  External(\_SB.PCI0.LPCB.EC0.MTH3, MethodObj, {StrObj, ThermalZoneObj, IntObj, IntObj, IntObj, IntObj, IntObj})

  //
  // External Method Objects only populate if used
  //
  Scope(\_SB.PCI0)
  {
    Method(MTHZ, 0)
    {
      Return(\_SB.PCI0.TST8())
    }

    Method(MTHY, 0)
    {
      Return(\_SB.PCI0.LPCB.EC0.MTH0())
    }

    Method(MTHX, 0)
    {
      Return(\_SB.PCI0.LPCB.EC0.MTH1("TEST"))
    }

    Method(MTHW, 0)
    {
      Return(\_SB.PCI0.LPCB.EC0.MTH2("TEST", "TEST", "TEST"))
    }

    Method(MTHV, 0)
    {
      Return(\_SB.PCI0.LPCB.EC0.MTH3("TEST", "TEST", "TEST", "TEST", "TEST", "TEST", "TEST"))
    }
  }
}