/*
 * 8-bit string services.
 * Copyright (c) 2026, Quinn Stephens.
 * All rights reserved.
 * Provided under the BSD 3-Clause license.
 */

#include <string.h>

static uintn writeHexadecimal(char8 *buffer, uintn bufferLength, uintn value, uint8 bits) {
    uintn rem;
    uint8 digit;

    rem = bufferLength;
    for (int8 s = bits - 4; s >= 0 && rem; s -= 4) {
        digit = (value >> s) & 0xf;

        if (digit >= 0xa) {
            *buffer++ = 'a' + (digit - 0xa);
        } else {
            *buffer++ = '0' + digit;
        }

        rem--;
    }

    return bufferLength - rem;
}

static uintn writeDecimal(char8 *buffer, uintn bufferLength, uintn value, uint8 bits) {
    uintn div, rem;
    bool pad;
    uint8 digit;

    if (bits >= 64) {
        div = 10000000000000000000ULL;
    } else if (bits >= 32) {
        div = 1000000000;
    } else if (bits >= 16) {
        div = 10000;
    } else {
        div = 100;
    }

    rem = bufferLength;
    pad = true;
    while (div > 0 && rem) {
        digit = value / div;
        value %= div;

        if (digit != 0) {
            pad = false;
        }

        if (!pad || div == 1) {
            *buffer++ = '0' + digit;
            rem--;
        }

        div /= 10;
    }

    return bufferLength - rem;
}

static uintn writeString(char8 *buffer, uintn bufferLength, const char8 *string) {
    uintn rem;

    rem = bufferLength;
    while (*string != '\0' && rem) {
        *buffer++ = *string++;
        rem--;
    }

    return bufferLength - rem;
}

uintn stringFormat8(char8 *buffer, uintn bufferLength, const char8 *format, VaList arguments) {
    uintn rem, len;
    uint8 bits;

    rem = bufferLength - 1;
    while (*format != '\0' && rem) {
        if (*format != '%') {
            *buffer++ = *format++;
            rem--;
            continue;
        }

        /* Parse size specifier */
        format++;
        bits = 0;
        while (*format >= '0' && *format <= '9') {
            bits *= 10;
            bits += *format - '0';
            format++;
        }

        if (bits % 4) {
            bits -= bits % 4;
        }

        if (bits > 64) {
            bits = 64;
        } else if (bits == 0) {
            bits = 8 * sizeof(uintn);
        }

        switch (*format) {
        case 'x':
            format++;
            len = writeHexadecimal(buffer, rem, vaListPop(arguments, uintn), bits);
            buffer += len;
            rem -= len;
            break;
        case 'u':
            format++;
            len = writeDecimal(buffer, rem, vaListPop(arguments, uintn), bits);
            buffer += len;
            rem -= len;
            break;
        case 's':
            format++;
            len = writeString(buffer, rem, vaListPop(arguments, const char8 *));
            buffer += len;
            rem -= len;
            break;
        case 'c':
            format++;
            *buffer++ = (char8) vaListPop(arguments, int32);
            rem--;
            break;
        case '%':
            *buffer++ = *format++;
            rem--;
            break;
        default:
            break;
        }
    }

    *buffer = '\0';

    return bufferLength - rem;
}
