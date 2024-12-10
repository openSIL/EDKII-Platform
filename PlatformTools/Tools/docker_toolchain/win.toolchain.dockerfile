# Copyright (C) 2024 Advanced Micro Devices, Inc. All rights reserved.
#
# SPDX-License-Identifier: MIT
#
# Dockerfile for Windows-Based Development Environment
# This Dockerfile is designed to be used as a development environment for Windows-based build projects.
# It is particularly useful for quickly starting AMD POC development and evaluation.
# The image provides a ready-to-use environment with all the necessary tools installed.
# Bootstrap AMD Tool is also included to fetch project source code from needed repositories
# Note: <project_manifest.yml> is not included, you must manually copy it to the running container
#       in order to run bootstrapper tool.
#
# Usage:
# - build image: docker build -f .\win.toolchain.dockerfile --tag win.toolchain .
# - launch container: docker run -v .:c:/workspace/<project_folder> -it --rm win.toolchain
# - build project:
#   - bootstrap: python .\bootstrapper\bootstrapper.py -f .\bootstrapper\mfs\<project_manifest>.yml
#   - build:
#       - cd <project_folder>
#       - .\dbuild.cmd <platform> --edk2args="-t VS2019"

FROM mcr.microsoft.com/windows/servercore:ltsc2019

SHELL ["powershell", "-Command", "$ErrorActionPreference = 'Stop'; $ProgressPreference = 'SilentlyContinue';"]

ENV INSTALL_DIR='C:\Install'
ENV WORKSPACE_DIR='C:\workspace'
ENV BUILDTOOLS_DIR="${INSTALL_DIR}\BuildTools"

RUN mkdir $env:INSTALL_DIR; mkdir $env:WORKSPACE_DIR

WORKDIR ${INSTALL_DIR}

# Create PowerShell script to expand compressed files
RUN Set-Content -Path (Join-Path $env:INSTALL_DIR '\fexpand.ps1') \
        -Value 'Expand-Archive -Path $args[0] -DestinationPath $args[1]'

# Install Vim
ENV VIM_INSTALLER='vim_installer.exe'
ENV VIM_URL='https://github.com/vim/vim-win32-installer/releases/download/v9.1.0/gvim_9.1.0_x86_signed.exe'
ENV VIM_PATH='C:\Program Files (x86)\Vim\vim91'
RUN Invoke-WebRequest -Uri $env:VIM_URL -OutFile $env:VIM_INSTALLER; \
    Start-Process -FilePath $env:VIM_INSTALLER -ArgumentList '/S' -NoNewWindow -Wait; \
    Remove-Item -Force $env:VIM_INSTALLER; \
    [System.Environment]::SetEnvironmentVariable('PATH', $env:PATH + ';' + $env:VIM_PATH, 'Machine')
        
# Install GIT
ENV GIT_VERSION='2.47.0'
ENV GIT_INSTALLER="Git-${GIT_VERSION}-64-bit.exe"
ENV GIT_URL="https://github.com/git-for-windows/git/releases/download/v${GIT_VERSION}.windows.1/Git-${GIT_VERSION}-64-bit.exe"
RUN Invoke-WebRequest -Uri $env:GIT_URL -OutFile $env:GIT_INSTALLER; \
    Start-Process -FilePath $env:GIT_INSTALLER \
        -ArgumentList '/VERYSILENT /NORESTART' \
        -Wait; \
    Remove-Item -Force $env:GIT_INSTALLER
        
# Install MSVC2019 Build Tools
# - Compiler v16
# - Win10-SDK v19041
ENV MSVC_COMPILER='16'
ENV MSVC_INSTALLER='vs_BuildTools.exe'
ENV MSVC_URL="https://aka.ms/vs/${MSVC_COMPILER}/release/${MSVC_INSTALLER}"
RUN Invoke-WebRequest -Uri $env:MSVC_URL -OutFile $env:MSVC_INSTALLER; \
    Start-Process -FilePath $env:MSVC_INSTALLER \
        -ArgumentList '--quiet --wait --norestart --nocache', \
            '--add Microsoft.VisualStudio.Workload.VCTools', \
            '--add Microsoft.VisualStudio.Component.VC.Tools.x86.x64', \
            '--add Microsoft.VisualStudio.Component.Windows10SDK.19041' \
        -Wait; \
    Remove-Item -Force $env:MSVC_INSTALLER

# Install Python
ENV PYTHON_VERSION='3.8.2'
ENV PYTHON_INSTALLER='python-installer.exe'
ENV PYTHON_URL="https://www.python.org/ftp/python/${PYTHON_VERSION}/python-${PYTHON_VERSION}-amd64.exe"
ENV PYTHON_DIR='C:\\Program Files\\Python38\\'
RUN Invoke-WebRequest -Uri $env:PYTHON_URL -OutFile $env:PYTHON_INSTALLER; \
    Start-Process -FilePath $env:PYTHON_INSTALLER \
        -ArgumentList '/quiet InstallAllUsers=1 PrependPath=1 Include_test=0' \
        -Wait; \
    Remove-Item -Force $env:PYTHON_INSTALLER

# PIP Update and Install Libs
RUN python -m pip install --upgrade pip; \
    pip install \
        pyyaml \
        requests \
        colorlog

# Install NASM
ENV NASM_VERSION='2.15.05'
ENV NASM_INSTALLER="nasm-${NASM_VERSION}-win64.zip"
ENV NASM_URL="https://www.nasm.us/pub/nasm/releasebuilds/${NASM_VERSION}/win64/${NASM_INSTALLER}"
ENV NASM_DIR="${BUILDTOOLS_DIR}\\nasm-${NASM_VERSION}\\"
RUN Invoke-WebRequest -Uri $env:NASM_URL -OutFile $env:NASM_INSTALLER; \
    .\fexpand.ps1 $env:NASM_INSTALLER $env:BUILDTOOLS_DIR; \
    Remove-Item -Force $env:NASM_INSTALLER

# Install IASL
ENV IASL_VERSION='20210930'
ENV IASL_INSTALLER="iasl-win-${IASL_VERSION}.zip"
ENV IASL_URL="https://downloadmirror.intel.com/774873/iasl-win-${IASL_VERSION}.zip"
ENV IASL_DIR="${BUILDTOOLS_DIR}\\IASL\\"
RUN Invoke-WebRequest -Uri $env:IASL_URL -OutFile $env:IASL_INSTALLER; \
    .\fexpand.ps1 $env:IASL_INSTALLER (Join-Path $env:BUILDTOOLS_DIR '\IASL'); \
    Remove-Item -Force $env:IASL_INSTALLER

# Install PERL
ENV PERL_VERSION='5.32.1'
ENV PERL_INSTALLER="strawberry-perl-${PERL_VERSION}.1-32bit-portable.zip"
ENV PERL_URL="https://strawberryperl.com/download/${PERL_VERSION}.1/${PERL_INSTALLER}"
ENV PERL_DIR="${BUILDTOOLS_DIR}\\Strawberry"
RUN Invoke-WebRequest -Uri $env:PERL_URL -OutFile $env:PERL_INSTALLER; \
    .\fexpand.ps1 $env:PERL_INSTALLER $env:PERL_DIR; \
    Remove-Item -Force $env:PERL_INSTALLER
    
# Set BuildTools Environment Variables
RUN [System.Environment]::SetEnvironmentVariable( 'PERL_PATH',  (Join-Path $env:PERL_DIR '\perl\bin'), 'Machine'); \
    [System.Environment]::SetEnvironmentVariable( 'PYTHON_HOME', $env:PYTHON_DIR, 'Machine'); \
    [System.Environment]::SetEnvironmentVariable( 'NASM_PREFIX', $env:NASM_DIR, 'Machine'); \
    [System.Environment]::SetEnvironmentVariable( 'IASL_PREFIX', $env:IASL_DIR, 'Machine')

WORKDIR ${WORKSPACE_DIR}

# Workaround to avoid dubious ownership when using persitent volumes
RUN git config --global --add safe.directory '*'

ENTRYPOINT ["powershell.exe", "-NoLogo", "-ExecutionPolicy", "Bypass"]
