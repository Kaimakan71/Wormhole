# x86-64 kernel build configuration.
# Copyright (c) 2026, Quinn Stephens.
# All rights reserved.
# Provided under the BSD 3-Clause license.

export SFILES += $(addprefix $(ARCH_DIR)/kernel/,trap.S)
export CFILES += $(addprefix $(ARCH_DIR)/kernel/,trap.c gdt.c idt.c cpu.c)
