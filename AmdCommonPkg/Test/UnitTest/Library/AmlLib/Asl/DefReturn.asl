/*****************************************************************************
 *
 * Copyright (C) 2020-2024 Advanced Micro Devices, Inc. All rights reserved.
 *
 *****************************************************************************/
// test_file_name ReturnTest.c
// suite_name Return
DefinitionBlock (
  "DefReturn.aml",
  "SSDT",
  0x02, // DSDT revision.
        // A Revision field value greater than or equal to 2 signifies that integers
        // declared within the Definition Block are to be evaluated as 64-bit values
  "AMD   ",   // OEM ID (6 byte string)
  "AMD_EDK2",// OEM table ID  (8 byte string)
  0x00 // OEM version of DSDT table (4 byte Integer)
)
{
  // test_name NoReturnValue
  // test_comment Return()
  Method(TSTA, 0, NotSerialized) {
    Return()
  }

  // test_name ReturnZeroValue
  // test_comment Return(0)
  Method(TSTB, 0, NotSerialized) {
    Return(0)
  }

  // test_name ReturnOneValue
  // test_comment Return(1)
  Method(TSTC, 0, NotSerialized) {
    Return(1)
  }

  // test_name ReturnBufferString
  // test_comment Return(BUF0)
  Name(BUF0, 0x00)
  Method(TSTD, 0, NotSerialized) {
    Return(BUF0)
  }

  Scope(\_SB) {
    Device(AWR0) {
      Name(_HID, EISAID("PNP0C02"))
      Name(_UID, 0x90)

      Device(ABR0) {
        Name(_HID, EISAID("PNP0C02"))
        Name(_UID, 0x80)
        Name(AB02, 0x00)
        Method(A053, 0, NotSerialized) {
          Return(AB02)
        }
      }
    }
  }

  // test_name ReturnMethodCall
  // test_comment Return(\_SB.AWR0.ABR0.A053())
  Method(TSTE, 0, NotSerialized) {
    Return(\_SB.AWR0.ABR0.A053())
  }
}