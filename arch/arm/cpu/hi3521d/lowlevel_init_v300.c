/*
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

#include <asm/arch/platform.h>
#include <config.h>
#ifdef CONFIG_DDR_TRAINING_V2
#include <ddr_interface.h>
#endif

#define HI_SYS_CTL_REG	SYS_CTRL_REG_BASE

#define REG_SC_GEN32		0xa0
#define DDR_DMC_BASE_REG0	0x12118000
#define DDR0_PLL_REG		0x1211c000

#define REG_DDR_DMC0_CFG_TIMING2 (DDR_DMC_BASE_REG0 + 0x108)
#define REG_DDR_PHY0_INITCTRL    (DDR0_PLL_REG + 0x4)
#define REG_DDR_DMC0_CTRL_SREF   DDR_DMC_BASE_REG0
#define REG_DDR_DMC0_IN_SREF     (DDR_DMC_BASE_REG0 + 0x294)

extern void reset_cpu(unsigned long addr);

static inline void DWB(void) /* drain write buffer */
{
}

static inline unsigned int readl(unsigned addr)
{
	unsigned int val;

	val = (*(volatile unsigned int *)(addr));
	return val;
}

static inline void writel(unsigned val, unsigned addr)
{
	DWB();
	(*(volatile unsigned *) (addr)) = (val);
	DWB();
}

static void ddr_set_sref(unsigned int val, unsigned int addr_sref,
		unsigned int st_reg)
{
	int cnt = 0;

	writel(val, addr_sref);
	while ((!(readl(st_reg) & 0x1)) && (cnt < 10000))
		cnt++;

	if (cnt >= 10000)
		reset_cpu(0);
}

static void ddr_hw_training(void)
{
	unsigned int initctrl_phy0;

	initctrl_phy0 = readl(HI_SYS_CTL_REG + REG_SC_GEN32);
	if (initctrl_phy0) {
		unsigned int timing;
		unsigned int timing_backup;

		/* Save the Auto-self refresh timing */
		timing = readl(REG_DDR_DMC0_CFG_TIMING2);
		timing_backup = timing & 0x7FF;
		/* Set Auto-self refresh timing to 0 */
		writel((timing & 0xFFFFF800), REG_DDR_DMC0_CFG_TIMING2);
		/* Start HW DDR training */
		writel(initctrl_phy0, REG_DDR_PHY0_INITCTRL);

		/* Exit Auto-self refresh */
		ddr_set_sref(0x2, REG_DDR_DMC0_CTRL_SREF, REG_DDR_DMC0_IN_SREF);

		do {
			initctrl_phy0 = readl(REG_DDR_PHY0_INITCTRL) & 0x7FFFF;
		} while (initctrl_phy0);
		/* Restore the Auto-self refresh timing */
		timing = readl(REG_DDR_DMC0_CFG_TIMING2);
		timing = (timing & 0xFFFFF800) | timing_backup;
		writel(timing, REG_DDR_DMC0_CFG_TIMING2);
	}

	/* Clear fifo */
	initctrl_phy0 = readl(REG_DDR_PHY0_INITCTRL);
	initctrl_phy0 = initctrl_phy0 | (1<<15);
	writel(initctrl_phy0, REG_DDR_PHY0_INITCTRL);
	initctrl_phy0 = readl(REG_DDR_PHY0_INITCTRL);
	initctrl_phy0 = initctrl_phy0 & ~(1<<15);
	writel(initctrl_phy0, REG_DDR_PHY0_INITCTRL);

}
static inline void delay(unsigned int num)
{
	unsigned int i;

	for (i = 0; i < (100 * num); i++)
		__asm__ __volatile__("nop");
}

#ifdef	CONFIG_SVB_ENABLE
#define PWM_REG_BASE	0x120e0000
#define CPU_VOL_REG		(0x04 + PWM_REG_BASE)
#define CORE_VOL_REG	PWM_REG_BASE

#define HPM_VER_REG		0x12121000
#define HPM_CORE_DATA	0x12122000
#define HPM_CPU_DATA	0x1212200c

static void get_hpm_iddq_value(unsigned int addr,
		unsigned int *hpm, unsigned int *iddq)
{
	unsigned int val;

	val = readl(addr);

	if (addr == HPM_CPU_DATA) {
		/* get the HPM && IDDQ of cpu */
		*hpm = val & 0x3ff;
		*iddq = (val & 0x3f0000) >> 16;
	} else if (addr == HPM_CORE_DATA) {
		/* get the HPM && IDDQ of core */
		*hpm = val & 0x3ff;
		*iddq = (val & 0x3f0000) >> 16;
	} else
		asm ("b .");
}

static void set_hpm_iddq_volt(unsigned int hpm,
		unsigned int iddq, unsigned int sel,
		unsigned int hpm_version)
{
	if (hpm_version == 0x00 && sel == 0) {
		if (hpm <= 209)
			writel(0xc7, CPU_VOL_REG);
		else if (hpm <= 234)
			writel(0x700c7, CPU_VOL_REG);
		else if (hpm <= 265)
			writel(0x2200c7, CPU_VOL_REG);
		else if (hpm <= 283)
			writel(0x4200c7, CPU_VOL_REG);
		else
			writel(0x5700c7, CPU_VOL_REG);
	} else if (hpm_version == 0x00 && sel == 1) {
		if (hpm <= 205)
			writel(0xc7, CORE_VOL_REG);
		else if (hpm <= 225)
			writel(0x1500c7, CORE_VOL_REG);
		else if (hpm <= 255)
			writel(0x2a00c7, CORE_VOL_REG);
		else if (hpm <= 270)
			writel(0x3f00c7, CORE_VOL_REG);
		else
			writel(0x5a00c7, CORE_VOL_REG);
	} else if (hpm_version == 0x01 && sel == 0) {
		if (hpm <= 230)
			writel(0xc7, CPU_VOL_REG);
		else if (hpm <= 259)
			writel(0x1600c7, CPU_VOL_REG);
		else if (hpm <= 277)
			writel(0x3500c7, CPU_VOL_REG);
		else
			writel(0x4a00c7, CPU_VOL_REG);
	} else if (hpm_version == 0x01 && sel == 1) {
		if (hpm <= 234)
			writel(0xc7, CORE_VOL_REG);
		else if (hpm <= 264)
			writel(0x1300c7, CORE_VOL_REG);
		else if (hpm <= 279)
			writel(0x2800c7, CORE_VOL_REG);
		else
			writel(0x4a00c7, CORE_VOL_REG);
	} else
		asm ("b .");
}

void start_svb(void)
{
	unsigned int cpu_hpm;
	unsigned int core_hpm;
	unsigned int cpu_iddq;
	unsigned int core_iddq;
	unsigned int hpm_ver = readl(HPM_VER_REG);

	/*
	 * Read and judge the value get from HPM and IDDQ
	 */
	get_hpm_iddq_value(HPM_CPU_DATA, &cpu_hpm, &cpu_iddq);
	get_hpm_iddq_value(HPM_CORE_DATA, &core_hpm, &core_iddq);

	/* get the HPM version: bit[1:0] */
	hpm_ver &= 0x3;

	if (0x00 == hpm_ver) {
		/*
		 * Set the voltage about HPM and IDDQ
		 * 0: CPU
		 * 1: CORE
		 */
			set_hpm_iddq_volt(cpu_hpm, cpu_iddq, 0, hpm_ver);
			set_hpm_iddq_volt(core_hpm, core_iddq, 1, hpm_ver);
	} else if (0x01 == hpm_ver) {
		/*
		 * Set the voltage about HPM and IDDQ
		 * 0: CPU
		 * 1: CORE
		 */
			set_hpm_iddq_volt(cpu_hpm, cpu_iddq, 0, hpm_ver);
			set_hpm_iddq_volt(core_hpm, core_iddq, 1, hpm_ver);
	}
}
#endif

void ddrc_enable_scramble(unsigned int reg)
{
	unsigned int val;

	/* ddr enable SCRAMBLE */
	val = readl(reg + 0x50);
	val |= (1 << 14);
	writel(val, reg + 0x50);
}

void start_ddr_training(unsigned int base)
{
#ifdef CONFIG_SVB_ENABLE
	/* add SVB function */
	start_svb();
	delay(10000);
#endif

	ddr_hw_training();

#ifdef CONFIG_DDR_TRAINING_V2
	int ret = 0;

	/* ddr training function */
	ret = ddr_sw_training_if(0);
	if (ret)
		reset_cpu(0);
#endif

	ddrc_enable_scramble(DDR_DMC_BASE_REG0);

	/*the value should config after trainning, or
	  it will cause chip compatibility problems*/
	writel(0x401, DDR_DMC_BASE_REG0 + 0x28);
}

