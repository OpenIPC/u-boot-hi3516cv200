#define HIUSB_OHCI_DEV_NAME	"hiusb-ohci"

#define IO_REG_USB2_CTRL	(CRG_REG_BASE + REG_USB2_CTRL)
#define USB2_BUS_CKEN		(1 << 0)
#define USB2_OHCI48M_CKEN	(1 << 1)
#define USB2_OHCI12M_CKEN	(1 << 2)
#define USB2_HST_PHY_CKEN	(1 << 4)
#define USB2_UTMI0_CKEN		(1 << 5)
#define USB2_UTMI1_CKEN		(1 << 6)
#define USB2_BUS_SRST_REQ	(1 << 12)
#define USB2_UTMI0_SRST_REQ	(1 << 13)
#define USB2_UTMI1_SRST_REQ	(1 << 14)
#define USB2_HST_PHY_SYST_REQ	(1 << 16)

#define IO_REG_USB2_PHY0	(CRG_REG_BASE + REG_USB2_PHY0)
#define USB_PHY0_REF_CKEN	(1 << 0)
#define USB_PHY0_SRST_REQ	(1 << 1)
#define USB_PHY0_SRST_TREQ	(1 << 2)
#define USB_PHY1_SRST_TREQ	(1 << 3)
#define USB_PHY0_TEST_SRST_REQ	(1 << 4)

#define IO_REG_USB2_CTRL1	(MISC_REG_BASE + REG_USB2_CTRL1)
#define USB2_MISC_CTRL1_VALUE    0x800000
#define USB2_MISC_CTRL1_VALUE2   0xa0060c

static void hiusb_ohci_enable_clk(void)
{
	int reg;

	/* reset enable */
	reg = readl(IO_REG_USB2_CTRL);
	reg |= (USB2_BUS_SRST_REQ
			| USB2_UTMI0_SRST_REQ
			| USB2_UTMI1_SRST_REQ
			| USB2_HST_PHY_SYST_REQ);

	writel(reg, IO_REG_USB2_CTRL);
	udelay(200);

	reg = readl(IO_REG_USB2_PHY0);
	reg |= (USB_PHY0_SRST_REQ
			| USB_PHY0_SRST_TREQ
			| USB_PHY1_SRST_TREQ);
	writel(reg, IO_REG_USB2_PHY0);
	udelay(200);
	/* for ssk usb storage ok */
	mdelay(20);

	/* open ref clock */
	reg = readl(IO_REG_USB2_PHY0);
	reg |= USB_PHY0_REF_CKEN;
	writel(reg, IO_REG_USB2_PHY0);
	udelay(100);

	/* cancel power on reset */
	reg = readl(IO_REG_USB2_PHY0);
	reg &= ~(USB_PHY0_SRST_REQ);
	reg &= ~(USB_PHY0_TEST_SRST_REQ);
	writel(reg , IO_REG_USB2_PHY0);
	udelay(300);

	/* cancel port reset */
	reg = readl(IO_REG_USB2_PHY0);
	reg &= ~(USB_PHY0_SRST_TREQ);
	reg &= ~(USB_PHY1_SRST_TREQ);
	writel(reg, IO_REG_USB2_PHY0);
	udelay(300);

	/* cancel control reset */
	reg = readl(IO_REG_USB2_CTRL);
	reg &= ~(USB2_BUS_SRST_REQ
			| USB2_UTMI0_SRST_REQ
			| USB2_UTMI1_SRST_REQ
			| USB2_HST_PHY_SYST_REQ);

	reg |= (USB2_BUS_CKEN
			| USB2_OHCI48M_CKEN
			| USB2_OHCI12M_CKEN
			| USB2_HST_PHY_CKEN
			| USB2_UTMI0_CKEN
			| USB2_UTMI1_CKEN);
	writel(reg, IO_REG_USB2_CTRL);
	udelay(200);

	writel(USB2_MISC_CTRL1_VALUE, IO_REG_USB2_CTRL1);
	mdelay(10);
	writel(USB2_MISC_CTRL1_VALUE2, IO_REG_USB2_CTRL1);
	mdelay(10);
}

static void hiusb_ohci_disable_clk(void)
{
	int reg;

	reg = readl(IO_REG_USB2_PHY0);
	reg |= (USB_PHY0_SRST_REQ
			| USB_PHY0_SRST_TREQ
			| USB_PHY1_SRST_TREQ);
	writel(reg, IO_REG_USB2_PHY0);
	udelay(100);

	/* close clock */
	reg = readl(IO_REG_USB2_PHY0);
	reg &= ~USB_PHY0_REF_CKEN;
	writel(reg, IO_REG_USB2_PHY0);
	udelay(300);

	/* close clock */
	reg = readl(IO_REG_USB2_CTRL);
	reg &= ~(USB2_BUS_CKEN
			| USB2_OHCI48M_CKEN
			| USB2_OHCI12M_CKEN
			| USB2_HST_PHY_CKEN
			| USB2_UTMI0_CKEN
			| USB2_UTMI1_CKEN);
	writel(reg, IO_REG_USB2_CTRL);
	udelay(200);
}
