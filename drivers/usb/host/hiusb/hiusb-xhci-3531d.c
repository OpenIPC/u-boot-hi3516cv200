#define HIUSB_XHCI_BASE		(0x11000000)
#define IO_REG_USB3_CTRL	(0x12040000 + 0x12c)
#define USB3_VCC_SRST_REQ2	(1 << 13)
#define USB3_UTMI_CKEN		(1 << 12)
#define USB3_PIPE_CKEN		(1 << 11)
#define USB3_SUSPEND_CKEN	(1 << 10)
#define USB3_REF_CKEN		(1 << 9)
#define USB3_BUS_CKEN		(1 << 8)

#define PERI_CRG72	(0x12040000 + 0x120)
#define COMBPHY1_REFCLK1_SEL	(0x3 << 14)
#define  COMBPHY1_LANE1_REQ		(0x1 << 3)
#define  COMBPHY1_LANE0_REQ		(0x1 << 2)

#define IO_REG_COMB_PHY1	(0x12120000 + 0x12c)
/* address 0x6 to write value 0x4 */
#define CONFIG_CLK		((0x1 << 5) | (0x6 << 0) | (0x4 << 8))

#define IO_REG_USB3_CTRL1	(0x12120000 + 0x12c)

#define GTXTHRCFG		0xC108
#define USB_TXPKTCNT_SEL	(1 << 29)
#define USB_TXPKTCNT		(3 << 24)
#define USB_MAX_TXBUTST_SIZE	(0x10 << 16)
#define GRXTHRCFG		0xC10C
#define USB_RXPKTCNT_SEL	(1 << 29)
#define USB_RXPKTCNT		(3 << 24)
#define USB_MAX_RXBUTST_SIZE	(0x10 << 16)

#define REG_GCTL		0xC110
#define PRTCAPDIR_HOST		(1 << 12)
#define PRTCAPDIR_MASK		((1 << 12) | (1 << 13))

#define REG_GUSB2PHYCFG0	0xC200
#define BIT_UTMI_ULPI		(1 << 4)
#define BIT_UTMI_8_16		(1 << 3)

#define REG_GUSB3PIPECTL0	0xC2C0
#define PCS_SSP_SOFT_RESET	(1 << 31)
#define USB3_PHY_SUSPEND_EN	(1 << 17)
#define USB3_SELECT_VOLT	(1 << 4)
#define USB3_DEEMPHASIS	(1 << 1)

#define ID_REG_USB2_CTRL1       (0x12120000 + 0x94)
#define USB2_PHY_TEST_REG_ACCESS	(1 << 20)

int xhci_hcd_init(int index, struct xhci_hccr **hccr, struct xhci_hcor **hcor)
{
	*hccr = (struct xhci_hccr *)(HIUSB_XHCI_BASE);
	*hcor = (struct xhci_hcor *)((uint32_t) *hccr
			+ HC_LENGTH(xhci_readl(&(*hccr)->cr_capbase)));

	return 0;
}

void hiusb_start_hcd(void)
{
	unsigned long flags;
	unsigned int reg;

	local_irq_save(flags);

	writel(0x301, (0x1204016c));
	wait_ms(2);
	reg = readl(IO_REG_USB3_CTRL);
	reg |= USB3_VCC_SRST_REQ2;
	writel(reg, IO_REG_USB3_CTRL);
	wait_ms(10);

	reg = readl(PERI_CRG72);
	reg |= COMBPHY1_LANE1_REQ;
	writel(reg, PERI_CRG72);
	wait_ms(10);

	reg = readl(PERI_CRG72);
	reg &= ~COMBPHY1_LANE1_REQ;
	reg &= ~COMBPHY1_LANE0_REQ;
	writel(reg, PERI_CRG72);
	wait_ms(10);

	writel(0x1, (0x1204016c));
	wait_ms(2);
#if 0
	/* config clock */
	reg = readl(IO_REG_COMB_PHY1);
	reg |= CONFIG_CLK;
	writel(reg, IO_REG_COMB_PHY1);
	wait_ms(100);

	writel(0x0, IO_REG_COMB_PHY1);
	wait_ms(2);
#endif
	/* de-assert usb3_vcc_srst_req */
	reg = readl(IO_REG_USB3_CTRL);
	reg |= USB3_VCC_SRST_REQ2;
	reg |= USB3_UTMI_CKEN;
	reg |= USB3_PIPE_CKEN;
	reg |= USB3_SUSPEND_CKEN;
	reg |= USB3_REF_CKEN;
	reg |= USB3_BUS_CKEN;
	writel(reg, IO_REG_USB3_CTRL);
	wait_ms(100);

	reg = readl(IO_REG_USB3_CTRL);
	reg &= ~(USB3_VCC_SRST_REQ2);
	writel(reg, IO_REG_USB3_CTRL);
	wait_ms(100);

	reg = readl(HIUSB_XHCI_BASE + REG_GUSB3PIPECTL0);
	reg |= PCS_SSP_SOFT_RESET;
	writel(reg, HIUSB_XHCI_BASE + REG_GUSB3PIPECTL0);

	/*step 3: USB2 PHY chose ulpi 8bit interface */
	reg = readl(HIUSB_XHCI_BASE + REG_GUSB2PHYCFG0);
	reg &= ~BIT_UTMI_ULPI;
	reg &= ~BIT_UTMI_8_16;
	reg &= ~(0x1 << 6);
	writel(reg, HIUSB_XHCI_BASE + REG_GUSB2PHYCFG0);
	wait_ms(20);

	/* set host mode. [13:12] 01: Host; 10: Device; 11: OTG */
	reg = readl(HIUSB_XHCI_BASE + REG_GCTL);
	reg &= ~PRTCAPDIR_MASK;
	reg |= PRTCAPDIR_HOST;
	writel(reg, HIUSB_XHCI_BASE + REG_GCTL);

#if 1
	/* config u3 u2 eye diagram */
	/* close HS pre-emphasis */
	reg = readl(MISC_REG_BASE + 0x10);
	reg |= USB2_PHY_TEST_REG_ACCESS;
	writel(reg, MISC_REG_BASE + 0x10);
	udelay(100);
	/* slew rate */
	writel(0xc4, MISC_REG_BASE + 0x8020);
	udelay(20);
	/* Calibration mode */
	writel(0xc1, MISC_REG_BASE + 0x8044);
	udelay(20);
	/* disconnect threshold value*/
	writel(0x1b, MISC_REG_BASE + 0x8028);
	udelay(20);
	/* turn on pre-emphasis */
	writel(0x1c, MISC_REG_BASE + 0x8000);
	udelay(20);
	writel(0x92, MISC_REG_BASE + 0x8014);
	udelay(20);
	writel(0xe, MISC_REG_BASE + 0x8018);
	udelay(20);
#endif
#if 0
	/* config u3 eye diagram */
	writel(0xd10, IO_REG_USB3_CTRL1);
	writel(0xd30, IO_REG_USB3_CTRL1);
	writel(0xd10, IO_REG_USB3_CTRL1);/* USB3_TX_TERMINATION_TRIM = 1101 */
	writel(0x0, IO_REG_USB3_CTRL1);
	wait_ms(10);
	writel(0xd1b, IO_REG_USB3_CTRL1);
	writel(0xd3b, IO_REG_USB3_CTRL1);
	writel(0xd1b, IO_REG_USB3_CTRL1); /* TX_SWING_COMP = 1101 */
	writel(0x0, IO_REG_USB3_CTRL1);
	wait_ms(10);

	writel(0x802, IO_REG_USB3_CTRL1);
	writel(0x822, IO_REG_USB3_CTRL1);
	writel(0x802, IO_REG_USB3_CTRL1); /* SSC enable */
	writel(0x0, IO_REG_USB3_CTRL1);
	wait_ms(10);
#endif
	reg = readl(HIUSB_XHCI_BASE + REG_GUSB3PIPECTL0);
	reg &= ~PCS_SSP_SOFT_RESET;
	reg &= ~USB3_PHY_SUSPEND_EN;	/* disable suspend */
	reg &= ~(0x7 << 3);
	reg |= USB3_SELECT_VOLT;	/* select volt is 1100mv */
	reg &= ~(0x3 << 1);
	reg |= USB3_DEEMPHASIS;		/* -3.5db de-emphasis */
	writel(reg, HIUSB_XHCI_BASE + REG_GUSB3PIPECTL0);
	wait_ms(100);

	reg = USB_TXPKTCNT_SEL | USB_TXPKTCNT | USB_MAX_TXBUTST_SIZE;
	writel(reg, HIUSB_XHCI_BASE + GTXTHRCFG);

	reg = USB_RXPKTCNT_SEL | USB_RXPKTCNT | USB_MAX_RXBUTST_SIZE;
	writel(reg, HIUSB_XHCI_BASE + GRXTHRCFG);
	wait_ms(20);

	local_irq_restore(flags);
}

void hiusb_stop_hcd(void)
{
	unsigned long flags;
	unsigned int reg;

	local_irq_save(flags);

	reg = readl(IO_REG_USB3_CTRL);
	writel(reg | (USB3_VCC_SRST_REQ2), IO_REG_USB3_CTRL);
	wait_ms(500);
	local_irq_restore(flags);
}
