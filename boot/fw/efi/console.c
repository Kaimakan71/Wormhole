/*
 * EFI console services.
 * Copyright (c) 2026, Quinn Stephens.
 * All rights reserved.
 * Provided under the BSD 3-Clause license.
 */

#include <string.h>
#include "bootefi.h"

#define DEFAULT_ATTRIBUTE EFI_TEXT_MAKE_ATTRIBUTE(EFI_COLOR_LIGHTGRAY, EFI_COLOR_BLACK)

static char16 efiConsoleBuffer[256];

void consolePrint(const char16 *format, ...) {
    VaList args;

    vaListStart(args, format);
    stringFormat16(efiConsoleBuffer, ARRAY_LENGTH(efiConsoleBuffer), format, args);
    vaListEnd(args);

    efiConsoleOut->outputString(efiConsoleOut, efiConsoleBuffer);
}

#define PrintPrefxed(tag, color) ({                                                              \
    VaList args;                                                                                 \
    vaListStart(args, format);                                                                   \
    stringFormat16(efiConsoleBuffer, ARRAY_LENGTH(efiConsoleBuffer), format, args);              \
    vaListEnd(args);                                                                             \
    efiConsoleOut->setAttribute(efiConsoleOut, EFI_TEXT_MAKE_ATTRIBUTE(color, EFI_COLOR_BLACK)); \
    efiConsoleOut->outputString(efiConsoleOut, tag);                                             \
    efiConsoleOut->setAttribute(efiConsoleOut, DEFAULT_ATTRIBUTE);                               \
    efiConsoleOut->outputString(efiConsoleOut, efiConsoleBuffer);                                \
})

void consoleDebug(const char16 *format, ...) {
    PrintPrefxed(L"[ debug ] ", EFI_COLOR_LIGHTGRAY);
}

void consoleInfo(const char16 *format, ...) {
    PrintPrefxed(L"[ info ] ", EFI_COLOR_LIGHTCYAN);
}

void consoleWarning(const char16 *format, ...) {
    PrintPrefxed(L"[ warning ] ", EFI_COLOR_YELLOW);
}

void consoleError(const char16 *format, ...) {
    PrintPrefxed(L"[ error ] ", EFI_COLOR_LIGHTRED);
}
