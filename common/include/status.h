/*
 * Status code definitions.
 * Copyright (c) 2026, Quinn Stephens.
 * All rights reserved.
 * Provided under the BSD 3-Clause license.
 */

#pragma once

#include <types.h>

typedef int32 Status;

#define STATUS_IS_SUCCESS(status) ((status) >= 0)

#define STATUS_SUCCESS           0
#define STATUS_UNKNOWN_ERROR     ((Status) 0x80000000)
#define STATUS_INVALID_ARGUMENT  ((Status) 0x80000001)
#define STATUS_UNSUPPORTED       ((Status) 0x80000002)
#define STATUS_ALLOCATION_FAILED ((Status) 0x80000003)
#define STATUS_FORMAT_ERROR      ((Status) 0x80000004)
#define STATUS_NOT_FOUND         ((Status) 0x80000005)
