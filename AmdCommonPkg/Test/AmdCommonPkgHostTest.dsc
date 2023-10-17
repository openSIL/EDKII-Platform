## @file
# UefiCpuPkg DSC file used to build host-based unit tests.
#
# Copyright (c) 2020, Intel Corporation. All rights reserved.<BR>
# SPDX-License-Identifier: BSD-2-Clause-Patent
#
##

[Defines]
  PLATFORM_NAME           = AmdCommonPkgHostTest
  PLATFORM_GUID           = d6371c59-749d-4e15-94c3-ea53bccc4d5b
  PLATFORM_VERSION        = 0.1
  DSC_SPECIFICATION       = 0x00010005
  OUTPUT_DIRECTORY        = Build/AmdCommonPkg
  SUPPORTED_ARCHITECTURES = IA32|X64
  BUILD_TARGETS           = NOOPT
  SKUID_IDENTIFIER        = DEFAULT

!include UnitTestFrameworkPkg/UnitTestFrameworkPkgHost.dsc.inc

[LibraryClasses]
  AmlLib|AmdCommonPkg/Library/DxeAmdAmlLib/AmlLib.inf
#  MemoryAllocationLib|UnitTestFrameworkPkg/Library/Posix/MemoryAllocationLibPosix/MemoryAllocationLibPosix.inf

[PcdsFixedAtBuild]
    gEfiMdePkgTokenSpaceGuid.PcdFixedDebugPrintErrorLevel|0x804000CF

[Components]
  #
  # Build HOST_APPLICATION that tests the MtrrLib
  #
  AmdCommonPkg/Test/UnitTest/Library/AmlLib/AmlHostUnitTests.inf
  AmdCommonPkg/Library/DxeAmdAmlLib/AmlLib.inf

  #
  # Build HOST_APPLICATION Libraries
  #
#  MdePkg/Library/BaseLib/UnitTestHostBaseLib.inf
#  UnitTestFrameworkPkg/Library/CmockaLib/CmockaLib.inf
  UnitTestFrameworkPkg/Library/Posix/DebugLibPosix/DebugLibPosix.inf
  UnitTestFrameworkPkg/Library/Posix/MemoryAllocationLibPosix/MemoryAllocationLibPosix.inf
#  UnitTestFrameworkPkg/Library/UnitTestLib/UnitTestLibCmocka.inf
  MdePkg/Library/BaseLib/UnitTestHostBaseLib.inf