/*
 * Process scheduler.
 * Copyright (c) 2026, Quinn Stephens.
 * All rights reserved.
 * Provided under the BSD 3-Clause license.
 */

#include <process.h>
#include <status.h>

extern Process *schedIdleProcess;

Status schedInitialize();
__noreturn void schedStart();
