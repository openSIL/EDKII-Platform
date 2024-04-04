/*****************************************************************************
 *
 * Copyright (C) 2020-2024 Advanced Micro Devices, Inc. All rights reserved.
 *
 *****************************************************************************/

#ifndef _AML_TEST_CONSTANTS_H_
#define _AML_TEST_CONSTANTS_H_

// ---------------------------------------------------------------------------
//  AML constants generated by iasl
//
//  This is to verify a 1-1 encoding between normal asl and the AmdAmlLib.
// ---------------------------------------------------------------------------

// INSERT HERE

/***************************************************************/
/* AML Alias Test Constants                             */
/***************************************************************/

extern UINT8   AslAliasSb[];
extern UINT8   AslAliasLongNameStrings[];
extern UINT8   AslAliasLongToShort[];
extern UINT8   AslAliasShortToShort[];
extern UINT8   AslAliasSameName[];
extern UINT8   AslAliasFilledName[];
/***************************************************************/
/* AML EisaId Test Constants                             */
/***************************************************************/

extern UINT8   AslEisaIdPnp0C02[];
/***************************************************************/
/* AML QWordMemory Test Constants                             */
/***************************************************************/

extern UINT8   AslQWM_PosAllFixedCacheRW[];
extern UINT8   AslQWM_SubAllFixedCacheRW[];
extern UINT8   AslQWM_PosMaxFixedCacheRW[];
extern UINT8   AslQWM_PosMinFixedCacheRW[];
extern UINT8   AslQWM_PosAllFixedWrCombRW[];
extern UINT8   AslQWM_PosAllFixedPrefetchRW[];
extern UINT8   AslQWM_PosAllFixedNonCacheRW[];
extern UINT8   AslQWM_PosAllFixedCacheRO[];
extern UINT8   AslQWM_LargeMinMaxFixed[];
extern UINT8   AslQWM_FixedMaxGranularity[];
extern UINT8   AslQWM_FixedMinGranularity[];
extern UINT8   AslQWM_FixedMinMaxTrans[];
extern UINT8   AslQWM_FixedMinGranTrans[];
/***************************************************************/
/* AML DWordMemory Test Constants                             */
/***************************************************************/

extern UINT8   AslDWM_PosAllFixedCacheRW[];
extern UINT8   AslDWM_SubAllFixedCacheRW[];
extern UINT8   AslDWM_PosMaxFixedCacheRW[];
extern UINT8   AslDWM_PosMinFixedCacheRW[];
extern UINT8   AslDWM_PosAllFixedWrCombRW[];
extern UINT8   AslDWM_PosAllFixedPrefetchRW[];
extern UINT8   AslDWM_PosAllFixedNonCacheRW[];
extern UINT8   AslDWM_PosAllFixedCacheRO[];
extern UINT8   AslDWM_LargeMinMaxFixed[];
extern UINT8   AslDWM_FixedMaxGranularity[];
extern UINT8   AslDWM_FixedMinGranularity[];
extern UINT8   AslDWM_FixedMinMaxTrans[];
extern UINT8   AslDWM_FixedMinGranTrans[];

/***************************************************************/
/* AML Dma Test Constants                             */
/***************************************************************/
extern UINT8   AslCompatMaster8[];
extern UINT8   AslTypeAMaster8[];
extern UINT8   AslTypeBMaster8[];
extern UINT8   AslTypeFMaster8[];
extern UINT8   AslCompatNotMaster8[];
extern UINT8   AslCompatNotMaster8_16[];
extern UINT8   AslCompatNotMaster16[];
extern UINT8   AslCompatNotMaster8Chan0[];
extern UINT8   AslCompatNotMaster8Chan7[];
extern UINT8   AslCompatNotMaster8ChanAll[];

/***************************************************************/
/* AML Package Test Constants                             */
/***************************************************************/
extern UINT8   AslPkgNoArgStr[];
extern UINT8   AslPkgNoArgNum[];
extern UINT8   AslPkgNoArgRef[];
extern UINT8   AslPkgNoArgPkg[];
extern UINT8   AslPkgMultiTypeArgs[];
extern UINT8   AslPkgSmallNumNoVar[];
extern UINT8   AslPkgNumMaxNoArg[];
extern UINT8   AslVarPkgStr[];
extern UINT8   AslVarPkgNoArg[];
extern UINT8   AslVarPkgRef[];
extern UINT8   AslVarPkgPkg[];
extern UINT8   AslVarPkgMultiTypeArgs[];

/***************************************************************/
/* AML Data Integer Test Constants                             */
/***************************************************************/
extern UINT8      DefDataIntTestA[];
extern UINT8      DefDataIntTestB[];
extern UINT8      DefDataIntTestC[];
extern UINT8      DefDataIntTestD[];
extern UINT8      DefDataIntTestE[];
extern UINT8      DefDataIntTestF[];
extern UINT8      DefDataIntTestG[];
extern UINT8      DefDataIntTestH[];
extern UINT8      DefDataIntTestI[];
extern UINT8      DefDataIntTestJ[];
extern UINT8      DefDataIntTestK[];
extern UINT8      DefDataIntTestL[];
extern UINT8      DefDataIntTestM[];
extern UINT8      DefDataIntTestN[];
extern UINT8      DefDataIntTestO[];


/***************************************************************/
/* AML Data String Test Constants                             */
/***************************************************************/
extern UINT8      DefDataStrTestA[];
extern UINT8      DefDataStrTestB[];
extern UINT8      DefDataStrTestC[];
extern UINT8      DefDataStrTestD[];
extern UINT8      DefDataStrTestE[];
extern UINT8      DefDataStrTestF[];
extern UINT8      DefDataStrTestG[];
/***************************************************************/
/* AML NameString Test Constants                               */
/***************************************************************/
extern UINT8  RootFill3_NameString[];

extern UINT8  RootFill2_NameString[];

extern UINT8  RootFill3NameString[];

extern UINT8  RootFill2NameString[];

extern UINT8  RootFill1NameString[];

extern UINT8  RootDualNameString[];

extern UINT8  RootMultiNameString[];

extern UINT8  MethodNameString[];

extern UINT8  PrefixNameString[];

extern UINT8  PrefixDualNameString[];

extern UINT8  PrefixMultiNameString[];

extern UINT8  DualPrefixNameString[];

extern UINT8  DualPrefixDualNameString[];

extern UINT8  DualPrefixMultiNameString[];

extern UINT8  TrioPrefixNameString[];

extern UINT8  TrioPrefixDualNameString[];

extern UINT8  TrioPrefixMultiNameString[];

extern UINT8  QuadPrefixMultiNameString[];

extern UINT8  PentaPrefixMultiNameStringMethod[];

/***************************************************************/
/* AML External Test Constants                                 */
/***************************************************************/
extern UINT8      DefExternalSb[];

extern UINT8      DefExternalPci0[];

extern UINT8      DefUnknownExternal[];

extern UINT8      DefIntegerExternal[];

extern UINT8      DefStringExternal[];

extern UINT8      DefBufferExternal[];

extern UINT8      DefPackageExternal[];

extern UINT8      DefFieldUnitExternal[];

extern UINT8      DefDeviceExternal[];

extern UINT8      DefEventExternal[];

extern UINT8      DefMethodExternal[];

extern UINT8      DefMutexExternal[];

extern UINT8      DefOpRegionExternal[];

extern UINT8      DefPowerResourceExternal[];

extern UINT8      DefProcessorExternal[];

extern UINT8      DefThermalZoneExternal[];

extern UINT8      DefBufferFieldExternal[];

extern UINT8      DefExternalLpc[];

extern UINT8      DefExternalEc0[];

extern UINT8      DefExternalTreeInt[];

extern UINT8      DefExternalTreeMux[];

extern UINT8      DefExternalTreeMethod[];

extern UINT8      DefExternalMethod1Arg[];

extern UINT8      DefExternalMethod3Arg[];

extern UINT8      DefExternalMethod7Arg[];

/***************************************************************/
/* AML Method Test Constants                                   */
/***************************************************************/
extern UINT8      DefMethodTestA[];

extern UINT8      DefMethodTestB[];

extern UINT8      DefMethodTestC[];

extern UINT8      DefMethodTestD[];

extern UINT8      DefMethodTestE[];

extern UINT8      DefMethodTestF[];

extern UINT8      DefMethodTestPPCV[];

/***************************************************************/
/* AML Return Test Constants                                   */
/***************************************************************/
extern UINT8      DefReturnZero[];

extern UINT8      DefReturnInteger1[];

extern UINT8      DefReturnStringBuf0[];

extern UINT8      DefReturnA053[];

#endif // _AML_TEST_CONSTANTS_H_