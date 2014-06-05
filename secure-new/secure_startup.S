#include "asm-inc.h"

	.text
	.align 4
	.globl _start
_start:
    B _start_secure

	.align 4
	.global secure_exception_vectors
secure_exception_vectors:
	B	_reset_handler		@- can by used by secure
	B	_undefined_handler	@- can by used by secure
	B	_swi_handler		@- can by used by secure
	B	_prefetch_handler	@- can by used by secure
	B	_abort_handler		@- can by used by secure
	B	_reserved_vector	@- this is reserved for hypvisor mode
	B	_irq_handler		@- can by used by secure
	B	_fiq_handler		@- can by used by secure

_reset_handler:
	mov	r0, #0x0
	BL	secure_exception_handler
	B	_reset_handler

_undefined_handler:
	mov	r0, #0x4
	BL	secure_exception_handler
	B	_undefined_handler

_swi_handler:
	mov	r0, #0x8
	BL	secure_exception_handler
	B	_swi_handler

_prefetch_handler:
	mov	r0, #0xc
	BL	secure_exception_handler
	B	_prefetch_handler

_abort_handler:
	mov	r0, #0x10
	BL	secure_exception_handler
	B	_abort_handler

_reserved_vector:
	mov	r0, #0x14
	BL	secure_exception_handler
	B	_reserved_vector

_irq_handler:
	mov	r0, #0x18
	BL	secure_exception_handler
	B	_irq_handler

_fiq_handler:
	mov	r0, #0x1c
	BL	secure_exception_handler
	B	_fiq_handler

	.text
	.arm
	.align 4
	.global _start_secure
_start_secure:
	adr		ip, save_stack
	stmia	ip, {lr}
	
@ set secure world VBAR 
	ldr     r1, =secure_exception_vectors   @ Load base address into r0
	mcr     p15, 0, r1, c12, c0, 0          @ Write secure world VBAR

@ set MVBAR
	ldr     r1, =monitor_exception_vectors  @ Get address of Monitor's vector table
    mcr     p15, 0, r1, c12, c0, 1          @ Write Monitor Vector Base Address Registe

@ Initialize stacks for some modes
@ set FIQ stack
	cpsid   if, #cpsr_mode_fiq
	ldr     sp, =FIQ_MODE_STACK_BASE

@ set IRQ stack
	cpsid   if, #cpsr_mode_irq
	ldr     sp, =IRQ_MODE_STACK_BASE

@ set ABT stack
	cpsid   if, #cpsr_mode_abt
	ldr     sp, =ABT_MODE_STACK_BASE

@ set UND stack
	cpsid   if, #cpsr_mode_und
	ldr     sp, =UND_MODE_STACK_BASE

@ set user/system stack
	cpsid   if, #cpsr_mode_sys
	ldr     sp, =SYS_MODE_STACK_BASE

@ set monitor stack
	cpsid   if, #cpsr_mode_mon
	ldr     sp, =MON_MODE_STACK_BASE

@ set SVC stack
	cpsid   if, #cpsr_mode_svc
	ldr     sp, =SVC_MODE_STACK_BASE

@ bank System Control Register
	mrc     p15, 0, r0, c1, c0, 0
	bic     r0, r0, #(0x1  <<13)    /* base address 0x00000000 */
	bic     r0, r0, #(0x1  <<12)    /* disable I Cache */
	bic     r0, r0, #(0x1  <<2)     /* disable D Cache */
	bic     r0, r0, #(0x1  <<0)     /* disable MMU */
	orr     r0, r0, #(0x1  <<14)    /* enable RR */
	mcr     p15, 0, r0, c1, c0, 0
	
@ bank Auxillary control register
	mrc     p15, 0, r0, c1, c0, 1
	bic     r0, r0, #(0x1  <<6)     /* enable SMP bit */
	orr     r0, r0, #(0x1  <<0)     /* enable FW bit */
	mcr     p15, 0, r0, c1, c0, 1

/* Init .bss */
/* Clear the .bss section (zero init) */
    LDR     r1,=__bss_start
    LDR     r2,=__bss_end
    MOV     r3,#0
1:
    CMP     r1,r2
    STMLTIA r1!,{r3}
    BLT     1b

/* Branch to C Library entry point */
	bl 		target_init

	adr		ip, save_stack
	ldmia	ip, {lr}
	mov		r3, lr	/* save uboot next pc point to r3 */
	bl		start_uboot

/* We should never reach here */
    b   .

save_stack:
	.space 4*4