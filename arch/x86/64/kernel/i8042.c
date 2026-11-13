/*
 * i8042 PS/2 controller driver.
 * Copyright (c) 2026, Quinn Stephens.
 * All rights reserved.
 * Provided under the BSD 3-Clause license.
 */

#include <console.h>
#include <cpu/idt.h>
#include <cpu/i8042.h>
#include <cpu/i8259.h>
#include <cpu/pmio.h>
#include <log.h>

#define TRACE_SOURCE "i8042"

static char8 i8042Keymap[128] = {
    '\0', '\0', '1',  '2',  '3',  '4',  '5',  '6',  '7',  '8',  '9',  '0',  '-',  '=',  '\0', '\0',
    'q',  'w',  'e',  'r',  't',  'y',  'u',  'i',  'o',  'p',  '[',  ']',  '\0', '\0', 'a',  's',
    'd',  'f',  'g',  'h',  'j',  'k',  'l',  ';',  '\'', '`',  '\0', '\\', 'z',  'x',  'c',  'v',
    'b',  'n',  'm',  ',',  '.',  '/',  '\0', '\0', '\0', ' ',  '\0', '\0', '\0', '\0', '\0', '\0',
    '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0',
    '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0',
    '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0',
    '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0', '\0',
};

static inline void i8042WaitRead() {
    while (!(pmioRead8(I8042_STATUS_PORT) & I8042_STATUS_OUTPUT_FULL));
}

static inline void i8042WaitWrite() {
    while (pmioRead8(I8042_STATUS_PORT) & I8042_STATUS_INPUT_FULL);
}

static inline void i8042Command(uint8 cmd) {
    i8042WaitWrite();
    pmioWrite8(I8042_COMMAND_PORT, cmd);
}

static void i8042Flush() {
    while (pmioRead8(I8042_STATUS_PORT) & I8042_STATUS_OUTPUT_FULL) {
        pmioRead8(I8042_DATA_PORT);
    }
}

static uint8 i8042ConfigRead() {
    i8042Command(I8042_COMMAND_READ_CONFIG);
    i8042WaitRead();
    return pmioRead8(I8042_DATA_PORT);
}

static void i8042ConfigWrite(uint8 data) {
    i8042Command(I8042_COMMAND_WRITE_CONFIG);
    i8042WaitWrite();
    pmioWrite8(I8042_DATA_PORT, data);
}

static __interrupt void i8042KeyboardIsr(void *trapframe) {
    uint8 data;
    char8 s[2];

    (void) trapframe;

    data = pmioRead8(I8042_DATA_PORT);
    if (data < 0x80) {
        s[0] = i8042Keymap[data];
        s[1] = '\0';
        consoleWrite(s);
    }

    i8259Eoi(I8042_KEYBOARD_IRQ);
}

void i8042Initialize() {
    uint8 config;

    traceDebug("Initializing PS/2 controller...\n");

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
    if (pmioRead8(I8042_DATA_PORT) != 0x55) {
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
}
