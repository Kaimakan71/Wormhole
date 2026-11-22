/*
 * Process scheduler.
 * Copyright (c) 2026, Quinn Stephens.
 * All rights reserved.
 * Provided under the BSD 3-Clause license.
 */

#include <status.h>
#include <thread.h>

Status schedInitialize();
void schedRegisterThread(Thread *thread);
void schedRegisterProcess(Process *process);
__noreturn void schedStart();

Status schedThreadExit(Thread *thread);
