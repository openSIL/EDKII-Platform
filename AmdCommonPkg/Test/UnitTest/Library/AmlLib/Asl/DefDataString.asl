/*****************************************************************************
 *
 * Copyright (C) 2020-2022 Advanced Micro Devices, Inc. All rights reserved.
 *
 *****************************************************************************/

DefinitionBlock (
  "DefDataString.aml",
  "SSDT",
  0x02, // DSDT revision.
        // A Revision field value greater than or equal to 2 signifies that integers
        // declared within the Definition Block are to be evaluated as 64-bit values
  "AMD   ",   // OEM ID (6 byte string)
  "AMD_EDK2",// OEM table ID  (8 byte string)
  0x00 // OEM version of DSDT table (4 byte Integer)
)
{

  // ****************************************************************************
  // DataStringObject
  // ****************************************************************************
  //
  // Data String "" - Empty String
  //
  Name (TSTA, "")

  //
  // Data String "A" - Short String
  //
  Name (TSTB, "A")

  //
  // Data String "ABCDEFGHIJKLMNOPQRSTUVWXYZ" - Upper case
  //
  Name (TSTC, "ABCDEFGHIJKLMNOPQRSTUVWXYZ")

  //
  // Data String "abcdefghijklmnopqrstuvwxyz" - Lower case
  //
  Name (TSTD, "abcdefghijklmnopqrstuvwxyz")

  //
  // Data String "01234567890" - Numbers
  //
  Name (TSTE, "01234567890")

  //
  // Data String "A !#$%^*(\\)_+`-.,/?;:<>=?@{}[]|~" - Symbols
  //
  Name (TSTF, "A !#$%^*(\\)_+`-.,/?;:<>=?@{}[]|~")

  //
  // Data String - Escape Characters
  //
  Name (TSTG, "\a\b\f\n\r\t\v")

}
