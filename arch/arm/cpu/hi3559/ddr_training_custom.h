/**
 * ddr_training_custom.h
 *
 * Copyright (c) 2009-2014, HiSilicon Technologies Co., Ltd.
 * All rights reserved.
 *
 * DDR training custom define.
 */

/* config DDRC, PHY, DDRT typte */
#define DDR_DDRC_V510_CONFIG
#define DDR_PHY_T28_CONFIG
#define DDR_DDRT_T28_CONFIG

/* config special item */
/*#define DDR_TRAINING_UART_DISABLE*/
#define DDR_VREF_TRAINING_CONFIG
#define DDR_MPR_TRAINING_CONFIG
#define DDR_VREF_WITHOUT_BDL_CONFIG

/* config DDRC, PHY, DDRT base address */
/* [CUSTOM] DDR DMC0 base register */
#define DDR_REG_BASE_DMC0		0x12068000
/* [CUSTOM] DDR DMC1 base register */
#define DDR_REG_BASE_DMC1		(DDR_REG_BASE_DMC0)
/* [CUSTOM] DDR PHY0 base register */
#define DDR_REG_BASE_PHY0		0x1206c000
/* [CUSTOM] DDR PHY1 base register */
#define DDR_REG_BASE_PHY1		(DDR_REG_BASE_PHY0)
/* [CUSTOM] DDR DDRT base register */
#define DDR_REG_BASE_DDRT		0x12050000
/* [CUSTOM] DDR training item system control */
#define DDR_REG_BASE_SYSCTRL		0x12020000
#define DDR_REG_BASE_AXI		0x12060000

/* config offset address */
/* Assume sysctrl offset address for DDR training as follows,
if not please define. */
/* [CUSTOM] ddrt reversed data */
#define SYSCTRL_DDRT_PATTERN		0x94
/* [CUSTOM] PHY2 ddrt reversed data */
#define SYSCTRL_DDRT_PATTERN_SEC	0x130
/* [CUSTOM] ddr training item */
#define SYSCTRL_DDR_TRAINING_CFG	0x90
/* [CUSTOM] ddr training stat */
#define SYSCTRL_DDR_TRAINING_STAT	0x98

/* config other special */
/* [CUSTOM] DDR training start address. MEM_BASE_DDR*/
#define DDRT_CFG_BASE_ADDR		0x80000000
/* [CUSTOM] SRAM start address.
NOTE: Makefile will parse it, plase define it as Hex. eg: 0xFFFF0C00 */
#define DDR_TRAINING_RUN_STACK		0x04010500

#define DDR_RELATE_REG_DECLARE
#define DDR_TRAINING_SAVE_REG_FUNC(relate_reg, mask) \
	ddr_training_save_reg_custom(relate_reg, mask)

#define DDR_TRAINING_RESTORE_REG_FUNC(relate_reg) \
	ddr_training_restore_reg_custom(relate_reg)

#define REG_PERI_CRG54          0x00D8
#define DDRT_CLKEN              (0x1<<3)

struct tr_custom_reg {
	unsigned int scramb;
};
void ddr_training_save_reg_custom(void *relate_reg, unsigned int mask);
void ddr_training_restore_reg_custom(void *relate_reg);
