/*****************************************************************************
 *
 * Copyright (C) 2020-2023 Advanced Micro Devices, Inc. All rights reserved.
 *
 *****************************************************************************/

DefinitionBlock (
  "DefDataInteger.aml",
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
  // DataIntegerObject
  // ****************************************************************************
  //
  // Data Integer 0
  //
  Name (TSTA, 0)

  //
  // Data Integer 1
  //
  Name (TSTB, 1)

  //
  // Data Integer 2 (Minimum ByteData Value)
  //
  Name (TSTC, 2)

  //
  // Data Integer 0xFF (Maximum ByteData Value)
  //
  Name (TSTD, 0xFF)

  //
  // Data Integer 0x00FF (Long ByteData Value)
  //
  Name (TSTE, 0x00FF)

  //
  // Data Integer 0x100 (Minimum WordData Value)
  //
  Name (TSTF, 0x100)

  //
  // Data Integer 0xFFFF (Maximum WordData Value)
  //
  Name (TSTG, 0xFFFF)

  //
  // Data Integer 0x000FFFF (Long WordData Value)
  //
  Name (TSTH, 0x0000FFFF)

  //
  // Data Integer 0x10000 (Minimum DwordData Value)
  //
  Name (TSTI, 0x10000)

  //
  // Data Integer 0xFFFFFFFF (Maximum DwordData Value)
  //
  Name (TSTJ, 0xFFFFFFFF)

  //
  // Data Integer 0x00000000FFFFFFFF (Long DwordData Value)
  //
  Name (TSTK, 0x00000000FFFFFFFF)

  //
  // Data Integer 0x100000000 (Minimum QwordData Value)
  //
  Name (TSTL, 0x100000000)

  //
  // Data Integer 0xFFFFFFFFFFFFFFFE (Maximum QwordData Value)
  //
  Name (TSTM, 0xFFFFFFFFFFFFFFFE)

  //
  // Long Qword Data Integer 0x0000000000000000FFFFFFFFFFFFFFFE
  //
  Name (TSTN, 0x0000000000000000FFFFFFFFFFFFFFFE)

  //
  // Data Integer 0xFFFFFFFFFFFFFFFF (AllOnes Data Value)
  //
  Name (TSTO, 0xFFFFFFFFFFFFFFFF)

}


