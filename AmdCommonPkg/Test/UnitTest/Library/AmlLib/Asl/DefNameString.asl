/*****************************************************************************
 *
 * Copyright (C) 2020-2024 Advanced Micro Devices, Inc. All rights reserved.
 *
 *****************************************************************************/

DefinitionBlock (
  "DefNameString.aml",
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
  // Invalid NULL Name
  // Note: \ is not a valid NameString in our Library
  External(\, UnknownObj)

  //
  // Test _ Fill Three Chars
  //
  External(\_, UnknownObj)

  //
  // Test _ Fill Two Chars
  //
  External(\__, DeviceObj)

  //
  // Test Fill Three Chars
  //
  External(\A, UnknownObj)

  //
  // Test Fill Two Chars
  //
  External(\BC, UnknownObj)

  //
  // Test Fill One Char
  //
  External(\DEF, UnknownObj)

  //
  // Test Dual Name Path
  //
  External(\GHI.JKLM, UnknownObj)

  //
  // Test Multi Name Path
  //
  External(\GHI.JKLM.NOPE, UnknownObj)

  //
  // Test _ Fill Three Chars
  //
  //External(^GHI.JKL, UnknownObj)

  //
  // Set Up Parent Prefix Tree
  //
  Scope(\_SB)
  {
    Method(MTHA, 0)
    {
      Return(^PCI1)
    }
    Method(MTHB, 0)
    {
      Return(^PCI1.MTHE)
    }
    Method(MTHC, 0)
    {
      Return(^PCI1.DEV3.MTHE)
    }
    Method(MTHD, 0)
    {
      Return(MTHA())
    }
    Method(MTHE, 0)
    {
      Return(^)  // Note: ^ is not a valid NameString in our Library
    }
    Device(PCI0)
    {
      Name(_HID, EISAID("PNP0C02"))
      Name(_UID, 0x90)
      Method(MTHA, 0)
      {
        Return(^^PCI1)
      }
      Method(MTHB, 0)
      {
        Return(^^PCI1.MTHE)
      }
      Method(MTHC, 0)
      {
        Return(^^PCI1.DEV3.MTHE)
      }
      Device(DEV0)
      {
        Name(_HID, EISAID("PNP0C02"))
        Name(_UID, 0x90)
        Method(MTHA, 0)
        {
          Return(^^^PCI1)
        }
        Method(MTHB, 0)
        {
          Return(^^^PCI1.MTHE)
        }
        Method(MTHC, 0)
        {
          Return(^^^PCI1.DEV3.MTHE)
        }
        Device(DEV1)
        {
          Name(_HID, EISAID("PNP0C02"))
          Name(_UID, 0x90)
          Method(MTHC, 0)
          {
            Return(^^^^PCI1.DEV3.MTHE)
          }
          Device(DEV2)
          {
            Name(_HID, EISAID("PNP0C02"))
            Name(_UID, 0x90)
            Method(MTHD, 0)
            {
              Return(^^^^^PCI1.DEV3.MTHE())
            }
          }
        }
      }

    }
    Device(PCI1)
    {
      Name(_HID, EISAID("PNP0C02"))
      Name(_UID, 0x90)
      Method(MTHE, 0)
      {
        Return("FORK")
      }
      Device(DEV3)
      {
        Name(_HID, EISAID("PNP0C02"))
        Name(_UID, 0x90)
        Method(MTHE, 0)
        {
          Return("FORK")
        }
      }
    }
  }
}