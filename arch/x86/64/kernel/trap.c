/*
 * Trap handling services.
 * Copyright (c) 2026, Quinn Stephens.
 * All rights reserved.
 * Provided under the BSD 3-Clause license.
 */

#include <console.h>
#include <cpu.h>
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

void trapException(Trapframe *tf) {
    consoleResetCursor();
    logError("Processor exception detected!\n");
    logWrite(
        "%s (code=0x%x)\n"
        "CS:RIP=0x%x:0x%x\n"
        "SS:RSP=0x%x:0x%x\n"
        "   RAX=0x%x RBX=0x%x RCX=0x%x    RDX=0x%x\n"
        "   RSI=0x%x RDI=0x%x  R8=0x%x     R9=0x%x\n"
        "   R10=0x%x R11=0x%x R12=0x%x    R13=0x%x\n"
        "   R14=0x%x R15=0x%x RBP=0x%x RFLAGS=0x%x\n",
        types[tf->vector], tf->errorCode,
        tf->cs, tf->rip,
        tf->ss, tf->rsp,
        tf->rax, tf->rbx, tf->rcx, tf->rdx,
        tf->rsi, tf->rdi, tf->r8,  tf->r9,
        tf->r10, tf->r11, tf->r12, tf->r13,
        tf->r14, tf->r15, tf->rbp, tf->rflags
    );
    cpuHang();
}
