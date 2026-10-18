/*
 * String services.
 * Copyright (c) 2026, Quinn Stephens.
 * All rights reserved.
 * Provided under the BSD 3-Clause license.
 */

#pragma once

#include <types.h>

uintn stringFormat8(char8 *buffer, uintn bufferLength, const char8 *format, VaList arguments);
uintn stringFormat16(char16 *buffer, uintn bufferLength, const char16 *format, VaList arguments);
