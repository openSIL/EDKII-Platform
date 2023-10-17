/*****************************************************************************
 *
 * Copyright (C) 2020-2023 Advanced Micro Devices, Inc. All rights reserved.
 *
 *****************************************************************************/
// test_file_name QWordMemoryTest.c
// suite_name QWordMemory
DefinitionBlock (
  "DefQWordMemory.aml",
  "SSDT",
  0x02, // DSDT revision.
        // A Revision field value greater than or equal to 2 signifies that integers
        // declared within the Definition Block are to be evaluated as 64-bit values
  "AMD   ",   // OEM ID (6 byte string)
  "AMD_EDK2",// OEM table ID  (8 byte string)
  0x00 // OEM version of DSDT table (4 byte Integer)
)
{
  // test_name QWM_PosAllFixedCacheRW
  // test_comment Name(TSTA, ResourceTemplate()
  Name(TSTA, ResourceTemplate()
  {
    QWordMemory(ResourceProducer, PosDecode, MinFixed, MaxFixed, Cacheable, ReadWrite,
      0x00000000,
      0x000C0000,
      0x000DFFFF,
      0x00000000,
      0x00020000
    )
  })

  // test_name QWM_SubAllFixedCacheRW
  // test_comment Name(TSTB, ResourceTemplate()
  Name(TSTB, ResourceTemplate()
  {
    QWordMemory(ResourceProducer, SubDecode, MinFixed, MaxFixed, Cacheable, ReadWrite,
      0x00000000,
      0x000C0000,
      0x000DFFFF,
      0x00000000,
      0x00020000
    )
  })

  // test_name QWM_PosMaxFixedCacheRW
  // test_comment Name(TSTC, ResourceTemplate()
  Name(TSTC, ResourceTemplate()
  {
    QWordMemory(ResourceProducer, PosDecode, MinNotFixed, MaxFixed, Cacheable, ReadWrite,
      0x00000000,
      0x00000000,
      0x000DFFFF,
      0x00000000,
      0x00000000
    )
  })

  // test_name QWM_PosMinFixedCacheRW
  // test_comment Name(TSTD, ResourceTemplate()
  Name(TSTD, ResourceTemplate()
  {
    QWordMemory(ResourceProducer, PosDecode, MinFixed, MaxNotFixed, Cacheable, ReadWrite,
      0x00000000,
      0x000C0000,
      0x000DFFFF,
      0x00000000,
      0x00000000
    )
  })

  // test_name QWM_PosAllFixedWrCombRW
  // test_comment Name(TSTE, ResourceTemplate()
  Name(TSTE, ResourceTemplate()
  {
    QWordMemory(ResourceProducer, PosDecode, MinFixed, MaxFixed, WriteCombining, ReadWrite,
      0x00000000,
      0x000C0000,
      0x000DFFFF,
      0x00000000,
      0x00020000
    )
  })

  // test_name QWM_PosAllFixedPrefetchRW
  // test_comment Name(TSTF, ResourceTemplate()
  Name(TSTF, ResourceTemplate()
  {
    QWordMemory(ResourceProducer, PosDecode, MinFixed, MaxFixed, Prefetchable, ReadWrite,
      0x00000000,
      0x000C0000,
      0x000DFFFF,
      0x00000000,
      0x00020000
    )
  })

  // test_name QWM_PosAllFixedNonCacheRW
  // test_comment Name(TSTG, ResourceTemplate()
  Name(TSTG, ResourceTemplate()
  {
    QWordMemory(ResourceProducer, PosDecode, MinFixed, MaxFixed, NonCacheable, ReadWrite,
      0x00000000,
      0x000C0000,
      0x000DFFFF,
      0x00000000,
      0x00020000
    )
  })

  // test_name QWM_PosAllFixedCacheRO
  // test_comment Name(TSTH, ResourceTemplate()
  Name(TSTH, ResourceTemplate()
  {
    QWordMemory(ResourceProducer, PosDecode, MinFixed, MaxFixed, Cacheable, ReadOnly,
      0x00000000,
      0x000C0000,
      0x000DFFFF,
      0x00000000,
      0x00020000
    )
  })

  // test_name QWM_LargeMinMaxFixed
  // test_comment Name(TSTI, ResourceTemplate()
  Name(TSTI, ResourceTemplate()
  {
    QWordMemory(ResourceProducer, PosDecode, MinFixed, MaxFixed, Cacheable, ReadWrite,
      0x00000000,
      0x02000000,
      0xFFDFFFFF,
      0x00000000,
      0xFDE00000
    )
  })

  // test_name QWM_FixedMaxGranularity
  // test_comment Name(TSTJ, ResourceTemplate()
  Name(TSTJ, ResourceTemplate()
  {
    QWordMemory(ResourceProducer, PosDecode, MinNotFixed, MaxFixed, Cacheable, ReadWrite,
      0x7FFFFFFF,
      0x00000000,
      0xFFFFFFFF,
      0x00000000,
      0x00000000
    )
  })

  // test_name QWM_FixedMinGranularity
  // test_comment Name(TSTK, ResourceTemplate()
  Name(TSTK, ResourceTemplate()
  {
    QWordMemory(ResourceProducer, PosDecode, MinFixed, MaxNotFixed, Cacheable, ReadWrite,
      0x7FFFFFFF,
      0x80000000,
      0xFFFFFFFF,
      0x00000000,
      0x00000000
    )
  })

  // test_name QWM_FixedMinMaxTrans
  // test_comment Name(TSTL, ResourceTemplate()
  Name(TSTL, ResourceTemplate()
  {
    QWordMemory(ResourceProducer, PosDecode, MinFixed, MaxFixed, Cacheable, ReadWrite,
      0x00000000,
      0x02000000,
      0xFFDFFFFF,
      0xFD010000,
      0xFDE00000
    )
  })

  // test_name QWM_FixedMinGranTrans
  // test_comment Name(TSTM, ResourceTemplate()
  Name(TSTM, ResourceTemplate()
  {
    QWordMemory(ResourceProducer, PosDecode, MinFixed, MaxNotFixed, Cacheable, ReadWrite,
      0x7FFFFFFF,
      0x80000000,
      0xFFFFFFFF,
      0x00010000,
      0x00000000
    )
  })
}