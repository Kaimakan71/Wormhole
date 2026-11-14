/*
 * Trap handling services.
 * Copyright (c) 2026, Quinn Stephens.
 * All rights reserved.
 * Provided under the BSD 3-Clause license.
 */

#include <console.h>
#include <cpu.h>
#include <cpu/state.h>
#include <cpu/trap.h>
#include <log.h>

static const char8 *types[] = {
    "[#DE] Divide Error",
    "[#DB] Debug Exception",
    "NMI Interrupt",
    "[#BP] Breakpoint",
    "[#OF] Overflow",
    "[#BR] BOUND Range Exceeded",
    "[#UD] Invalid Opcode",
    "[#NM] Device Not Available",
    "[#DF] Double Fault",
    "Coprocessor Segment Overrun",
    "[#TS] Invalid TSS",
    "[#NP] Segment Not Present",
    "[#SS] Stack-Segment Fault",
    "[#GP] General Protection",
    "[#PF] Page Fault",
    "Reserved",
    "[#MF] x87 FPU Floating-Point-Error",
    "[#AC] Alignment Check",
    "[#MC] Machine Check",
    "[#XM] SIMD Floating-Point Exception",
    "[#VE] Virtualization Exception",
    "[#CP] Control Protection Exception",
};

void trapException(CpuThreadState *state) {
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
        types[state->vector], state->errorCode,
        state->cs, state->rip,
        state->ss, state->rsp,
        state->rax, state->rbx, state->rcx, state->rdx,
        state->rsi, state->rdi, state->r8,  state->r9,
        state->r10, state->r11, state->r12, state->r13,
        state->r14, state->r15, state->rbp, state->rflags
    );
    cpuHang();
}
