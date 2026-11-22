/*
 * Trap handling services.
 * Copyright (c) 2026, Quinn Stephens.
 * All rights reserved.
 * Provided under the BSD 3-Clause license.
 */

#include <console.h>
#include <cpu.h>
#include <cpu/state.h>
#include <log.h>

static const char8 *trapStrings[] = {
    "[#DE] Divide error",
    "[#DB] Debug exception",
    "Non-Maskable Interrupt",
    "[#BP] Breakpoint",
    "[#OF] Overflow",
    "[#BR] BOUND range exceeded",
    "[#UD] Invalid opcode",
    "[#NM] Device not available",
    "[#DF] Double fault",
    "Coprocessor segment overrun",
    "[#TS] Invalid TSS",
    "[#NP] Segment not present",
    "[#SS] Stack-segment fault",
    "[#GP] General protection fault",
    "[#PF] Page fault",
    "Reserved",
    "[#MF] x87 Floating-Point Unit error",
    "[#AC] Alignment check",
    "[#MC] Machine check",
    "[#XM] SIMD floating-point exception",
    "[#VE] Virtualization exception",
    "[#CP] Control protection exception",
};

void trapException(CpuThreadState *cpuState) {
    consoleReset(false);
    logError("Processor exception detected!\n");
    logWrite(
        "%s (code=0x%x)\n"
        "CS:RIP=0x%x:0x%x\n"
        "SS:RSP=0x%x:0x%x\n"
        "   RAX=0x%x RBX=0x%x RCX=0x%x    RDX=0x%x\n"
        "   RSI=0x%x RDI=0x%x  R8=0x%x     R9=0x%x\n"
        "   R10=0x%x R11=0x%x R12=0x%x    R13=0x%x\n"
        "   R14=0x%x R15=0x%x RBP=0x%x RFLAGS=0x%x\n",
        trapStrings[cpuState->vector], cpuState->errorCode,
        cpuState->cs, cpuState->rip,
        cpuState->ss, cpuState->rsp,
        cpuState->rax, cpuState->rbx, cpuState->rcx, cpuState->rdx,
        cpuState->rsi, cpuState->rdi, cpuState->r8,  cpuState->r9,
        cpuState->r10, cpuState->r11, cpuState->r12, cpuState->r13,
        cpuState->r14, cpuState->r15, cpuState->rbp, cpuState->rflags
    );

    cpuHang();
}
