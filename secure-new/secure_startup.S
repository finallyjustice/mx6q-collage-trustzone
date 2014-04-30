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
	cpsid if, #cpsr_mode_mon
	ldr sp, =0x00908000
	mov fp, #0	
	
	bl monitor_init
	  	
	cpsid if, #cpsr_mode_svc
	ldr sp, =0x00909000
	
	@disable cache & table type
    mrc p15, 0, r0, c1, c0, 0
    bic r0, r0, #0x00004	@disable data cache
    bic r0, r0, #0x01000	@disable instruction cache
    mcr p15, 0, r0, c1, c0, 0

    @invalidate TLBs
    mov r0, #0x0
    mcr p15, 0, r0, c8, c7, 0
	
	bl secure_main