# x86-64 project build configuration.
# Copyright (c) 2026, Quinn Stephens.
# All rights reserved.
# Provided under the BSD 3-Clause license.

export TOOLCHAIN_ARCH = x86_64

export ARCH_DIR = $(abspath arch/x86/64/)

export CFLAGS += -mno-red-zone -mno-sse -mno-sse2 -mno-mmx -mno-avx -mno-80387 -DCONFIG_X86=1 -DCONFIG_64BIT=1 -DCONFIG_X86_64=1
