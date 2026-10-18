# Boot manager build script.
# Copyright (c) 2026, Quinn Stephens.
# All rights reserved.
# Provided under the BSD 3-Clause license.

export BUILD_DIR := $(BUILD_DIR)/boot

export QEMU = qemu-system-$(TOOLCHAIN_ARCH)

export CFLAGS += -Iinclude -I../common/include -I$(ARCH_DIR)/include
export QEMUFLAGS = -d int,cpu_reset

export SFILES =
export CFILES = $(addprefix ../common/,string16.c tlsf.c) $(ARCH_DIR)/boot/mm.c

ifeq ($(TARGET_FIRMWARE),efi)
	include fw/efi/build.mk
endif

export CFILES += $(addprefix mm/,heap.c mm.c) image.c main.c

ifeq ($(CONFIG_QEMU_KVM),y)
	export QEMUFLAGS += --enable-kvm
endif

ifeq ($(CONFIG_QEMU_SERIAL),y)
	export QEMUFLAGS += -serial stdio
endif

.PHONY: all
all: $(BUILD_DIR)/$(EXE_NAME)

.PHONY: run
run:
	@mkdir -p disk/EFI/Boot
	@cp $(BUILD_DIR)/$(EXE_NAME) disk/EFI/Boot/bootx64.efi
	@cp $(BUILD_DIR)/../kernel/kernel.elf disk/kernel.elf
	@$(QEMU) $(QEMUFLAGS)

include ../subproj.mk
