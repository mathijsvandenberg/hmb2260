# HMB2260
Tools for loading custom firmware into a HMB2260v2 Box

## Prerequisites 
- binutils (MIPSEL), if you like to build your own firmware.
- gcc (MIPSEL), if you like to build your own firmware.
- python3
- pyserial
- FTDI USB to serial cable


### Download sources
```
wget http://ftp.gnu.org/gnu/binutils/binutils-2.42.tar.gz
wget http://ftp.gnu.org/gnu/gcc/gcc-14.1.0/gcc-14.1.0.tar.gz
```

### Build binutils
```
tar -zxvf binutils-2.42.tar.gz
cd binutils-2.42
./configure --target=mipsel-elf --prefix=/opt/cross --disable-nls
 make -j4
make install
```

### Build GCC
```
tar -zxvf gcc-14.1.0.tar.gz
cd gcc-14.1.0
./contrib/download_prerequisites
mkdir build && cd build
../configure --target=mipsel-elf --prefix=/opt/cross --disable-nls --enable-languages=c,c++ --without-headers
make all-gcc -j4
make all-target-libgcc -j4
make install-gcc
make install-target-libgcc
```

### Set PATH
```
export PATH=/opt/cross/bin:$PATH
```

## Building HMB2260 executable
just run `make`

## Starting the python script
```
python3 loader.py <serial port> <binary>
e.g. python3 loader.py COM10 hmb2260.bin

````
Make sure you get the relevant imports installed by pip such as `pyserial`.

## Pics or it didn't happen
![HBM2260 booting code](/images/hmb2260.gif)

```
python3 .\loader.py COM10 .\hmb2260.bin
Ready! Now powerup the HMB2260 port and apply the 'NAND glitch' shortly!

BCM74290010

M00CDZSL2=1
LLMB=0000C000
BP=02000000
L2Cfg=1E7AD627
L2Buf=77707711
BrCfg=F01C200E
BrMod=02800801
BrZp0=44000000
BrZp1=00002000
#@

BCM97241B0 CFE v3.22, Endian Mode: Little
Build Date: Wed Oct  2 14:18:55 CST 2013 (chih@JD3810-Linux-Compier)
Copyright (C) Broadcom Corporation.

ReShmoo MEMC0

CPU speed:            1305MHz
DDR Frequency:        796 MHz
DDR Mode:             DDR3
Total memory(MEMC 0): 1024MB
MEMC 0 DDR Width:     32
Boot Device:          NAND
Total flash:          512MB
RTS VERSION:          20120402210459_7241
ETH0_PHY:             INT
ETH0_MDIO_MODE:       1
ETH0_SPEED:           100
ETH0_PHYADDR:         1

Initializing USB.

CFE initialized.

[INFO] Get RBL Flag Data
    Reading flash0.RBL size 0x20000 offset 0xa0000 ...... Success
[INFO] Get DBL1 Header Data
    Reading flash0.DBL1 size 0x20000 offset 0x0 ...... Success
[INFO] Get DBL2 Header Data
    Reading flash0.DBL2 size 0x20000 offset 0x0 ...... Success
[INFO] All DBL state error, enter while loop!
[INFO] arcShowRunDblError DBL1 state 101 DBL2 state 101, no change DBL, mark BBT 0, Error Loop 1
CFE> Trigger!
Sending binary file by using srec...
boot -srec -nz uart0:hmb2260loader
Loader:srec Filesys:raw Dev:uart0 File:hmb2260loader Options:(null)
Loading: [ERROR] can't set IOCTL command 23
0x81001820 [100.06%]
Bye!
0193 lines
Entry address is 0x81000000
Starting program at 0x81000000

HMB2260 Loader v0.01 MvdB 2024
CHIP_FAMILY_ID : 0xB0404000: 10 00 29 74 
PRODUCT_ID     : 0xB0404004: 10 00 41 72
TIMER0_CTRL    : 0xB04066C8: 98 7F 33 C0
TIMER1_CTRL    : 0xB04066CC: 00 00 00 00
TIMER1_CTRL    : 0xB04066CC: 00 00 00 CF
TIMER2_CTRL    : 0xB04066D0: 00 00 00 00
TIMER0_STAT    : 0xB04066D8: 4F 3C 0E 40 
TIMER0_STAT    : 0xB04066D8: 4B BD 0F 40
TIMER0_STAT    : 0xB04066D8: 4E 3E 11 40
TIMER1_STAT    : 0xB04066DC: D3 AA 08 00
TIMER2_STAT    : 0xB04066E0: 00 00 00 00
UART0_LCR      : 0xB040670C: 03 00 00 00 
UART0_THR      : 0xB0406700: 0A 00 00 00 
UART0_IER      : 0xB0406704: 00 00 00 00
FLASH_DEV_ID   : 0xB0412994: 95 10 DC EC 
FLASH_DEV_ID_EX: 0xB0412998: DC EC EC 55 
Start Dump
0x00000000: xx xx xx <cut out real data> xx xx xx xx xx xx xx     .......Z..$@..<
0x00000010: xx xx xx <cut out real data> xx xx xx xx xx xx xx     .......Z..$@..<
... and more
```