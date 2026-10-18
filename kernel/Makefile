# Kernel build script.
# Copyright (c) 2026, Quinn Stephens.
# All rights reserved.
# Provided under the BSD 3-Clause license.

export BUILD_DIR := $(BUILD_DIR)/kernel

export ELF_NAME = kernel.elf

export CC = clang
export LD = ld.lld

export CFLAGS += -Iinclude -I../common/include -I$(ARCH_DIR)/include -target $(TOOLCHAIN_ARCH)-unknown-elf -mcmodel=kernel
export LDFLAGS += -T$(ARCH_DIR)/kernel/link.lds -zmax-page-size=0x1000

export SFILES =
export CFILES = ../common/string8.c console.c log.c main.c

.PHONY: all
all: $(BUILD_DIR)/$(ELF_NAME)

include ../subproj.mk
