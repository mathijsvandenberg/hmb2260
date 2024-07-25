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
