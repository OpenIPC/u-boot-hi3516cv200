/**
 * ddr_training_custom.c
 *
 * Copyright (c) 2009-2014, HiSilicon Technologies Co., Ltd.
 * All rights reserved.
 *
 * DDR training custom implement.
 */

#include <ddr_interface.h>
#include "ddr_training_impl.h"

/**
 * ddr_cmd_prepare_copy
 * @void
 *
 * Do some prepare before copy code from DDR to SRAM.
 * Keep empty when nothing to do.
 */
void ddr_cmd_prepare_copy(void) { return; }

/**
 * ddr_cmd_site_save
 * @void
 *
 * Save site before DDR training command execute .
 * Keep empty when nothing to do.
 */
void ddr_cmd_site_save(void) { return; }

/**
 * ddr_cmd_site_restore
 * @void
 *
 * Restore site after DDR training command execute.
 * Keep empty when nothing to do.
 */
void ddr_cmd_site_restore(void) { return; }
#define DDR_DDRC_SCRAMB_CTRL0	(0x12118050)
#define DDR_DDRC_SCRAMB_CTRL1	(0x12119050)

void ddr_training_save_reg_custom(void *reg, unsigned int mask)
{
	struct tr_relate_reg *relate_reg = (struct tr_relate_reg *)reg;

	/* save scramb */
	relate_reg->custom.scramb[0] = REG_READ(DDR_DDRC_SCRAMB_CTRL0);
	/* disable scramb */
	REG_WRITE(relate_reg->custom.scramb[0] & 0xffffbfff, DDR_DDRC_SCRAMB_CTRL0);
	DDR_DEBUG("Disable scramb [0x%x] = 0x%x.", DDR_DDRC_SCRAMB_CTRL0, relate_reg->custom.scramb[0] & 0xffffbfff);

#if DDR_PHY_NUM == 2
	relate_reg->custom.scramb[1] = REG_READ(DDR_DDRC_SCRAMB_CTRL1);
	REG_WRITE(relate_reg->custom.scramb[1] & 0xffffbfff, DDR_DDRC_SCRAMB_CTRL1);
	DDR_DEBUG("Disable scramb [0x%x] = 0x%x.", DDR_DDRC_SCRAMB_CTRL1, relate_reg->custom.scramb[1] & 0xffffbfff);
#endif
}

void ddr_training_restore_reg_custom(void *reg)
{
	struct tr_relate_reg *relate_reg = (struct tr_relate_reg *)reg;

	/* restore scramb */
	REG_WRITE(relate_reg->custom.scramb[0], DDR_DDRC_SCRAMB_CTRL0);
	DDR_DEBUG("Restore scramb[0x%x] = 0x%x.", DDR_DDRC_SCRAMB_CTRL0, relate_reg->custom.scramb[0]);

#if DDR_PHY_NUM == 2
	REG_WRITE(relate_reg->custom.scramb[1], DDR_DDRC_SCRAMB_CTRL1);
	DDR_DEBUG("Restore scramb[0x%x] = 0x%x.", DDR_DDRC_SCRAMB_CTRL1, relate_reg->custom.scramb[1]);
#endif
}
