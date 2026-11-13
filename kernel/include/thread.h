/*
 * Thread management services.
 * Copyright (c) 2026, Quinn Stephens.
 * All rights reserved.
 * Provided under the BSD 3-Clause license.
 */

#pragma once

#include <process.h>

typedef struct {
    ListItem listItem;
    const char8 *name;
    Process *parent;
} Thread;

Thread *threadCreate(Process *parent, const char8 *name);
