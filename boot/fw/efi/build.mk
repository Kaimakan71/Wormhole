# EFI boot manager build configuration.
# Copyright (c) 2026, Quinn Stephens.
# All rights reserved.
# Provided under the BSD 3-Clause license.

export EXE_NAME = boot.efi

export CC = clang
export LD = lld-link

export CFLAGS += -target $(TOOLCHAIN_ARCH)-windows-unknown -fshort-wchar
export LDFLAGS += -nodefaultlib -subsystem:efi_application -entry:efiEntry
export QEMUFLAGS += -L . -bios OVMF.fd -hda fat:rw:disk

export CFILES += $(addprefix fw/efi/,status.c console.c protocol.c mm.c fs.c entry.c)
