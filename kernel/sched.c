/*
 * Process scheduler.
 * Copyright (c) 2026, Quinn Stephens.
 * All rights reserved.
 * Provided under the BSD 3-Clause license.
 */

#include <cpu.h>
#include <log.h>
#include <process.h>
#include <sched.h>

#define TRACE_SOURCE "sched"

Status schedInitialize() {
    Process *idleProcess;

    traceDebug("Initializing scheduler...\n");

    idleProcess = processCreate("idle");
    if (idleProcess == NULL) {
        traceError("Failed to create idle process\n");
        return STATUS_UNKNOWN_ERROR;
    }

    return STATUS_SUCCESS;
}
