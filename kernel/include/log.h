/*
 * Logging services.
 * Copyright (c) 2026, Quinn Stephens.
 * All rights reserved.
 * Provided under the BSD 3-Clause license.
 */

#pragma once

#include <types.h>

void logWrite(const char8 *format, ...);
void logDebug(const char8 *format, ...);
void logInfo(const char8 *format, ...);
void logWarning(const char8 *format, ...);
void logError(const char8 *format, ...);

#define traceDebug(...)   logDebug(TRACE_SOURCE ": " __VA_ARGS__)
#define traceInfo(...)    logInfo(TRACE_SOURCE ": " __VA_ARGS__)
#define traceWarning(...) logWarning(TRACE_SOURCE ": " __VA_ARGS__)
#define traceError(...)   logError(TRACE_SOURCE ": " __VA_ARGS__)
