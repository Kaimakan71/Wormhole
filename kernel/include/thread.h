/*
 * Thread management services.
 * Copyright (c) 2026, Quinn Stephens.
 * All rights reserved.
 * Provided under the BSD 3-Clause license.
 */

#pragma once

#include <process.h>

#define THREAD_FLAG_KERNEL BIT(0)

typedef struct CpuThreadState CpuThreadState;

typedef struct Thread {
    ListItem listItem;
    ListItem schedListItem;

    uintn id;
    const char8 *name;
    uint8 flags;
    Process *parent;

    void *stack;
    uintn stackSize;

    void *kernelStack;
    uintn kernelStackTop;

    CpuThreadState *cpuState;
} Thread;

Thread *threadCreate(Process *parent, const char8 *name, uint8 flags, void *entry);
void threadDestroy(Thread *thread);

void threadArchInitialize(Thread *thread, void *entry);
Thread *threadArchGetCurrent();
void threadArchSetCurrent(Thread *thread);
__noreturn void threadArchSwitchTo(Thread *thread);
