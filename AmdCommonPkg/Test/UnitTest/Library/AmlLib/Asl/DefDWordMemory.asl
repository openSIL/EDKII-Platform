/*****************************************************************************
 *
 * Copyright (C) 2020-2024 Advanced Micro Devices, Inc. All rights reserved.
 *
 *****************************************************************************/
// test_file_name DWordMemoryTest.c
// suite_name DWordMemory
DefinitionBlock (
  "DefDWordMemory.aml",
  "SSDT",
  0x02, // DSDT revision.
        // A Revision field value greater than or equal to 2 signifies that integers
        // declared within the Definition Block are to be evaluated as 64-bit values
  "AMD   ",   // OEM ID (6 byte string)
  "AMD_EDK2",// OEM table ID  (8 byte string)
  0x00 // OEM version of DSDT table (4 byte Integer)
)
{
  // test_name DWM_PosAllFixedCacheRW
  // test_comment Name(TSTA, ResourceTemplate()
  Name(TSTA, ResourceTemplate()
  {
    DWordMemory(ResourceProducer, PosDecode, MinFixed, MaxFixed, Cacheable, ReadWrite,
      0x00000000,
      0x000C0000,
      0x000DFFFF,
      0x00000000,
      0x00020000
    )
  })

  // test_name DWM_SubAllFixedCacheRW
  // test_comment Name(TSTB, ResourceTemplate()
  Name(TSTB, ResourceTemplate()
  {
    DWordMemory(ResourceProducer, SubDecode, MinFixed, MaxFixed, Cacheable, ReadWrite,
      0x00000000,
      0x000C0000,
      0x000DFFFF,
      0x00000000,
      0x00020000
    )
  })

  // test_name DWM_PosMaxFixedCacheRW
  // test_comment Name(TSTC, ResourceTemplate()
  Name(TSTC, ResourceTemplate()
  {
    DWordMemory(ResourceProducer, PosDecode, MinNotFixed, MaxFixed, Cacheable, ReadWrite,
      0x00000000,
      0x00000000,
      0x000DFFFF,
      0x00000000,
      0x00000000
    )
  })

  // test_name DWM_PosMinFixedCacheRW
  // test_comment Name(TSTD, ResourceTemplate()
  Name(TSTD, ResourceTemplate()
  {
    DWordMemory(ResourceProducer, PosDecode, MinFixed, MaxNotFixed, Cacheable, ReadWrite,
      0x00000000,
      0x000C0000,
      0x000DFFFF,
      0x00000000,
      0x00000000
    )
  })

  // test_name DWM_PosAllFixedWrCombRW
  // test_comment Name(TSTE, ResourceTemplate()
  Name(TSTE, ResourceTemplate()
  {
    DWordMemory(ResourceProducer, PosDecode, MinFixed, MaxFixed, WriteCombining, ReadWrite,
      0x00000000,
      0x000C0000,
      0x000DFFFF,
      0x00000000,
      0x00020000
    )
  })

  // test_name DWM_PosAllFixedPrefetchRW
  // test_comment Name(TSTF, ResourceTemplate()
  Name(TSTF, ResourceTemplate()
  {
    DWordMemory(ResourceProducer, PosDecode, MinFixed, MaxFixed, Prefetchable, ReadWrite,
      0x00000000,
      0x000C0000,
      0x000DFFFF,
      0x00000000,
      0x00020000
    )
  })

  // test_name DWM_PosAllFixedNonCacheRW
  // test_comment Name(TSTG, ResourceTemplate()
  Name(TSTG, ResourceTemplate()
  {
    DWordMemory(ResourceProducer, PosDecode, MinFixed, MaxFixed, NonCacheable, ReadWrite,
      0x00000000,
      0x000C0000,
      0x000DFFFF,
      0x00000000,
      0x00020000
    )
  })

  // test_name DWM_PosAllFixedCacheRO
  // test_comment Name(TSTH, ResourceTemplate()
  Name(TSTH, ResourceTemplate()
  {
    DWordMemory(ResourceProducer, PosDecode, MinFixed, MaxFixed, Cacheable, ReadOnly,
      0x00000000,
      0x000C0000,
      0x000DFFFF,
      0x00000000,
      0x00020000
    )
  })

  // test_name DWM_LargeMinMaxFixed
  // test_comment Name(TSTI, ResourceTemplate()
  Name(TSTI, ResourceTemplate()
  {
    DWordMemory(ResourceProducer, PosDecode, MinFixed, MaxFixed, Cacheable, ReadWrite,
      0x00000000,
      0x02000000,
      0xFFDFFFFF,
      0x00000000,
      0xFDE00000
    )
  })

  // test_name DWM_FixedMaxGranularity
  // test_comment Name(TSTJ, ResourceTemplate()
  Name(TSTJ, ResourceTemplate()
  {
    DWordMemory(ResourceProducer, PosDecode, MinNotFixed, MaxFixed, Cacheable, ReadWrite,
      0x7FFFFFFF,
      0x00000000,
      0xFFFFFFFF,
      0x00000000,
      0x00000000
    )
  })

  // test_name DWM_FixedMinGranularity
  // test_comment Name(TSTK, ResourceTemplate()
  Name(TSTK, ResourceTemplate()
  {
    DWordMemory(ResourceProducer, PosDecode, MinFixed, MaxNotFixed, Cacheable, ReadWrite,
      0x7FFFFFFF,
      0x80000000,
      0xFFFFFFFF,
      0x00000000,
      0x00000000
    )
  })

  // test_name DWM_FixedMinMaxTrans
  // test_comment Name(TSTL, ResourceTemplate()
  Name(TSTL, ResourceTemplate()
  {
    DWordMemory(ResourceProducer, PosDecode, MinFixed, MaxFixed, Cacheable, ReadWrite,
      0x00000000,
      0x02000000,
      0xFFDFFFFF,
      0xFD010000,
      0xFDE00000
    )
  })

  // test_name DWM_FixedMinGranTrans
  // test_comment Name(TSTM, ResourceTemplate()
  Name(TSTM, ResourceTemplate()
  {
    DWordMemory(ResourceProducer, PosDecode, MinFixed, MaxNotFixed, Cacheable, ReadWrite,
      0x7FFFFFFF,
      0x80000000,
      0xFFFFFFFF,
      0x00010000,
      0x00000000
    )
  })
}