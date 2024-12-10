# Copyright (C) 2024 Advanced Micro Devices, Inc. All rights reserved.
#
# SPDX-License-Identifier: MIT
#
# Dockerfile for Linux-Based Development Environment
# This Dockerfile is designed to be used as a development environment for Linux-based build projects.
# It is particularly useful for quickly starting AMD POC development and evaluation.
# The image provides a ready-to-use environment with all the necessary tools installed.
# Bootstrap AMD Tool is also included to fetch project source code from needed repositories
# Note: <project_manifest.yml> is not included, you must manually copy it to the running container
#       in order to run bootstrapper tool.
#
# Usage:
# - build image: docker build -f .\linux.toolchain.dockerfile --tag linux.toolchain .
# - launch container: docker run -v .:/workspace/<project_folder> -it --rm linux.toolchain
# - build project:
#   - bootstrap: python ./bootstrapper/bootstrapper.py -f ./bootstrapper/mfs/<project_manifest>.yml
#   - build:
#       - cd <project_folder>
#       - chmod +x dbuild.sh
#       - ./dbuild.sh <platform>

FROM ubuntu:20.04

ENV WORKSPACE_DIR='/workspace'

# Set environment variables to avoid interactive prompts during package installation
ENV DEBIAN_FRONTEND=noninteractive

# Update and install required packages
RUN apt-get update && apt-get install -y \
    wget \
    curl \
    alien \
    vim \
    git \
    build-essential \
    uuid-dev \
    python3 \
    python3-pip \
    perl \
    libperl-dev \
    libxml-simple-perl \
    libxml-parser-perl \
    ca-certificates \
    iasl \
    && apt-get clean && rm -rf /var/lib/apt/lists/*

# Create a global symlink for python3 to be accessible as python
RUN ln -s /usr/bin/python3 /usr/bin/python

# Install required packages for Bootstrapper Tool
RUN pip install \
    pyyaml \
    requests \
    colorlog

# Install NASM
ENV NASM_VERSION='2.15.05'
ENV NASM_RPM="nasm-${NASM_VERSION}-0.fc31.x86_64.rpm"
ENV NASM_URL="https://www.nasm.us/pub/nasm/releasebuilds/${NASM_VERSION}/linux/${NASM_RPM}"
RUN wget --no-check-certificate ${NASM_URL} && \
    alien -i ${NASM_RPM} && \
    rm ${NASM_RPM}
   

WORKDIR ${WORKSPACE_DIR}
