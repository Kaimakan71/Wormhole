# Project build script.
# Copyright (c) 2026, Quinn Stephens.
# All rights reserved.
# Provided under the BSD 3-Clause license.

include config.mk

export CFLAGS = -nostdinc -Wall -Wextra -MMD -O3 -ffreestanding -fno-stack-protector
export LDFLAGS =

ifeq ($(TARGET_ARCH),x86_64)
	include arch/x86/64/build.mk
endif

.PHONY: all
all:
	@$(MAKE) -C boot/

.PHONY: clean
clean:
	@$(MAKE) -C boot/ clean

.PHONY: run
run:
	@$(MAKE) -C boot/ run
