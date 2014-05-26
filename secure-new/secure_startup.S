#define cpsr_mode_use	(0x10)
#define cpsr_mode_fiq	(0x11)
#define cpsr_mode_irq	(0x12)
#define cpsr_mode_svc	(0x13)
#define cpsr_mode_mon	(0x16)
#define cpsr_mode_abt	(0x17)
#define cpsr_mode_und	(0x1b)
#define cpsr_mode_sys	(0x1F)

#define cpsr_irq_bit	(0x80)
#define cpsr_fiq_bit	(0x40)
#define cpsr_asyc_bit	(0x100)

	.text
	.arm
	.align 5
	.global _start_secure
_start_secure:
	cpsid   if, #cpsr_mode_mon
	ldr     sp, =0x00908000
	mov     fp, #0	

	cpsid   if, #cpsr_mode_svc
	ldr     sp, =0x00909000

	bl  Init_aips
	bl  Init_clock

	@disable cache mmu & table type
    mrc     p15, 0, r0, c1, c0, 0
    bic     r0, r0, #(0x1  <<13)    /* Normal exception vectors, base address 0x00000000 */
    bic     r0, r0, #(0x1  <<12)    /* disable I Cache */
    bic     r0, r0, #(0x1  <<2)     /* disable D Cache */
    bic     r0, r0, #(0x1  <<0)     /* disable MMU */
	orr     r0, r0, #(0x1  <<14)    /* enable RR */
    mcr     p15, 0, r0, c1, c0, 0

    @invalidate TLBs
    mov     r0, #0x0
    mcr     p15, 0, r0, c8, c7, 0
	
	@ Invalidate TLBs
    mov     r1, #0x0
    mcr     p15, 0, r1, c8, c3, 0

	@Auxillary control register
	mrc     p15, 0, r0, c1, c0, 1
	ldr     r1, =0x41
	orr     r0, r0, r1
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
	bl target_init

	bl start_uboot

Init_aips:	
	ldr r0, =0x0207C000
	ldr r1, =0x77777777
	str r1, [r0, #0x0]
	str r1, [r0, #0x4]
	ldr r1, =0x0
	str r1, [r0, #0x40]
	str r1, [r0, #0x44]
	str r1, [r0, #0x48]
	str r1, [r0, #0x4C]
	str r1, [r0, #0x50]

	ldr r0, =0x0217C000
	ldr r1, =0x77777777
	str r1, [r0, #0x0]
	str r1, [r0, #0x4]
	ldr r1, =0x0
	str r1, [r0, #0x40]
	str r1, [r0, #0x44]
	str r1, [r0, #0x48]
	str r1, [r0, #0x4C]
	str r1, [r0, #0x50]
	bx lr

Init_clock:
	/* Restore the default values in the Gate registers */
	ldr r0, =0x020c4000
	ldr r1, =0xC0003F
	str r1, [r0, #0x68]
	ldr r1, =0x30FC00
	str r1, [r0, #0x6c]
	ldr r1, =0xFFFC000
	str r1, [r0, #0x70]
	ldr r1, =0x3FF00000
	str r1, [r0, #0x74]
	ldr r1, =0xFFF300
	str r1, [r0, #0x78]
	ldr r1, =0xF0000C3
	str r1, [r0, #0x7c]
	ldr r1, =0x3FC
	str r1, [r0, #0x80]
	bx lr	