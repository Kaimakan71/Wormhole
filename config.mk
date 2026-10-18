# Project build configuration.
# Copyright (c) 2026, Quinn Stephens.
# All rights reserved.
# Provided under the BSD 3-Clause license.

# Target options
export TARGET_ARCH = x86_64
export TARGET_FIRMWARE = efi

# Build directory options
export BUILD_DIR = $(abspath build/)

# Virtualization options
export CONFIG_QEMU_KVM = y
export CONFIG_QEMU_SERIAL = y
