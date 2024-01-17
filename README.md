# README

## Beta Release of UEFI Host Firmware for Genoa openSIL POC (EDKII-Platform)

Thank you for your interest in the testing of this beta firmware. This firmware is provided to you as-is and is under the terms of the [beta license agreement](https://github.com/openSIL/EDKII-Platform/blob/main/2023.10.9%20Beta%20Firmware%20License%20(source%20code)%20v.2.pdf) that is available in this repository.


### Disclaimer

Please note that the content available in [this](https://github.com/openSIL/EDKII-Platform) repository is a beta version of the EDKII-Platform (a component of a sample UEFI Host Firmware for Genoa openSIL POC), which means it is still in a development and testing phase. While we have done our best to ensure the stability of this set of firmware, it may still contain bugs, incomplete features, or other issues.

### License

By using this beta firmware, you agree to the terms and conditions of the [Beta Firmare license agreement](https://github.com/openSIL/EDKII-Platform/blob/main/2023.10.9%20Beta%20Firmware%20License%20(source%20code)%20v.2.pdf).


### Intended usage

The beta release of the EDKII-Platform is intended to integrate with the AMD openSIL Genoa Proof-Of-Concept (POC), which is available at https://github.com/openSIL/openSIL.
## Quick Start Guide for Building the EDK2-Based BIOS

### EDKII Host FW Components required to build EDKII Genoa openSIL POC FW:

  * All components of the EDKII Host FW stack, including this AMD EDKII CRB platform package are provided as sample code and are intended to be used in accordance with the [Beta Firmware License](https://github.com/openSIL/EDKII-Platform/blob/main/2023.10.9%20Beta%20Firmware%20License%20(source%20code)%20v.2.pdf) provided in this repository.
### GitHub.com openSIL components

  * [AMD openSIL - Genoa POC](https://github.com/openSIL/openSIL)
  * [opensil-uefi-interface](https://github.com/openSIL/opensil-uefi-interface)
  * [AGCL-R (AGESA Compatibility Layer - Reduced)](https://github.com/openSIL/AGCL-R)
  * [EDKII-Platform Code (this repository)](https://github.com/openSIL/EDKII-Platform)

## Configuring Build Environment

  * Windows Cmd promt or Linux bash
  * Create a directory to be used as the workspace directory. "workspace" is
    used in the rest of the document to represent the path.
  * Clone EDKII Host FW Components
    * [opensil-uefi-interface](https://github.com/openSIL/opensil-uefi-interface)
      * Clone to 'workspace\AmdOpenSilPkg' directory.
      * The opensil-uefi-interface includes the [AMD openSIL - Genoa POC](https://github.com/openSIL/openSIL) repository as a submodule.
    * AGCL-R: [AGCL-R (AGESA Compatibility Layer - Reduced)](https://github.com/openSIL/AGCL-R)
    * Platform: [EDKII-Platform Code (this repository)](https://github.com/openSIL/EDKII-Platform)
      * Clone to 'workspace\Platform' directory.

  * Acquire EDK2 FW components
    * [edk2-stable202205](https://github.com/tianocore/edk2/releases/tag/edk2-stable202205)
      * Clone to 'workspace\edk2' directory.
    * [edk2-platforms](https://github.com/tianocore/edk2-platforms/commit/b8ffb76b471dae5e24badcd9e04033e8c9439ce3)
      * Clone to 'workspace\edk2-platforms' directory.

  * Acquire BMC GOP EFI Driver.
    * The Genoa-Onyx platform utilizes the AST 2600 BMC chip.
      For UEFI video support on the Genoa-Onyx, the GOP EFI driver must be obtained and placed in:
      `CrbSupportPkg\BmcGopDxe\X64\uefi_2600.efi>`
      To include the above driver in a build, copy the following file:
        source: `<workspace>\Platform\AmdCommonPkg\ToCopy\CrbSupportPkg\BmcGopDxe\BmcGopDxe.inf`
        destination: `<workspace>\CrbSupportPkg\BmcGopDxe\BmcGopDxe.inf`
  * Copy
    Windows:
      Source: Platform\PlatformTools\root_dbuild.cmd
      Destination: <workspace>\dbuild.cmd

    Linux:
      Source: Platform\PlatformTools\root_dbuild.sh
      Destination + executable set: <workspace>\dbuild.sh

  * **Properly install tools in Required Tools for Windows or Required Tools for Linux**
## Required Tools for Windows

  * **Git**

    Download URL: https://git-scm.com/

    Make sure any proxy requirements are set in the git config settings.

  * **Microsoft Visual Studio 2019** (tested)

    Make sure Visual Studio and the SDK are properly configured for your
    environment.

    The build will ultimately execute edksetup.bat from edk2 open source which
    should be able to detect properly installed Visual Studio components and
    SDKs.

    For inspiration on Visual Studio and SDK Environment Variables, please
    refer to:

    `edk2/Conf/tools_def.txt`

    `Platform/PlatformTools/BuildTools-env.cmd`

    If you do not have a Visual Studio install which can be located by
    edksetup.bat, you will need to configure all the proper PREFIX variables
    required for the build.

    *  **Microsoft SDK**

      Match chosen version of Microsoft Visual Studio.

  * **Python 3.x** (tested 3.7.4 & 3.9)

    Download URL: https://www.python.org

    Environment Variable: PYTHON_HOME

    E.g., `PYTHON_HOME = C:\Python39`

  * **Perl** (tested 5.32.1.1)

    Download URL: https://strawberryperl.com (tested)

    Strawberry Perl might require separately installing XML::LibXML
    `cpan install XML::LibXML`

    Environment Variable: PERL_PATH
    E.g., `PERL_PATH=C:\Strawberry\perl\bin`

    Alternatively, ActiveState perl is available if there is trouble
    installing Strawberry Perl.

  * **NASM** (tested 2.15.05)

    Environment Variable: NASM_PREFIX

  * **ASL compiler** (tested 20200110)

    Environment Variable: ASL_PREFIX

## Required Tools for Linux

  * **Ubuntu 20.04 LTS** Tested in Windows Subsystem for Linux

  * **Packages to install**

    build-essential

    uuid-dev

    python3

    python3-pip

    perl

    libperl-dev

    libxml-simple-perl

    libxml-parser-perl

    curl

    ca-certificates

    nasm

    iasl

  **Once files and folders are copied, the workspace should look like this:**

  * workspace
    * AGCL-R
    * AmdOpenSilPkg
      * opensil-uefi-interface
        * OpenSIL
        * Platform
        * SilToUefi
    * BuildTools
    * CrbSupportPkg
    * edk2
    * edk2-platforms
    * Platform
    * ...
    * readme.md
    * dbuild.cmd
    * dbuild.sh

## Building Platform BIOS (Windows CMD prompt)

  * Make sure your build environment is configured as referenced in
    [Required Tools for Windows](#required-tools-for-windows)

  * From the workspace directory, run the dbuild.cmd script.

    ex: `dbuild.cmd genoa-onyx` or 'dbuild genoa-onyx --edk2args="-b DEBUG"'

    By default (without options), the script will display a usage message and
    list all available CRBs to build.

  * To receive more verbose information for the first Level commands run:

    `dbuild --help`

  * Commands can have additional options.  Example:

    `dbuild genoa-onyx --help`

  * If you are not configuring the proper PREFIX variables for Visual Studio,
    you will need to provide an edk2args override to supply the tagname.
    The build process will use this to configure the build output directory
    before the edk2 build commences

    `--edk2args="-t VS2019"`

  * The final BIOS will be placed in <workspace>\\*.FD

## Building Platform BIOS (Linux bash)

  * Make sure your build environment is configured as referenced in
    [Required Tools for Linux](#required-tools-for-linux)

  * From the workspace directory, run the dbuild.sh script.

    ex: `./dbuild.sh genoa-onyx`

    By default (without options), the script will display a usage message and
    list all available CRBs to build.

  * To receive more verbose information for the first Level commands run:

    `./dbuild.sh --help`

  * Commands can have additional options.  Example:
    `./dbuild.sh genoa-onyx --help`

  * The final BIOS will be placed in <workspace>\\*.FD

## Enable CRB Serial Debug output

  * The EDKII build defaults to a "RELEASE" type build, which does not include
    serial debug output.

    To build a "DEBUG" build, pass to dbuild `--edk2args="-b DEBUG"`.

    Make sure to surround the arguments passed via edk2args with double quotes.

    In the Project.dsc file,
    `gEfiMdePkgTokenSpaceGuid.PcdFixedDebugPrintErrorLevel` can be modified
    to change the EDK2 debug output.
## Current Level of Functionality

  * Basic OS boot support on Onyx CRB Hardware