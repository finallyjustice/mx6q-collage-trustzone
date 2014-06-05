#include "asm-inc.h"

@rt is temporary register
.macro scr_nsbit_set rt
	mrc     p15, 0, \rt, c1, c1, 0  @ Read Secure Configuration Register
	orr	\rt, \rt, #SCR_NS_BIT
	mcr     p15, 0, \rt, c1, c1, 0  @ Write Secure Configuration Register
.endm

.macro scr_nsbit_clear rt
	mrc     p15, 0, \rt, c1, c1, 0  @ Read Secure Configuration Register
	bic	\rt, \rt, #SCR_NS_BIT
	mcr     p15, 0, \rt, c1, c1, 0  @ Write Secure Configuration Register
.endm	

.macro GET_CPU_ID rt
	mrc     p15, 0, \rt, c0, c0, 5   @ Read CPU ID register
	and     \rt, \rt, #0x03           @ Mask off, leaving the CPU ID field
.endm

@ ------------------------------------------------------------
@ Monitor mode vector table
@ ------------------------------------------------------------
	.section .vector, "ax"
	.global monitor_exception_vectors
	.align 5
monitor_exception_vectors:
	@ Monitor's
	NOP		@ Reset				@- not used by Monitor
	NOP		@ Undef				@- not used by Monitor
	B		SMC_Handler
	B		_prefetch_handler	@- can by used by Monitor
	B		_abort_handler		@- can by used by Monitor
	NOP		@ RESERVED			@- this is reserved for hypvisor mode
	B		_irq_handler		@- can by used by Monitor
	B		_fiq_handler		@- can by used by Monitor

_prefetch_handler:
	mov	r0, #0xc
	BL	monitor_exception_handler
	B	_prefetch_handler

_abort_handler:
	mov	r0, #0x10
	BL	monitor_exception_handler
	B	_abort_handler

_irq_handler:
	mov	r0, #0x18
	BL	monitor_exception_handler
	B	_irq_handler

_fiq_handler:
	mov	r0, #0x1c
	BL	monitor_exception_handler
	B	_fiq_handler

	.align 5
	.global SMC_Handler
SMC_Handler:
	cmp	r0,	#SMCID_SWITCH_TO_NS
	beq	call_non_secure_world

	cmp	r0,	#SMCID_SWITCH_TO_S
	beq	call_secure_world

	@cmp r0, #SMCID_NS_INVOKE_S
	@beq	call_ns_invoke_s
	b	call_ns_invoke_s

	bx  lr
	
	.align 5
	.global start_uboot
start_uboot:
@prepare for uboot context
	mov 	lr, r3
	ldr 	r4, =NS_STACK_BASE
	
	mov 	r0, #0
	mov 	r1, #0
	mov 	r2, #0
	mov 	r3, #0
	stmia	r4!, {r0-r3}
	mov     r0, r4

	mov 	r4, #0
	mov 	r5, #0
	mov 	r6, #0
	mov 	r7, #0
	mov 	r8, #0
	mov 	r9, #0
	mov 	r10, #0
	mov 	r11, #0
	mov 	r12, #0
	stmia   r0!, {r4-r12}

	ldr 	r1, =(cpsr_mode_svc | cpsr_irq_bit | cpsr_fiq_bit)
    stmia   r0!, {r1, lr}
	
@invoke smc
	ldr		r0, =SMCID_SWITCH_TO_NS
	smc		#0
	
	bl secure_loop
	
@	srsdb sp!, #cpsr_mode_mon
@	rfeia sp!
@	stmdb sp!, {r4-r12, lr}
@	ldmia sp!, {r4-r12, pc}^

@ r0 points to the base address where the register context get saved
.func save_context
.global save_context
save_context:	
    @ Save general purpose registers, SPSR and LR
    @ --------------------------------------------
	
    mov     r4, r0                      
    pop     {r0 - r3}                   @ Retrieve from stack
    stmia   r4!, {r0 - r3}              @ r0 - r3
    mov     r0, r4

    mov     r2, lr                      @ Save return path
	
    pop     {r4, lr}                    @ Retrieve from stack
	
    stmia   r0!, {r4-r12}               @ r4 to r12
	
    mrs     r1, spsr                    @ SPSR, LR
    stmia   r0!, {r1, lr}

    mov     lr, r2                      @ restore return path 
	
    @ Save banked registers
    cps #cpsr_mode_svc	
    mrs     r1, spsr
    stmia   r0!, {r1, r13, lr}

    cps #cpsr_mode_sys	
    stmia   r0!, {r13, lr}

    cps #cpsr_mode_abt	
    mrs     r1, spsr
    stmia   r0!, {r1, r13, lr}

    cps #cpsr_mode_und	
    mrs     r1, spsr
    stmia   r0!, {r1, r13, lr}

    cps #cpsr_mode_irq	
    mrs     r1, spsr
    stmia   r0!, {r1, r13, lr}

    cps #cpsr_mode_mon
    mov    pc, lr

.endfunc

@ r0 points to the base address from which the register context get restored
.func restore_context
.global restore_context
restore_context:	
    @ Restore other world's registers, SPSR and LR
    @ ---------------------------------------------
    push    {lr}

    mov	r4, r0                         @r0 - r3
    ldmia r4!, {r0 - r3}
	
    push {r0, r1}
    mov r0, r4

    ldmia r0!, {r4 - r12}              @r4 - r12
	
    ldmia r0!, {r1, lr}                @spsr, lr
    msr	spsr_cxsf, r1
	
    @Restore banked registers
    cps #cpsr_mode_svc	
    ldmia   r0!, {r1, r13, lr} 
    msr spsr_cxsf, r1

    cps #cpsr_mode_sys	
    ldmia   r0!, {r13, lr} 

    cps #cpsr_mode_abt	
    ldmia   r0!, {r1, r13, lr} 
    msr spsr_cxsf, r1

    cps #cpsr_mode_und	
    ldmia   r0!, {r1, r13, lr} 
    msr spsr_cxsf, r1	

    cps #cpsr_mode_irq	
    ldmia   r0!, {r1, r13, lr} 
    msr spsr_cxsf, r1

    cps #cpsr_mode_mon	
    pop     {r0, r1}
    ldr    pc, [r13], #4
.endfunc

call_non_secure_world:
	push	{r4, lr}
	push	{r0-r3}
	b mon_switchto_nsworld

call_secure_world:
	push	{r4, lr}
	push	{r0-r3}
	b mon_switchto_sworld

call_ns_invoke_s:
	push	{r4, lr}
	push	{r0-r3}

	/* Copy args to params stack */
    ldr     r4, =params_stack
    stmia   r4, {r0-r3}

    b mon_switchto_sworld
	

.func  mon_switchto_nsworld
mon_switchto_nsworld:
    ldr	r0, =S_STACK_BASE
    bl    save_context

    ldr	r0, =NS_STACK_BASE
    bl    restore_context
    
    @ Clear local monitor
    @ --------------------
    clrex

    push {r0}
    @Move to Non Secure
    scr_nsbit_set r0
    pop  {r0}

	dsb
	isb

    movs    pc, lr
.endfunc

.func  mon_switchto_sworld
mon_switchto_sworld:
    @Move to Secure
    scr_nsbit_clear r0
	
    ldr	r0, =NS_STACK_BASE
    bl    save_context

    ldr	r0, =S_STACK_BASE
    bl    restore_context
    @ Clear local monitor
    @ --------------------
    clrex                         

    movs    pc, lr
.endfunc
	
@ ------------------------------------------------------------
@ Space reserved for stacks
@ ------------------------------------------------------------  
NS_STACK_BASE:
  .space	4*256
NS_STACK_LIMIT:

S_STACK_BASE:
  .space    4*256
S_STACK_LIMIT:

NS_STACK_SP:
  .word     0

S_STACK_SP:
  .word     0

@ ------------------------------------------------------------
@ End
@ ------------------------------------------------------------	
