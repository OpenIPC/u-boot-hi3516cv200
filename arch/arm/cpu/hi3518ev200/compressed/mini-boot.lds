
OUTPUT_FORMAT("elf32-littlearm", "elf32-littlearm", "elf32-littlearm")
OUTPUT_ARCH(arm)
ENTRY(_start)

/* The bootrom copies this image into SRAM at 0x04010500, and SRAM ends at
 * 0x04014000.  Everything ahead of the compressed payload -- the vector
 * table, the 4 KB reg_info block and all mini-boot code -- therefore has to
 * fit in 0x3B00 bytes.  Overrunning it writes over the bootrom's own stack:
 * the chip stops answering mid-transfer and the board cannot be recovered
 * over UART at all.  See OpenIPC/firmware#2299.  */
MINI_BOOT_SRAM_WINDOW = 0x3B00;

/* The window is not a multiple of 1 KB, and the payload has to start on a
 * 1 KB boundary (see the ALIGN below), so the largest prelude that is both
 * loadable and not clipped is the window rounded down: 0x3800 (14336).  That
 * is the number to aim at, not 0x3B00.  */
MINI_BOOT_SRAM_TARGET = 0x3800;

/* MINI_BOOT_SIZE_CEILING is supplied by the Makefile with --defsym, because
 * the two variants built from these sources have different limits: the
 * recovery image is held to MINI_BOOT_SRAM_TARGET for real, while the flashed
 * image is not inside it yet (#5) and ratchets against its current size --
 * that number may only ever be lowered, never raised, so the gap cannot
 * silently grow again while it is being closed.  Linking without the --defsym
 * is an error rather than a default, so neither variant can lose its gate.  */

SECTIONS
{
	. = 0x00000000;

	. = ALIGN(4);
	.text	:
	{
		start.o	(.text .text.*)
		lowlevel_init_svb.o (.text .text.*)
		ddr_training_impl.o (.text .text.*)
		ddr_training_ctl.o (.text .text.*)
		ddr_training_boot.o (.text .text.*)
		ddr_training_custom.o (.text .text.*)
		*(.text .text.*)
	}
	__text_end = .;

	/* Start the payload on a 1 KB boundary.  Recovery tools find the end of
	 * the SRAM-resident program by scanning for the compressed header and
	 * rounding DOWN to 1 KB; without this padding that rounding clips the
	 * tail of the last function.  */
	. = ALIGN(1024);
	__image_start = .;
	.image : { *(.image) }

	. = ALIGN(4);
	.rodata : { *(SORT_BY_ALIGNMENT(SORT_BY_NAME(.rodata*))) }

	. = ALIGN(4);
	.data : { *(.data .data.*) }

	. = ALIGN(4);
	.got : { *(.got) }

	. = ALIGN(4);
	__bss_start = .;
	.bss : { *(.bss) }
	_end = .;
}

ASSERT(__image_start - ADDR(.text) <= MINI_BOOT_SIZE_CEILING,
	"mini-boot grew past MINI_BOOT_SIZE_CEILING. The target is MINI_BOOT_SRAM_TARGET (0x3800) -- anything above that cannot be recovered over UART at all -- so this must go down, not up. See OpenIPC/u-boot-hi3516cv200#5.")
