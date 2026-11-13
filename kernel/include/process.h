/*
 * Process management services.
 * Copyright (c) 2026, Quinn Stephens.
 * All rights reserved.
 * Provided under the BSD 3-Clause license.
 */

#pragma once

#include <list.h>
#include <types.h>

typedef struct {
    const char8 *name;
    List threads;
} Process;

Process *processCreate(const char8 *name);
