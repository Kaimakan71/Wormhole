/*
 * Thread management services.
 * Copyright (c) 2026, Quinn Stephens.
 * All rights reserved.
 * Provided under the BSD 3-Clause license.
 */

#pragma once

#include <cpu/state.h>
#include <process.h>

typedef struct Thread {
    ListItem listItem;
    ListItem schedListItem;

    uintn id;
    const char8 *name;
    Process *parent;

    void *stack;
    uintn stackSize;

    void *kernelStack;
    uintn kernelStackTop;

    CpuThreadState *cpuState;
} Thread;

Thread *threadCreate(Process *parent, const char8 *name, void *entry);
void threadDestroy(Thread *thread);

void threadArchInitialize(Thread *thread, void *entry);
Thread *threadArchGetCurrent();
void threadArchSetCurrent(Thread *thread);
