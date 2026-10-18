/*
 * Logging services.
 * Copyright (c) 2026, Quinn Stephens.
 * All rights reserved.
 * Provided under the BSD 3-Clause license.
 */

#include <console.h>
#include <log.h>
#include <string.h>

#define NORMAL_COLOR  0xaaaaaa
#define DEBUG_COLOR   0xaaaaaa
#define INFO_COLOR    0x00ffff
#define WARNING_COLOR 0xffff55
#define ERROR_COLOR   0xff5555

static char8 buffer[1024];

void logWrite(const char8 *format, ...) {
    VaList args;

    vaListStart(args, format);
    stringFormat8(buffer, ARRAY_LENGTH(buffer), format, args);
    vaListEnd(args);

    consoleWrite(buffer);
}

#define DO_PRINT(tag, color) ({                                \
    VaList args;                                               \
    vaListStart(args, format);                                 \
    stringFormat8(buffer, ARRAY_LENGTH(buffer), format, args); \
    vaListEnd(args);                                           \
    consoleSetColor(color);                                    \
    consoleWrite(tag);                                         \
    consoleSetColor(NORMAL_COLOR);                             \
    consoleWrite(buffer);                                      \
})

void logDebug(const char8 *format, ...) {
    DO_PRINT("[ debug ] ", DEBUG_COLOR);
}

void logInfo(const char8 *format, ...) {
    DO_PRINT("[ info ] ", INFO_COLOR);
}

void logWarning(const char8 *format, ...) {
    DO_PRINT("[ warning ] ", WARNING_COLOR);
}

void logError(const char8 *format, ...) {
    DO_PRINT("[ error ] ", ERROR_COLOR);
}
