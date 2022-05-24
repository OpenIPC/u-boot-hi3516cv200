/*
 * Configuation settings for the hi3531d board.
 *
 * Copyright (c) 2016-2017 HiSilicon Technologies Co., Ltd.
 *
 * This program is free software; you can redistribute  it and/or modify it
 * under  the terms of  the GNU General  Public License as published by the
 * Free Software Foundation;  either version 2 of the  License, or (at your
 * option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

/*
 * CPU specific code
 */

#include <common.h>
#include <command.h>
#include <asm/system.h>
#include <asm/cache.h>
#include <asm/io.h>
#include <asm/sizes.h>
#include <asm/cpu_info.h>

static void cache_flush(void);

int cleanup_before_linux(void)
{
	unsigned int i;

	/*
	 * this function is called just before we call linux
	 * it prepares the processor for linux
	 *
	 * we turn off caches etc ...
	 */
	disable_interrupts();

	/* turn off I/D-cache */
	icache_disable();
	dcache_disable();

	/* invalidate I-cache */
	cache_flush();

	i = 0;
	/* mem barrier to sync up things */
	asm("mcr p15, 0, %0, c7, c10, 4" : : "r"(i));

#ifdef CFG_MMU_HANDLEOK
	mmu_turnoff();
#endif

	return 0;
}
/*****************************************************************************/
struct cpu_info_t cpu_info[] = {
	{
		.name   = "Unknown",
		.chipid = 0,
		.chipid_mask = ~0,
		.devs = 0,
		.max_ddr_size = 0,
		.boot_media = NULL,
		.get_clock  = NULL,
		.get_cpu_version = NULL,
	},
	{
		.name   = "Hi3531D",
		.chipid = _HI3531D_V100,
		.chipid_mask = _HI3531D_MASK,
		.devs = 0,
		.max_ddr_size = SZ_3G,
		.boot_media = NULL,
		.get_clock  = NULL,
		.get_cpu_version = NULL,
	},
	{0},
};
static void cache_flush(void)
{
	asm ("mcr p15, 0, %0, c7, c5, 0" : : "r" (0));
}
