/*
 * Process scheduler.
 * Copyright (c) 2026, Quinn Stephens.
 * All rights reserved.
 * Provided under the BSD 3-Clause license.
 */

#include <status.h>
#include <thread.h>

Status schedInitialize();
Status schedStart();
Thread *schedSchedule(Thread *current);
