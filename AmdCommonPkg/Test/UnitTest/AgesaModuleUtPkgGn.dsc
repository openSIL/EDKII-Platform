# Copyright (C) 2021 - 2024 Advanced Micro Devices, Inc. All rights reserved.
#
# @file AgesaModuleUtPkgGn.dsc
#


[Defines]
  PLATFORM_NAME           = AgesaModuleUtPkgGn
  PLATFORM_GUID           = A39EA781-17D9-46E1-85CD-FA455FE17017
  PLATFORM_VERSION        = 0.1
  DSC_SPECIFICATION       = 0x00010005
  OUTPUT_DIRECTORY        = Build/AmdCommonPkg/HostTest
  SUPPORTED_ARCHITECTURES = IA32|X64
  BUILD_TARGETS           = NOOPT
  SKUID_IDENTIFIER        = DEFAULT

!include UnitTestFrameworkPkg/UnitTestFrameworkPkgHost.dsc.inc
!include AmdCommonPkg/Test/UnitTest/AgesaModuleUtPkg.dsc.inc
!include AmdOpenSilPkg/opensil-uefi-interface/UnitTest/AmdOpenSilUtPkgGn.dsc.inc

[Components.common.HOST_APPLICATION]

  AmdCommonPkg/Test/UnitTest/Source/Examples/HelloWorldUt/HelloWorldUt.inf
