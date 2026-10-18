/*
 * Internal boot memory management services.
 * Copyright (c) 2026, Quinn Stephens.
 * All rights reserved.
 * Provided under the BSD 3-Clause license.
 */

#pragma once

#include <status.h>

Status mmArchInitialize();
uintn mmArchGetStack();
void mmArchShutdown();

Status mmHeapInitialize(uintn heapSize);
void mmHeapShutdown();

Status mmFwInitialize();
void mmFwShutdown();
