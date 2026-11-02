/*
 * Console services.
 * Copyright (c) 2026, Quinn Stephens.
 * All rights reserved.
 * Provided under the BSD 3-Clause license.
 */

#pragma once

#include <bootinfo.h>

void consoleInitialize(Framebuffer *fb);
void consoleReset(bool clear);
void consoleSetColor(uint32 color);
void consoleWrite(const char8 *s);
