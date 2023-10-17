/*****************************************************************************
 *
 * Copyright (C) 2020-2023 Advanced Micro Devices, Inc. All rights reserved.
 *
 *****************************************************************************/
// test_file_name PackageTest.c
// suite_name Package
// caps_name PACKAGE
DefinitionBlock (
  "DefPackage.aml",
  "SSDT",
  0x02, // DSDT revision.
        // A Revision field value greater than or equal to 2 signifies that integers
        // declared within the Definition Block are to be evaluated as 64-bit values
  "AMD   ",   // OEM ID (6 byte string)
  "AMD_EDK2",// OEM table ID  (8 byte string)
  0x00 // OEM version of DSDT table (4 byte Integer)
)
{
  // test_name PkgNoArgStr
  // test_comment Name(TSTA, Package() {"A"})
  Name(TSTA, Package() {"A"})

  // test_name PkgNoArgNum
  // test_comment Name(TSTB, Package() {45})
  Name(TSTB, Package() {45})

  // test_name PkgNoArgRef
  // test_comment Name(TSTC, Package() {TSTA, TSTB})
  Name(TSTC, Package() {TSTA, TSTB})

  // test_name PkgNoArgPkg
  // test_comment Name(TSTD, Package() {
  Name(TSTD, Package() {
    Package() {4},
    Package(6) {"A", "B", 42},
    Package(67) {},
    Package(1) {"CARP"}
    })

  // test_name PkgMultiTypeArgs
  // test_comment Name(TSTE, Package() {"A", 7, TSTB, Package() {Package() {10}, Package(4) {}}})
  Name(TSTE, Package() {"A", 7, TSTB, Package() {Package() {10}, Package(4) {}}})

  // test_name PkgSmallNumNoVar
  // test_comment Name(TSTF, Package(1) {}
  Name(TSTF, Package(1) {})

  // test_name PkgNumMaxNoArg
  // test_comment Name(TSTG, Package(255) { })
  Name(TSTG, Package(255) { })

  // test_name VarPkgNoArg
  // test_comment Name(TSTH, Package(256) { })
  Name(TSTH, Package(256) { })

  // test_name VarPkgStr
  // test_comment Name(TSTI, Package(256) {"A"})
  Name(TSTI, Package(256) {"A"})

  // test_name VarPkgRef
  // test_comment Name(TSTJ, Package(436) {TSTA, TSTB})
  Name(TSTJ, Package(436) {TSTA, TSTB})

  // test_name VarPkgPkg
  // test_comment Name(TSTK, Package(521) {
  Name(TSTK, Package(521) {
    Package() {4},
    Package(6) {"A", "B", 42},
    Package(67) {},
    Package(3) {"CARP"}
  })

  // test_name VarPkgMultiTypeArgs
  // test_comment Name(TSTL, Package(512) {"A", 7, TSTB, Package() {Package() {10}, Package(4) {}}})
  Name(TSTL, Package(512) {"A", 7, TSTB, Package() {Package() {10}, Package(4) {}}})


}