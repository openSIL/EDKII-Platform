/*****************************************************************************
 *
 * Copyright (C) 2020-2024 Advanced Micro Devices, Inc. All rights reserved.
 *
 *****************************************************************************/
// test_file_name DmaTest.c
// suite_name Dma
DefinitionBlock (
  "DefDma.aml",
  "SSDT",
  0x02, // DSDT revision.
        // A Revision field value greater than or equal to 2 signifies that integers
        // declared within the Definition Block are to be evaluated as 64-bit values
  "AMD   ",   // OEM ID (6 byte string)
  "AMD_EDK2",// OEM table ID  (8 byte string)
  0x00 // OEM version of DSDT table (4 byte Integer)
)
{
  // test_name CompatMaster8
  // test_comment Name(TSTA, ResourceTemplate()
  Name(TSTA, ResourceTemplate()
  {
    DMA(Compatibility, BusMaster, Transfer8){}
  })

  // test_name TypeAMaster8
  // test_comment Name(TSTB, ResourceTemplate()
  Name(TSTB, ResourceTemplate()
  {
    DMA(TypeA, BusMaster, Transfer8){}
  })

  // test_name TypeBMaster8
  // test_comment Name(TSTC, ResourceTemplate()
  Name(TSTC, ResourceTemplate()
  {
    DMA(TypeB, BusMaster, Transfer8){}
  })

  // test_name TypeFMaster8
  // test_comment Name(TSTD, ResourceTemplate()
  Name(TSTD, ResourceTemplate()
  {
    DMA(TypeF, BusMaster, Transfer8){}
  })

  // test_name CompatNotMaster8
  // test_comment Name(TSTE, ResourceTemplate()
  Name(TSTE, ResourceTemplate()
  {
    DMA(Compatibility, NotBusMaster, Transfer8){}
  })

  // test_name CompatNotMaster8_16
  // test_comment Name(TSTF, ResourceTemplate()
  Name(TSTF, ResourceTemplate()
  {
    DMA(Compatibility, NotBusMaster, Transfer8_16){}
  })

  // test_name CompatNotMaster16
  // test_comment Name(TSTG, ResourceTemplate()
  Name(TSTG, ResourceTemplate()
  {
    DMA(Compatibility, NotBusMaster, Transfer16){}
  })

  // test_name CompatNotMaster8Chan0
  // test_comment Name(TSTH, ResourceTemplate()
  Name(TSTH, ResourceTemplate()
  {
    DMA(Compatibility, NotBusMaster, Transfer8){0}
  })

  // test_name CompatNotMaster8Chan7
  // test_comment Name(TSTI, ResourceTemplate()
  Name(TSTI, ResourceTemplate()
  {
    DMA(Compatibility, NotBusMaster, Transfer8){7}
  })

  // test_name CompatNotMaster8ChanAll
  // test_comment Name(TSTJ, ResourceTemplate()
  Name(TSTJ, ResourceTemplate()
  {
    DMA(Compatibility, NotBusMaster, Transfer8){0,1,2,3,4,5,6,7}
  })
}