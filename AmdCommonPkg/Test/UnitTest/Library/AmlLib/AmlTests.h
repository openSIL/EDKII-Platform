/*****************************************************************************
 *
 * Copyright (C) 2019-2024 Advanced Micro Devices, Inc. All rights reserved.
 *
 *****************************************************************************/

#ifndef _AML_TESTS_H_
#define _AML_TESTS_H_

#include <Uefi.h>
#include <IndustryStandard/Acpi.h>

// INSERT HERE

/***************************************************************/
/* AML Alias Test Suite                        */
/***************************************************************/
EFI_STATUS
EFIAPI
AliasTestMain (
  VOID
);

/***************************************************************/
/* AML EisaId Test Suite                        */
/***************************************************************/
EFI_STATUS
EFIAPI
EisaIdTestMain (
  VOID
);

/***************************************************************/
/* AML QWordMemory Test Suite                        */
/***************************************************************/
EFI_STATUS
EFIAPI
QWordMemoryTestMain (
  VOID
);

/***************************************************************/
/* AML DWordMemory Test Suite                        */
/***************************************************************/
EFI_STATUS
EFIAPI
DWordMemoryTestMain (
  VOID
);

/***************************************************************/
/* AML Dma Test Suite                        */
/***************************************************************/
EFI_STATUS
EFIAPI
DmaTestMain (
  VOID
);

/***************************************************************/
/* AML Package Test Suite                        */
/***************************************************************/
EFI_STATUS
EFIAPI
PackageTestMain (
  VOID
);

// ---------------------------------------------------------------------------
//  Name Objects Encoding
// ---------------------------------------------------------------------------
/**
  Tests the DefNameString function of the AMD AML Library

  LeadNameChar      := ‘A’-‘Z’ | ‘_’
  DigitChar         := ‘0’-‘9’
  NameChar          := DigitChar | LeadNameChar
  RootChar          := ‘\’
  ParentPrefixChar  := ‘^’

  ‘A’-‘Z’           := 0x41 - 0x5A
  ‘_’               := 0x5F
  ‘0’-‘9’           := 0x30 - 0x39
  ‘\’               := 0x5C
  ‘^’               := 0x5E

  NameSeg           := <LeadNameChar NameChar NameChar NameChar>
                      // Notice that NameSegs shorter than 4 characters are filled with
                      // trailing underscores (‘_’s).
  NameString        := <RootChar NamePath> | <PrefixPath NamePath>
  PrefixPath        := Nothing | <‘^’ PrefixPath>
  NamePath          := NameSeg | DualNamePath | MultiNamePath | NullName

  DualNamePath      := DualNamePrefix NameSeg NameSeg
  DualNamePrefix    := 0x2E
  MultiNamePath     := MultiNamePrefix SegCount NameSeg(SegCount)
  MultiNamePrefix   := 0x2F

  SegCount          := ByteData

  @retval         EFI_SUCCESS
  @retval         Error status
**/
EFI_STATUS
EFIAPI
DefNameStringTestMain (
  VOID
  );

// ---------------------------------------------------------------------------
//  Data Objects Encoding
// ---------------------------------------------------------------------------
/**
  Tests the DefDataInteger function of the AMD AML Library

  ComputationalData := ByteConst | WordConst | DWordConst | QWordConst | String |
                       ConstObj | RevisionOp | DefBuffer
  DataObject        := ComputationalData | DefPackage | DefVarPackage
  DataRefObject     := DataObject | ObjectReference | DDBHandle
  ByteConst         := BytePrefix ByteData
  BytePrefix        := 0x0A
  WordConst         := WordPrefix WordData
  WordPrefix        := 0x0B
  DWordConst        := DWordPrefix DWordData
  DWordPrefix       := 0x0C
  QWordConst        := QWordPrefix QWordData
  QWordPrefix       := 0x0E
  ConstObj          := ZeroOp | OneOp | OnesOp
  ByteData          := 0x00 - 0xFF
  WordData          := ByteData[0:7] ByteData[8:15]
                       // 0x0000-0xFFFF
  DWordData         := WordData[0:15] WordData[16:31]
                       // 0x00000000-0xFFFFFFFF
  QWordData         := DWordData[0:31] DWordData[32:63]
                       // 0x0000000000000000-0xFFFFFFFFFFFFFFFF
  ZeroOp            := 0x00
  OneOp             := 0x01
  OnesOp            := 0xFF

  @retval         EFI_SUCCESS
  @retval         Error status
**/
EFI_STATUS
EFIAPI
DefDataIntegerTestMain (
  VOID
);

/**
  Tests the DefDataString function of the AMD AML Library

  ComputationalData   := String

  String              := StringPrefix AsciiCharList NullChar
  StringPrefix        := 0x0D
  AsciiCharList       := Nothing | <AsciiChar AsciiCharList>
  AsciiChar           := 0x01 - 0x7F
  NullChar            := 0x00

  @retval         EFI_SUCCESS
  @retval         Error status
**/
EFI_STATUS
EFIAPI
DefDataStringTestMain (
  VOID
);


// ---------------------------------------------------------------------------
//  Package Length Encoding
// ---------------------------------------------------------------------------

// ---------------------------------------------------------------------------
//  Term Objects Encoding
// ---------------------------------------------------------------------------

// ---------------------------------------------------------------------------
//  Namespace Modifier Objects Encoding
// ---------------------------------------------------------------------------

// ---------------------------------------------------------------------------
//  Named Objects Encoding
// ---------------------------------------------------------------------------
/**
  Tests the DefExternal function of the AMD AML Library

  DefExternal    := ExternalOp NameString ObjectType ArgumentCount
  ExternalOp     := 0x15
  ObjectType     := ByteData
  ArgumentCount  := ByteData (0 - 7)

  @retval         EFI_SUCCESS
  @retval         Error status
**/
EFI_STATUS
EFIAPI
DefExternalTestMain (
  VOID
  );

/**
  Tests the DefMethod function of the AMD AML Library

  DefMethod    := MethodOp PkgLength NameString MethodFlags TermList
  MethodOp     := 0x14
  MethodFlags  := ByteData  // bit 0-2: ArgCount (0-7)
                            // bit 3: SerializeFlag
                            // 0 NotSerialized
                            // 1 Serialized
                            // bit 4-7: SyncLevel (0x00-0x0f)

  @retval         EFI_SUCCESS
  @retval         Error status
**/
EFI_STATUS
EFIAPI
DefMethodTestMain (
  VOID
  );

// ---------------------------------------------------------------------------
//  Type 1 Opcodes Encoding
// --------------------------------------------------------------------------
/**
  Tests the DefReturn function of the AMD AML Library

  DefReturn  := ReturnOp ArgObject
  ReturnOp   := 0xA4
  ArgObject  := TermArg => DataRefObject

  @retval         EFI_SUCCESS
  @retval         Error status
**/
EFI_STATUS
EFIAPI
DefReturnTestMain (
  VOID
);

// ---------------------------------------------------------------------------
//  Type 2 Opcodes Encoding
// ---------------------------------------------------------------------------

// ---------------------------------------------------------------------------
//  Miscellaneous Objects Encoding
// ---------------------------------------------------------------------------

// ***************************************************************************
//  AML Assistance Functions
// ***************************************************************************

#endif // _AML_TESTS_H_
