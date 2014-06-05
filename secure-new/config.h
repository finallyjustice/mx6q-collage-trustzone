typedef unsigned long  u32;
typedef unsigned short u16;
typedef unsigned char  u8;

#define __REG(x)        (*((volatile u32 *)(x)))
#define __REG16(x)      (*((volatile u16 *)(x)))
#define __REG8(x)       (*((volatile u8 *)(x)))

#define REG_RD(base, reg) \
	(*(volatile unsigned int *)((base) + (reg)))
#define REG_WR(base, reg, value) \
	((*(volatile unsigned int *)((base) + (reg))) = (value))

// CPU init and prepare for uart
#define ATZ1_BASE_ADDR              0x02000000
#define AIPS1_OFF_BASE_ADDR         (ATZ1_BASE_ADDR + 0x80000)
#define ANATOP_BASE_ADDR            (AIPS1_OFF_BASE_ADDR + 0x48000)
#define IOMUXC_BASE_ADDR            (AIPS1_OFF_BASE_ADDR + 0x60000)
#define SNVS_BASE_ADDR              (AIPS1_OFF_BASE_ADDR + 0x4C000)

#define IOMUXC_GPR1_OFFSET			0x4
#define HW_ANADIG_REG_CORE			0x00000140
#define BM_ANADIG_REG_CORE_REG2_TRG 0x007C0000
#define BF_ANADIG_REG_CORE_REG2_TRG(v)  \
	(((v) << 18) & BM_ANADIG_REG_CORE_REG2_TRG)


typedef unsigned long long iomux_v3_cfg_t;
#define MUX_CTRL_OFS_SHIFT	0
#define MUX_CTRL_OFS_MASK	((iomux_v3_cfg_t)0xfff << MUX_CTRL_OFS_SHIFT)
#define MUX_PAD_CTRL_OFS_SHIFT	12
#define MUX_PAD_CTRL_OFS_MASK	((iomux_v3_cfg_t)0xfff << \
	MUX_PAD_CTRL_OFS_SHIFT)
#define MUX_SEL_INPUT_OFS_SHIFT	24
#define MUX_SEL_INPUT_OFS_MASK	((iomux_v3_cfg_t)0xfff << \
	MUX_SEL_INPUT_OFS_SHIFT)

#define MUX_MODE_SHIFT		36
#define MUX_MODE_MASK		((iomux_v3_cfg_t)0x1f << MUX_MODE_SHIFT)
#define MUX_PAD_CTRL_SHIFT	41
#define MUX_PAD_CTRL_MASK	((iomux_v3_cfg_t)0x7ffff << MUX_PAD_CTRL_SHIFT)
#define MUX_SEL_INPUT_SHIFT	60
#define MUX_SEL_INPUT_MASK	((iomux_v3_cfg_t)0xf << MUX_SEL_INPUT_SHIFT)

#define MUX_PAD_CTRL(x)		((iomux_v3_cfg_t)(x) << MUX_PAD_CTRL_SHIFT)

#define PAD_CTL_LVE_OFFSET	(22)
#define PAD_CTL_LVE		(1 << 18)
#define NO_PAD_CTRL		(1 << 17)
#define PAD_CTL_HYS		(1 << 16)
#define PAD_CTL_PUS_100K_UP	(2 << 14)
#define PAD_CTL_PUE		(1 << 13)
#define PAD_CTL_PKE		(1 << 12)
#define PAD_CTL_SPEED_MED	(2 << 6)
#define PAD_CTL_DSE_40ohm	(6 << 3)
#define PAD_CTL_SRE_FAST	(1 << 0)

#define MX6Q_UART_PAD_CTRL	(PAD_CTL_PKE | PAD_CTL_PUE |	\
		PAD_CTL_PUS_100K_UP | PAD_CTL_SPEED_MED | \
		PAD_CTL_DSE_40ohm | PAD_CTL_SRE_FAST | PAD_CTL_HYS)

#define IOMUX_PAD(_pad_ctrl_ofs, _mux_ctrl_ofs, _mux_mode, _sel_input_ofs, \
		_sel_input, _pad_ctrl)					\
	(((iomux_v3_cfg_t)(_mux_ctrl_ofs) << MUX_CTRL_OFS_SHIFT) |	\
		((iomux_v3_cfg_t)(_mux_mode) << MUX_MODE_SHIFT) |	\
		((iomux_v3_cfg_t)(_pad_ctrl_ofs) << MUX_PAD_CTRL_OFS_SHIFT) | \
		((iomux_v3_cfg_t)(_pad_ctrl) << MUX_PAD_CTRL_SHIFT) |	\
		((iomux_v3_cfg_t)(_sel_input_ofs) << \
			MUX_SEL_INPUT_OFS_SHIFT) | \
		((iomux_v3_cfg_t)(_sel_input) << MUX_SEL_INPUT_SHIFT))
		
#define _MX6Q_PAD_SD3_DAT7__UART1_TXD                                          \
		IOMUX_PAD(0x0690, 0x02A8, 1, 0x0000, 0, 0)

#define _MX6Q_PAD_SD3_DAT6__UART1_RXD                                          \
		IOMUX_PAD(0x0694, 0x02AC, 1, 0x0920, 3, 0)
		
#define  MX6Q_PAD_SD3_DAT7__UART1_TXD	\
	(_MX6Q_PAD_SD3_DAT7__UART1_TXD | MUX_PAD_CTRL(MX6Q_UART_PAD_CTRL))

#define  MX6Q_PAD_SD3_DAT6__UART1_RXD    \
	(_MX6Q_PAD_SD3_DAT6__UART1_RXD | MUX_PAD_CTRL(MX6Q_UART_PAD_CTRL))


#define UART_PHYS 0x02020000  // for debug uart1
	/* Register definitions */
#define URXD  0x0  /* Receiver Register */
#define UTXD  0x40 /* Transmitter Register */
#define UCR1  0x80 /* Control Register 1 */
#define UCR2  0x84 /* Control Register 2 */
#define UCR3  0x88 /* Control Register 3 */
#define UCR4  0x8c /* Control Register 4 */
#define UFCR  0x90 /* FIFO Control Register */
#define USR1  0x94 /* Status Register 1 */
#define USR2  0x98 /* Status Register 2 */
#define UESC  0x9c /* Escape Character Register */
#define UTIM  0xa0 /* Escape Timer Register */
#define UBIR  0xa4 /* BRM Incremental Register */
#define UBMR  0xa8 /* BRM Modulator Register */
#define UBRC  0xac /* Baud Rate Count Register */
#define UTS   0xb4 /* UART Test Register (mx31) */
	
	/* UART Control Register Bit Fields.*/
#define  URXD_CHARRDY    (1<<15)
#define  URXD_ERR        (1<<14)
#define  URXD_OVRRUN     (1<<13)
#define  URXD_FRMERR     (1<<12)
#define  URXD_BRK        (1<<11)
#define  URXD_PRERR      (1<<10)
#define  URXD_RX_DATA    (0xFF)
#define  UCR1_ADEN       (1<<15) /* Auto dectect interrupt */
#define  UCR1_ADBR       (1<<14) /* Auto detect baud rate */
#define  UCR1_TRDYEN     (1<<13) /* Transmitter ready interrupt enable */
#define  UCR1_IDEN       (1<<12) /* Idle condition interrupt */
#define  UCR1_RRDYEN     (1<<9)	 /* Recv ready interrupt enable */
#define  UCR1_RDMAEN     (1<<8)	 /* Recv ready DMA enable */
#define  UCR1_IREN       (1<<7)	 /* Infrared interface enable */
#define  UCR1_TXMPTYEN   (1<<6)	 /* Transimitter empty interrupt enable */
#define  UCR1_RTSDEN     (1<<5)	 /* RTS delta interrupt enable */
#define  UCR1_SNDBRK     (1<<4)	 /* Send break */
#define  UCR1_TDMAEN     (1<<3)	 /* Transmitter ready DMA enable */
#define  UCR1_UARTCLKEN  (1<<2)	 /* UART clock enabled */
#define  UCR1_DOZE       (1<<1)	 /* Doze */
#define  UCR1_UARTEN     (1<<0)	 /* UART enabled */
#define  UCR2_ESCI	 (1<<15) /* Escape seq interrupt enable */
#define  UCR2_IRTS	 (1<<14) /* Ignore RTS pin */
#define  UCR2_CTSC	 (1<<13) /* CTS pin control */
#define  UCR2_CTS        (1<<12) /* Clear to send */
#define  UCR2_ESCEN      (1<<11) /* Escape enable */
#define  UCR2_PREN       (1<<8)  /* Parity enable */
#define  UCR2_PROE       (1<<7)  /* Parity odd/even */
#define  UCR2_STPB       (1<<6)	 /* Stop */
#define  UCR2_WS         (1<<5)	 /* Word size */
#define  UCR2_RTSEN      (1<<4)	 /* Request to send interrupt enable */
#define  UCR2_TXEN       (1<<2)	 /* Transmitter enabled */
#define  UCR2_RXEN       (1<<1)	 /* Receiver enabled */
#define  UCR2_SRST	 (1<<0)	 /* SW reset */
#define  UCR3_DTREN	 (1<<13) /* DTR interrupt enable */
#define  UCR3_PARERREN   (1<<12) /* Parity enable */
#define  UCR3_FRAERREN   (1<<11) /* Frame error interrupt enable */
#define  UCR3_DSR        (1<<10) /* Data set ready */
#define  UCR3_DCD        (1<<9)  /* Data carrier detect */
#define  UCR3_RI         (1<<8)  /* Ring indicator */
#define  UCR3_TIMEOUTEN  (1<<7)  /* Timeout interrupt enable */
#define  UCR3_RXDSEN	 (1<<6)  /* Receive status interrupt enable */
#define  UCR3_AIRINTEN   (1<<5)  /* Async IR wake interrupt enable */
#define  UCR3_AWAKEN	 (1<<4)  /* Async wake interrupt enable */
#define  UCR3_REF25	 (1<<3)  /* Ref freq 25 MHz */
#define  UCR3_REF30	 (1<<2)  /* Ref Freq 30 MHz */
#define  UCR3_INVT	 (1<<1)  /* Inverted Infrared transmission */
#define  UCR3_BPEN	 (1<<0)  /* Preset registers enable */
#define  UCR4_CTSTL_32   (32<<10) /* CTS trigger level (32 chars) */
#define  UCR4_INVR	 (1<<9)  /* Inverted infrared reception */
#define  UCR4_ENIRI	 (1<<8)  /* Serial infrared interrupt enable */
#define  UCR4_WKEN	 (1<<7)  /* Wake interrupt enable */
#define  UCR4_REF16	 (1<<6)  /* Ref freq 16 MHz */
#define  UCR4_IRSC	 (1<<5)  /* IR special case */
#define  UCR4_TCEN	 (1<<3)  /* Transmit complete interrupt enable */
#define  UCR4_BKEN	 (1<<2)  /* Break condition interrupt enable */
#define  UCR4_OREN	 (1<<1)  /* Receiver overrun interrupt enable */
#define  UCR4_DREN	 (1<<0)  /* Recv data ready interrupt enable */
#define  UFCR_RXTL_SHF   0       /* Receiver trigger level shift */
#define  UFCR_RFDIV      (7<<7)  /* Reference freq divider mask */
#define  UFCR_TXTL_SHF   10      /* Transmitter trigger level shift */
#define  USR1_PARITYERR  (1<<15) /* Parity error interrupt flag */
#define  USR1_RTSS	 (1<<14) /* RTS pin status */
#define  USR1_TRDY	 (1<<13) /* Transmitter ready interrupt/dma flag */
#define  USR1_RTSD	 (1<<12) /* RTS delta */
#define  USR1_ESCF	 (1<<11) /* Escape seq interrupt flag */
#define  USR1_FRAMERR    (1<<10) /* Frame error interrupt flag */
#define  USR1_RRDY       (1<<9)	 /* Receiver ready interrupt/dma flag */
#define  USR1_TIMEOUT    (1<<7)	 /* Receive timeout interrupt status */
#define  USR1_RXDS	 (1<<6)	 /* Receiver idle interrupt flag */
#define  USR1_AIRINT	 (1<<5)	 /* Async IR wake interrupt flag */
#define  USR1_AWAKE	 (1<<4)	 /* Aysnc wake interrupt flag */
#define  USR2_ADET	 (1<<15) /* Auto baud rate detect complete */
#define  USR2_TXFE	 (1<<14) /* Transmit buffer FIFO empty */
#define  USR2_DTRF	 (1<<13) /* DTR edge interrupt flag */
#define  USR2_IDLE	 (1<<12) /* Idle condition */
#define  USR2_IRINT	 (1<<8)	 /* Serial infrared interrupt flag */
#define  USR2_WAKE	 (1<<7)	 /* Wake */
#define  USR2_RTSF	 (1<<4)	 /* RTS edge interrupt flag */
#define  USR2_TXDC	 (1<<3)	 /* Transmitter complete */
#define  USR2_BRCD	 (1<<2)	 /* Break condition */
#define  USR2_ORE        (1<<1)	 /* Overrun error */
#define  USR2_RDR        (1<<0)	 /* Recv data ready */
#define  UTS_FRCPERR	 (1<<13) /* Force parity error */
#define  UTS_LOOP        (1<<12) /* Loop tx and rx */
#define  UTS_TXEMPTY	 (1<<6)	 /* TxFIFO empty */
#define  UTS_RXEMPTY	 (1<<5)	 /* RxFIFO empty */
#define  UTS_TXFULL	 (1<<4)	 /* TxFIFO full */
#define  UTS_RXFULL	 (1<<3)	 /* RxFIFO full */
#define  UTS_SOFTRST	 (1<<0)	 /* Software reset */


#define Asm __asm__ volatile
#define CP15_SET_VBAR(x)	Asm("mcr p15, 0, %0, c12, c0, 0"::"r"(x))
#define CP15_SET_MVBAR(x)	Asm("mcr p15, 0, %0, c12, c0, 1"::"r"(x))
#define CP15_SET_SCR(x)		Asm("mcr p15, 0, %0, c1,  c1, 0"::"r"(x))
#define CP15_SET_NSACR(x)	Asm("mcr p15, 0, %0, c1,  c1, 2"::"r"(x))
#define CP15_SET_CPACR(x)	Asm("mcr p15, 0, %0, c1,  c0, 2"::"r"(x))

#define CP15_GET_ID_PFR1(x)	Asm("mrc p15, 0, %0, c0,  c1, 1":"=r"(x))
#define CP15_GET_SCR(x)		Asm("mrc p15, 0, %0, c1,  c1, 0":"=r"(x))

//For Secure Configuration Register
//=================================
#define SCR_NET	(1 << 6)	/*Not Early Termination*/
#define SCR_AW	(1 << 5)	/*0  the CPSR.A bit can be modified only in Secure state.*/
							/*1  the CPSR.A bit can be modified in any security state.*/
#define SCR_FW	(1 << 4)	/*0  the CPSR.F bit can be modified only in Secure state*/
							/*1  the CPSR.F bit can be modified in any security state.*/
#define SCR_EA	(1 << 3)	/*0  Abort mode handles external aborts*/
							/*1  Monitor mode handles external aborts.*/
#define SCR_FIQ	(1 << 2)	/*0  FIQ mode entered when FIQ is taken*/
							/*1  Monitor mode entered when FIQ is taken.*/
#define SCR_IRQ	(1 << 1)	/*0  IRQ mode entered when IRQ is taken*/
							/*1  Monitor mode entered when IRQ is taken.*/
#define SCR_NS	(1 << 0)	/*0:Secure state. 1:Non-secure state*/

#define SCR_SETTING	( SCR_AW | SCR_FW )
//=================================SCR

//For Non-Secure Access Control Register
//=================================
#define NSACR_NS_SMP_BIT	(1 << 18)
/*Determines if the SMP bit of the Auxiliary Control Register is writeable in Non-secure state*/

#define NSACR_TL_BIT		(1 << 17)
/*Determines if lockable TLB entries can be allocated in Non-secure state*/

#define NSACR_PLE_BIT		(1 << 16)
/*Controls NS accesses to the Preload Engine resources*/

#define NSACR_CP11_BIT		(1 << 11)
/*Determines permission to access coprocessor 11 in the Non-secure state*/

#define NSACR_CP10_BIT		(1 << 10)
/*Determines permission to access coprocessor 10 in the Non-secure state*/

#define NSACR_SETTING (NSACR_NS_SMP_BIT | NSACR_TL_BIT |NSACR_PLE_BIT| NSACR_CP11_BIT | NSACR_CP10_BIT)
//=================================end NSACR

void cpu_init(void);
void uart_init(void);
void uart_puts(const char *s);
