# u-boot-hi3516cv200

OpenIPC's U-Boot for HiSilicon V2-generation silicon: **Hi3516CV200** and
**Hi3518EV200** (ARM926EJ-S). Hi3518EV201 has a config and a register table
in tree but no published build.

Builds are published automatically to the
[OpenIPC/firmware `latest` release](https://github.com/OpenIPC/firmware/releases/tag/latest).

## Which image do I flash?

| board | DRAM | flash this | recover over UART with |
|---|---|---|---|
| Hi3516CV200 | DDR3 | `u-boot-hi3516cv200-universal.bin` | `u-boot-hi3516cv200-recovery.bin` |
| Hi3518EV200 | DDR2 | `u-boot-hi3518ev200-universal.bin` | `u-boot-hi3518ev200-recovery.bin` |
| Hi3518EV200 | DDR3 | `u-boot-hi3518ev200-ddr3-universal.bin` | `u-boot-hi3518ev200-ddr3-recovery.bin` |

`-universal` is the image you write to flash. `-recovery` is the same
bootloader built to fit the boot ROM's SRAM window, for uploading over UART
to a board that will not boot; do not flash it.

### Hi3518EV200 comes in both DDR2 and DDR3

**Check before you flash.** These two are not interchangeable — the DRAM type
is set by a register table baked into the image, so a DDR2 image on a DDR3
board (or the reverse) has no working memory at all. The symptom is a camera
that flashes and verifies perfectly and then boots nothing, which reads as a
bad flash or a dead chip and is neither. See
[OpenIPC/firmware#2299](https://github.com/OpenIPC/firmware/issues/2299).

From a working HiSilicon U-Boot prompt — the vendor's own bootloader will do,
you do not need OpenIPC running yet:

```
hisilicon # md.l 0x20111050 1
20111050: 00000016    ....
```

The last hex digit is the DRAM type, per `DMC_CFG_DRAM_TYPE_MASK` in
`drivers/ddr/ddr_ddrc_v500.h` (`[2:0]` 101=DDR2, 110=DDR3, 111=DDR4):

| last digit | DRAM | image |
|---|---|---|
| `5` | DDR2 | `u-boot-hi3518ev200-*.bin` |
| `6` | DDR3 | `u-boot-hi3518ev200-ddr3-*.bin` |

If you cannot get a prompt at all, read the part number off the DRAM package
and look it up — the datasheet states the type outright. The board in #2299
carried a Nanya `NT5CC128M16IP-DI`, a 2 Gbit DDR3L device, which agreed with
the register.

## Building

```sh
make hi3518ev200_config          # or hi3516cv200_config
cp reg_info_hi3518ev200.bin .reg # the DDR register table for the board
make CROSS_COMPILE=arm-hisiv510-linux-
make CROSS_COMPILE=arm-hisiv510-linux- mini-boot.bin
```

`mini-boot.bin` is assembled as 64 bytes of vectors, then the 4096-byte
register table from `.reg`, then code, then the LZMA-compressed U-Boot. The
table is copied in with `dd bs=4096 conv=sync` and nothing checksums it, so
selecting a different `.reg` is a table swap and nothing else — which is
exactly how the `-ddr3` variant is built, from `hi3518ev200_config` with
`reg_info_hi3516cv200.bin`.

Toolchain: `arm-hisiv510-linux` from
[OpenIPC/toolchains](https://github.com/OpenIPC/toolchains/releases/tag/v1).
`make mini-boot-recovery.bin` builds the UART-recovery variant.
