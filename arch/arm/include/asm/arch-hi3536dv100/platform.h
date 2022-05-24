#ifndef __HI_CHIP_REGS_H__
#define __HI_CHIP_REGS_H__

/* -------------------------------------------------------------------- */
#define RAM_START_ADRS			0x04010500

/* -------------------------------------------------------------------- */
#define FMC_REG_BASE			0x10000000

/* -------------------------------------------------------------------- */
#define REG_BASE_SF				0x10010000

/* -------------------------------------------------------------------- */
#define HIUSB_OHCI_BASE			0x11000000

/* -------------------------------------------------------------------- */
#define TIMER0_REG_BASE			0x12000000
#define TIMER1_REG_BASE			0x12000020
#define TIMER2_REG_BASE			0x12010000
#define TIMER3_REG_BASE			0x12010020
#define TIMER4_REG_BASE			0x12020000
#define TIMER5_REG_BASE			0x12020020
#define TIMER6_REG_BASE			0x12030000
#define TIMER7_REG_BASE			0x12030020

#define REG_TIMER_RELOAD		0x0
#define REG_TIMER_VALUE			0x4
#define REG_TIMER_CONTROL		0x8

/* -------------------------------------------------------------------- */
/* Clock and Reset Generator REG */
/* -------------------------------------------------------------------- */
#define CRG_REG_BASE			0x12040000

#define REG_CRG20			0x0050
#define REG_CRG46			0x00b8
#define REG_CRG47			0x00bc
#define REG_CRG48			0x00c0
#define REG_CRG49			0x00c4
#define REG_CRG51			0x00cc

/* SOC CRG register offset */
#define REG_SOC_CRG			REG_CRG20
#define GET_SYS_BUS_CLK(_reg)		(((_reg) >> 2) & 0x3)
#define SYS_CLK_XTAL			0
#define SYS_CLK_202M			2

/* USB 2.0 CRG Control register offset */
#define REG_USB2_CTRL			REG_CRG46

/* USB 2.0 CRG PHY register offset */
#define REG_USB2_PHY0			REG_CRG47

/* FMC CRG register offset */
#define REG_FMC_CRG			REG_CRG48
#define FMC_CLK_SEL(_clk)		(((_clk) & 0x3) << 2)
#define GET_FMC_CLK_TYPE(_reg)		(((_reg) >> 2) & 0x3)
#define FMC_CLK_SEL_MASK		(0x3 << 2)
#define FMC_CLK_24M			0
#define FMC_CLK_83M			1
#define FMC_CLK_150M		2
#define FMC_CLK_ENABLE			(0x1 << 1)

/* Ethernet CRG register offset */
#define REG_ETH_CRG			REG_CRG49
#define REG_ETH_MAC_IF		0x8c

/* Uart CRG register offset */
#define REG_UART_CRG			REG_CRG51
#define UART_CLK_SEL(_clk)		(((_clk) & 0x3) << 18)
#define UART_CLK_SEL_MASK		(0x3 << 18)
#define UART_CLK_APB			0
#define UART_CLK_24M			1
#define UART_CLK_2M			2

/* -------------------------------------------------------------------- */
/* System Control REG */
/* -------------------------------------------------------------------- */
#define SYS_CTRL_REG_BASE		0x12050000
#define REG_BASE_SCTL			SYS_CTRL_REG_BASE

/* System Control register offset */
#define REG_SC_CTRL			0x0000
#define SC_CTRL_TIMER0_CLK_SEL(_clk)	(((_clk) & 0x1) << 16)
#define TIMER0_CLK_SEL_MASK		(0x1 << 16)
#define TIMER_CLK_3M			0
#define TIMER_CLK_BUS			1
#define SC_CTRL_REMAP_CLEAR		(0x1 << 8)

/* System soft reset register offset */
#define REG_SC_SYSRES			0x0004

/* System Status register offset */
#define REG_SYSSTAT			0x008c
/* bit[8]=0; SPI nor flash; bit[8]=1: SPI nand flash */
#define GET_SPI_DEVICE_TYPE(_reg)	(((_reg) >> 8) & 0x1)
/* if bit[8]=0 SPI nor flash
 * bit[7]=0: 3-Byte address mode; bit[7]=1: 4-Byte address mode */
#define GET_SPI_NOR_ADDR_MODE(_reg)	(((_reg) >> 7) & 0x1)
#define GET_SYS_BOOT_MODE(_reg)		(((_reg) >> 8) & 0x1)
#define BOOT_FROM_SPI_NOR		0
#define BOOT_FROM_SPI_NAND		1
#define REG_SC_GEN0			0x0138
#define REG_SC_GEN1			0x013c
#define REG_SC_GEN2			0x0140
#define REG_SC_GEN3			0x0144
#define REG_SC_GEN4			0x0148

/* -------------------------------------------------------------------- */
#define WDG_REG_BASE			0x12070000
#define UART0_REG_BASE			0x12080000
#define UART1_REG_BASE			0x12090000
#define UART2_REG_BASE			0x120A0000

/* -------------------------------------------------------------------- */
#define RTC_REG_BASE			0x120B0000
#define IO_CONFIG_REG_BASE		0x120F0000
#define DDRC0_REG_BASE			0x12110000

/* -------------------------------------------------------------------- */
/* Peripheral Control REG */
/* -------------------------------------------------------------------- */
#define MISC_REG_BASE			0x12120000

#define MISC_CTRL20			0x0050
#define MISC_CTRL21			0x0054
#define MISC_CTRL25			0x0064

/* USB 2.0 MISC Control register offset */
#define REG_USB2_CTRL0			MISC_CTRL20
#define REG_USB2_CTRL1			MISC_CTRL21

/* Ethernet internal FEPHY MISC Control register offset */
#define REG_FEPHY_CTRL1			MISC_CTRL25

#define IR_REG_BASE				0x12140000
#define GPIO0_REG_BASE			0x12150000
#define GPIO1_REG_BASE			0x12160000
#define GPIO2_REG_BASE			0x12170000
#define GPIO3_REG_BASE			0x12180000
#define GPIO4_REG_BASE			0x12190000
#define GPIO5_REG_BASE			0x121A0000
#define GPIO6_REG_BASE			0x121B0000
#define GPIO7_REG_BASE			0x121C0000
#define GPIO8_REG_BASE			0x121D0000
#define GPIO9_REG_BASE			0x121E0000
#define GPIO10_REG_BASE			0x121F0000
#define GPIO11_REG_BASE			0x12200000
#define GPIO12_REG_BASE			0x12210000
#define GPIO13_REG_BASE			0x12220000
#define FMC_MEM_BASE			0x14000000
#define DDR_MEM_BASE			0x80000000

/********** Communication Register and flag used by bootrom *************/
#define REG_START_FLAG         (SYS_CTRL_REG_BASE + REG_SC_GEN1)
#define CONFIG_START_MAGIC     (0x444f574e)

/*-----------------------------------------------------------------------
 * DDR Training
 * ----------------------------------------------------------------------*/

#define STACK_TRAINING			0x04018000
#define _HI3536D_V100           (0x003536d100LL)
#define _HI3536DV100_MASK       (0xFFFFFFFFFFLL)
/******************************************************************************/

#endif /* End of __HI_CHIP_REGS_H__ */

