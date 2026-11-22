/*
 * i8042 PS/2 controller driver.
 * Copyright (c) 2026, Quinn Stephens.
 * All rights reserved.
 * Provided under the BSD 3-Clause license.
 */

#include <cpu/idt.h>
#include <cpu/i8042.h>
#include <cpu/i8259.h>
#include <cpu/pmio.h>
#include <log.h>

#define TRACE_SOURCE "i8042"

#define I8042_SCANCODE_LEFT_SHIFT_PRESSED  0x2a
#define I8042_SCANCODE_RIGHT_SHIFT_PRESSED 0x36

#define I8042_SCANCODE_LEFT_SHIFT_RELEASED  0xaa
#define I8042_SCANCODE_RIGHT_SHIFT_RELEASED 0xb6

static const char8 i8042Keymap[64] = {
    '\0', '\e', '1',  '2',  '3',  '4',  '5',  '6',  '7',  '8',  '9',  '0',  '-',  '=',  '\b', '\t',
    'q',  'w',  'e',  'r',  't',  'y',  'u',  'i',  'o',  'p',  '[',  ']',  '\n', '\0', 'a',  's',
    'd',  'f',  'g',  'h',  'j',  'k',  'l',  ';',  '\'', '`',  '\0', '\\', 'z',  'x',  'c',  'v',
    'b',  'n',  'm',  ',',  '.',  '/',  '\0', '\0', '\0', ' ',  '\0', '\0', '\0', '\0', '\0', '\0',
};

static const char8 i8042KeymapShift[64] = {
    '\0', '\e', '!',  '@',  '#',  '$',  '%',  '^',  '&',  '*',  '(',  ')',  '_',  '+',  '\b', '\t',
    'Q',  'W',  'E',  'R',  'T',  'Y',  'U',  'I',  'O',  'P',  '{',  '}',  '\n', '\0', 'A',  'S',
    'D',  'F',  'G',  'H',  'J',  'K',  'L',  ':',  '"',  '~',  '\0', '|',  'Z',  'X',  'C',  'V',
    'B',  'N',  'M',  '<',  '>',  '?',  '\0', '\0', '\0', ' ',  '\0', '\0', '\0', '\0', '\0', '\0',
};

static bool i8042ShiftPressed;

static inline void i8042WaitRead() {
    while (!(inb(I8042_STATUS_PORT) & I8042_STATUS_OUTPUT_FULL));
}

static inline void i8042WaitWrite() {
    while (inb(I8042_STATUS_PORT) & I8042_STATUS_INPUT_FULL);
}

static inline void i8042Command(uint8 cmd) {
    i8042WaitWrite();
    outb(I8042_COMMAND_PORT, cmd);
}

static void i8042Flush() {
    while (inb(I8042_STATUS_PORT) & I8042_STATUS_OUTPUT_FULL) {
        inb(I8042_DATA_PORT);
    }
}

static uint8 i8042ConfigRead() {
    i8042Command(I8042_COMMAND_READ_CONFIG);
    i8042WaitRead();
    return inb(I8042_DATA_PORT);
}

static void i8042ConfigWrite(uint8 data) {
    i8042Command(I8042_COMMAND_WRITE_CONFIG);
    i8042WaitWrite();
    outb(I8042_DATA_PORT, data);
}

static void i8042DefaultCallback(char8 ch) {
    (void) ch;
}

static __interrupt void i8042KeyboardIsr(void *trapframe) {
    uint8 data;
    char8 ch;

    (void) trapframe;

    /* TODO: Defer scancode processing */

    data = inb(I8042_DATA_PORT);
    if (data == I8042_SCANCODE_LEFT_SHIFT_PRESSED || data == I8042_SCANCODE_RIGHT_SHIFT_PRESSED) {
        i8042ShiftPressed = true;
    } else if (data == I8042_SCANCODE_LEFT_SHIFT_RELEASED || data == I8042_SCANCODE_RIGHT_SHIFT_RELEASED) {
        i8042ShiftPressed = false;
    } else if (data < ARRAY_LENGTH(i8042Keymap)) {
        if (i8042ShiftPressed) {
            ch = i8042KeymapShift[data];
        } else {
            ch = i8042Keymap[data];
        }

        i8042KeyboardDevice.onKeyPress(ch);
    }

    i8259Eoi(I8042_KEYBOARD_IRQ);
}

void i8042Initialize() {
    uint8 config;

    traceDebug("Initializing PS/2 controller...\n");

    i8042ShiftPressed = false;

    /* Disable ports and flush old data */
    i8042Command(I8042_COMMAND_DISABLE_PORT_A);
    i8042Command(I8042_COMMAND_DISABLE_PORT_B);
    i8042Flush();

    /* Disable interrupts */
    config = i8042ConfigRead();
    CLEAR_BIT(config, I8042_CONFIG_INT_ENABLE_A);
    CLEAR_BIT(config, I8042_CONFIG_INT_ENABLE_B);
    i8042ConfigWrite(config);

    /* Perform controller self-test */
    i8042Command(I8042_COMMAND_SELF_TEST);
    i8042WaitRead();
    if (inb(I8042_DATA_PORT) != 0x55) {
        traceError("Self-test failed\n");
        return;
    }

    /* Enable ports */
    i8042Command(I8042_COMMAND_ENABLE_PORT_A);
    i8042Command(I8042_COMMAND_ENABLE_PORT_B);

    /* Register ISR */
    idtSetVector(I8042_KEYBOARD_IRQ + IDT_IRQ_OFFSET, (uint64) i8042KeyboardIsr, 0, IDT_ENTRY_TYPE_INTERRUPT, 0);
    i8259Unmask(I8042_KEYBOARD_IRQ);

    /* Enable interrupts */
    config = i8042ConfigRead();
    SET_BIT(config, I8042_CONFIG_INT_ENABLE_A);
    SET_BIT(config, I8042_CONFIG_INT_ENABLE_B);
    i8042ConfigWrite(config);

    /* Register keyboard device */
    deviceRegister(&i8042KeyboardDevice.device);
}

KeyboardDevice i8042KeyboardDevice = {
    .device = {
        .name  = "i8042 PS/2 Keyboard",
        .class = &deviceClassKeyboard,
    },
    .onKeyPress = i8042DefaultCallback,
};
