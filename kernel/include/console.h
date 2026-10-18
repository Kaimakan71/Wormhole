/*
 * Console services.
 * Copyright (c) 2026, Quinn Stephens.
 * All rights reserved.
 * Provided under the BSD 3-Clause license.
 */

#pragma once

#include <bootinfo.h>

void consoleInitialize(Framebuffer *fb);
void consoleSetColor(uint32 color);
void consoleResetCursor();
void consoleClear();
void consoleWrite(const char8 *s);
