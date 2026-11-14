/*
 * x86-64 scheduler services.
 * Copyright (c) 2026, Quinn Stephens.
 * All rights reserved.
 * Provided under the BSD 3-Clause license.
 */

#pragma once

#include <cpu/state.h>

void schedYield(CpuState *regs);
