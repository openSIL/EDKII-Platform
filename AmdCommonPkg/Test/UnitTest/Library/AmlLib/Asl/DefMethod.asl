/*****************************************************************************
 *
 * Copyright (C) 2020-2022 Advanced Micro Devices, Inc. All rights reserved.
 *
 *****************************************************************************/
// test_file_name MethodTest.c
// suite_name Method
DefinitionBlock (
  "DefMethod.aml",
  "SSDT",
  0x02, // DSDT revision.
        // A Revision field value greater than or equal to 2 signifies that integers
        // declared within the Definition Block are to be evaluated as 64-bit values
  "AMD   ",   // OEM ID (6 byte string)
  "AMD_EDK2",// OEM table ID  (8 byte string)
  0x00 // OEM version of DSDT table (4 byte Integer)
)
{
  // test_name MethodWithArgs
  // test_comment Method(TSTA, 3, Serialized) {
  Method(TSTA, 3, Serialized) {
    Return(0)
  }

  // test_name MethodWithSyncSet
  // test_comment Method(TSTB, 7, Serialized, 4) {
  Method(TSTB, 7, Serialized, 4) {
    Return("ONE")
  }

  // test_name MethodMaxArgs
  // test_comment Method(TSTC, 7, NotSerialized) {
  Method(TSTC, 7, NotSerialized) {
    Return(0)
  }

  // test_name MethodWithInternals
  // test_comment Method(TSTD, 0, NotSerialized, 0) {
  Method(TSTD, 0, NotSerialized, 0) {
    Name(TMP1, One)
    Name(TMP2, "AMDTEST")
    Name(TMP3, "AMD AML TEST")
    Name(TMP4, 0x02)
  }

  // test_name MethodCallsMethod
  // test_comment Method(TSTE, 7, Serialized, 4) {
  Method(TSTE, 7, Serialized, 4) {
    TSTD()
  }

  // test_name MethodWithInternalCall
  // test_comment Method(TSTF, 7, NotSerialized) {
  Method(TSTF, 7, NotSerialized) {
    Name(TMP1, One)
    Name(TMP2, "AMDTEST")
    Name(TMP3, "AMD AML TEST")
    Name(TMP4, 0x02)
    TSTA(TMP1, TMP2, TMP3)
  }

  // Reserved Method, No Args, Return Name
  Name(PPCV, 0x00)
  // test_name ReservedMethodNoArg
  // test_comment Method(_PPC, 0, NotSerialized) {
  Method(_PPC, 0, NotSerialized) {
    Return(PPCV)
  }
}
