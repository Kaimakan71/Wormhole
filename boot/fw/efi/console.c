/*
 * EFI console services.
 * Copyright (c) 2026, Quinn Stephens.
 * All rights reserved.
 * Provided under the BSD 3-Clause license.
 */

#include <string.h>
#include "bootefi.h"

#define DEFAULT_ATTRIBUTE EFI_TEXT_MAKE_ATTRIBUTE(EFI_COLOR_LIGHTGRAY, EFI_COLOR_BLACK)

static char16 buffer[256];

void consolePrint(const char16 *format, ...) {
    VaList args;

    vaListStart(args, format);
    stringFormat16(buffer, ARRAY_LENGTH(buffer), format, args);
    vaListEnd(args);

    efiConsoleOut->outputString(efiConsoleOut, buffer);
}

#define DO_PRINT(tag, color) ({                                                                  \
    VaList args;                                                                                 \
    vaListStart(args, format);                                                                   \
    stringFormat16(buffer, ARRAY_LENGTH(buffer), format, args);                                  \
    vaListEnd(args);                                                                             \
    efiConsoleOut->setAttribute(efiConsoleOut, EFI_TEXT_MAKE_ATTRIBUTE(color, EFI_COLOR_BLACK)); \
    efiConsoleOut->outputString(efiConsoleOut, tag);                                             \
    efiConsoleOut->setAttribute(efiConsoleOut, DEFAULT_ATTRIBUTE);                               \
    efiConsoleOut->outputString(efiConsoleOut, buffer);                                          \
})

void consoleDebug(const char16 *format, ...) {
    DO_PRINT(L"[ debug ] ", EFI_COLOR_LIGHTGRAY);
}

void consoleInfo(const char16 *format, ...) {
    DO_PRINT(L"[ info ] ", EFI_COLOR_LIGHTCYAN);
}

void consoleWarning(const char16 *format, ...) {
    DO_PRINT(L"[ warning ] ", EFI_COLOR_YELLOW);
}

void consoleError(const char16 *format, ...) {
    DO_PRINT(L"[ error ] ", EFI_COLOR_LIGHTRED);
}
