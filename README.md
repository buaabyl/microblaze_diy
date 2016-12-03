# microblaze_diy
Using microblaze as toy, create most of thing from nothing.

The firmware size is small enough:) If you create from SDK, it will be 7000+ bytes!!!
Is waste of Block RAM!

Target
- create cross comipler from gcc source
- using Microblaze MCS and XPS
- create a demo without Xilinx SDK
- porting SDRAM interface
- create a bootloader with xmodem


## compile gcc
MUST build linux version first, because mingw32 need executable,
and microblaze-gcc and microblaze-as to create libgcc and newlib

```sh
$ lsb_release -a
Distributor ID: Ubuntu
Description:    Ubuntu 16.04 LTS
Release:        16.04
Codename:       xenial
Linux fish-vm 4.4.0-24-generic #43-Ubuntu SMP Wed Jun 8 19:27:37 UTC 2016 x86_64 x86_64 x86_64 GNU/Linux
gcc version 5.3.1 20160413 (Ubuntu 5.3.1-14ubuntu2.1)
Target: x86_64-linux-gnu

```

the building script
```sh
export MB_WORK=~/work

export MB_LINUX_DEP=${MB_WORK}/mb-linux-dep
export MB_LINUX_PREFIX=${MB_WORK}/mb-elf-toolchain-linux

export MB_WIN32_DEP=${MB_WORK}/mb-win32-dep
export MB_WIN32_PREFIX=${MB_WORK}/mb-elf-toolchain-win32

export MB_BUILD=x86_64-linux-gnu
export MB_HOST=i686-w64-mingw32
export MB_TARGET=microblaze-elf
export MB_PREFIX=mb-

export PATH=${MB_LINUX_PREFIX}/bin:$PATH

# prepare --------------------------------------------------
mkdir -p ${MB_WORK}/build-linux
mkdir -p ${MB_WORK}/build-w32
mkdir -p ${MB_WORK}/source

# download sources...
cd ${MB_WORK}/source
wget http://ftp.gnu.org/gnu/gmp/gmp-6.1.1.tar.xz
wget http://ftp.gnu.org/gnu/mpfr/mpfr-3.1.4.tar.xz
wget http://ftp.gnu.org/gnu/mpc/mpc-1.0.3.tar.gz
wget http://isl.gforge.inria.fr/isl-0.17.tar.xz
wget http://ftp.gnu.org/gnu/binutils/binutils-2.26.1.tar.bz2
wget http://ftp.gnu.org/gnu/gcc/gcc-5.4.0/gcc-5.4.0.tar.bz2
tar xvf gmp-6.1.1.tar.xz
tar xvf mpfr-3.1.4.tar.xz
tar xvf mpc-1.0.3.tar.gz
tar xvf isl-0.17.tar.xz
tar xvf binutils-2.26.1.tar.bz2
tar xvf gcc-5.4.0.tar.bz2

# linux ----------------------------------------------------
# gmp
cd ${MB_WORK}/build-linux
mkdir build-gmp
cd build-gmp
../../source/gmp-6.1.1/configure --prefix=${MB_LINUX_DEP}
make && make install

# mpfr
cd ${MB_WORK}/build-linux
mkdir build-mpfr
cd build-mpfr
../../source/mpfr-3.1.4/configure \
  --enable-static --disable-shared \
  --with-gmp=${MB_LINUX_DEP} \
  --prefix=${MB_LINUX_DEP}
make && make install

# mpc
cd ${MB_WORK}/build-linux
mkdir build-mpc
cd build-mpc
../../source/mpc-1.0.3/configure \
  --enable-static --disable-shared \
  --with-gmp=${MB_LINUX_DEP} \
  --with-mpfr=${MB_LINUX_DEP} \
  --prefix=${MB_LINUX_DEP}
make && make install

# isl
cd ${MB_WORK}/build-linux
mkdir build-isl
cd build-isl
../../source/isl-0.17/configure \
  --without-piplib \
  --enable-static --disable-shared \
  --with-gmp-prefix=${MB_LINUX_DEP} \
  --prefix=${MB_LINUX_DEP}
make && make install

# binutils
cd ${MB_WORK}/build-linux
mkdir build-binutils
cd build-binutils
../../source/binutils-2.26.1/configure --target=${MB_TARGET} \
  --prefix=${MB_LINUX_PREFIX} \
  --program-prefix=${MB_PREFIX}
make && make install

# bootstrap gcc
cd ${MB_WORK}/build-linux
mkdir build-gcc1
cd build-gcc1
../../source/gcc-5.4.0/configure \
  --target=${MB_TARGET} \
  --program-prefix=${MB_PREFIX} \
  --with-gnu-as --with-gnu-ld \
  --enable-static --disable-shared \
  --with-gmp=${MB_LINUX_DEP} \
  --with-mpfr=${MB_LINUX_DEP} \
  --with-mpc=${MB_LINUX_DEP} \
  --with-isl=${MB_LINUX_DEP} \
  --with-newlib \
  --without-headers \
  --enable-languages="c,c++" --enable-interwork --enable-multilib \
  --disable-decimal-float --disable-libffi \
  --disable-libgomp --disable-libmudflap --disable-libquadmath --disable-libssp --disable-libstdcxx-pch \
  --disable-nls --disable-shared --disable-threads --disable-tls \
  --prefix=${MB_LINUX_PREFIX}
make -j2 all-gcc
make install-gcc

# because newlib not support program-prefix, we need to create symbol link
cd ${MB_LINUX_PREFIX}/bin
ln -s mb-gcc mb-cc
ln -s mb-ar microblaze-elf-ar
ln -s mb-as microblaze-elf-as
ln -s mb-c++ microblaze-elf-c++
ln -s mb-gcc microblaze-elf-gcc
ln -s mb-gcc microblaze-elf-cc
ln -s mb-gcc-ar microblaze-elf-gcc-ar
ln -s mb-gcc-nm microblaze-elf-gcc-nm
ln -s mb-gcc-ranlib microblaze-elf-gcc-ranlib
ln -s mb-gcov microblaze-elf-gcov
ln -s mb-gcov-tool microblaze-elf-gcov-tool
ln -s mb-ld microblaze-elf-ld
ln -s mb-ld microblaze-elf-ld.bfd
ln -s mb-nm microblaze-elf-nm
ln -s mb-objcopy microblaze-elf-objcopy
ln -s mb-objdump microblaze-elf-objdump
ln -s mb-ranlib microblaze-elf-ranlib
ln -s mb-readelf microblaze-elf-readelf
ln -s mb-size microblaze-elf-size
ln -s mb-strings microblaze-elf-strings
ln -s mb-strip microblaze-elf-strip

# newlib
cd ${MB_WORK}/build-linux
mkdir build-newlib
cd build-newlib
../../source/newlib-2.4.0.20160527/configure \
  --target=${MB_TARGET} \
  --prefix=${MB_LINUX_PREFIX}
make && make install

# gcc
cd ${MB_WORK}/build-linux
mkdir build-gcc2
cd build-gcc2
../../source/gcc-5.4.0/configure \
  --target=${MB_TARGET} \
  --program-prefix=${MB_PREFIX} \
  --with-gnu-as --with-gnu-ld \
  --enable-static --disable-shared \
  --with-gmp=${MB_LINUX_DEP} \
  --with-mpfr=${MB_LINUX_DEP} \
  --with-mpc=${MB_LINUX_DEP} \
  --with-isl=${MB_LINUX_DEP} \
  --with-newlib \
  --enable-languages="c,c++" --enable-interwork --enable-multilib \
  --disable-decimal-float --disable-libffi \
  --disable-libgomp --disable-libmudflap --disable-libquadmath --disable-libssp --disable-libstdcxx-pch \
  --disable-nls --disable-shared --disable-threads --disable-tls \
  --prefix=${MB_LINUX_PREFIX}
make -j 2
make install

# mingw32 --------------------------------------------------
# binutils
cd ${MB_WORK}/build-w32
mkdir build-binutils
cd build-binutils
../../source/binutils-2.26.1/configure --target=${MB_TARGET} --build=${MB_BUILD} --host=${MB_HOST} \
  --prefix=${MB_WIN32_PREFIX} \
  --program-prefix=${MB_PREFIX}
make && make install

# gmp
cd ${MB_WORK}/build-w32
mkdir build-gmp
cd build-gmp
../../source/gmp-6.1.1/configure --build=${MB_BUILD} --host=${MB_HOST} \
  --prefix=${MB_WIN32_DEP}
make && make install

# mpfr
cd ${MB_WORK}/build-w32
mkdir build-mpfr
cd build-mpfr
../../source/mpfr-3.1.4/configure --build=${MB_BUILD} --host=${MB_HOST} \
  --enable-static --disable-shared \
  --with-gmp=${MB_WIN32_DEP} \
  --prefix=${MB_WIN32_DEP}
make && make install

# mpc
cd ${MB_WORK}/build-w32
mkdir build-mpc
cd build-mpc
../../source/mpc-1.0.3/configure --build=${MB_BUILD} --host=${MB_HOST} \
  --enable-static --disable-shared \
  --with-gmp=${MB_WIN32_DEP} \
  --with-mpfr=${MB_WIN32_DEP} \
  --prefix=${MB_WIN32_DEP}
make && make install

# isl
cd ${MB_WORK}/build-w32
mkdir build-isl
cd build-isl
../../source/isl-0.17/configure --build=${MB_BUILD} --host=${MB_HOST} \
  --enable-static --disable-shared \
  --with-gmp-prefix=${MB_WIN32_DEP} \
  --prefix=${MB_WIN32_DEP}
make && make install

# newlib no need to rebuild, just install
cd ${MB_WORK}/build-linux
cd build-newlib
mkdir -p ${MB_WIN32_PREFIX}
make install DESTDIR=${MB_WIN32_PREFIX}/tmp
cp ${MB_WIN32_PREFIX}/tmp${MB_LINUX_PREFIX}/microblaze-elf ${MB_WIN32_PREFIX} -r
rm -rf ${MB_WIN32_PREFIX}/tmp

# gcc
cd ${MB_WORK}/build-w32
mkdir build-gcc
cd build-gcc
../../source/gcc-5.4.0/configure --target=${MB_TARGET} --build=${MB_BUILD} --host=${MB_HOST} \
  --program-prefix=${MB_PREFIX} \
  --with-gnu-as --with-gnu-ld \
  --enable-static --disable-shared \
  --with-gmp=${MB_WIN32_DEP} \
  --with-mpfr=${MB_WIN32_DEP} \
  --with-mpc=${MB_WIN32_DEP} \
  --with-isl=${MB_WIN32_DEP} \
  --with-newlib \
  --enable-languages="c,c++" --enable-interwork --enable-multilib \
  --disable-decimal-float --disable-libffi \
  --disable-libgomp --disable-libmudflap --disable-libquadmath --disable-libssp --disable-libstdcxx-pch \
  --disable-nls --disable-shared --disable-threads --disable-tls \
  --prefix=${MB_WIN32_PREFIX}
make -j 2
make install
```

## build firmware from gcc not from Xilinx SDK
goto firmware and do this
```sh
Z:\microblaze_diy\firmware>make
assembly ../source/newlib/crt0.s
assembly ../source/libgcc/crti.s
assembly ../source/newlib/crtinit.s
assembly ../source/newlib/_exception_handler.s
assembly ../source/newlib/_hw_exception_handler.s
assembly ../source/newlib/_program_init.s
assembly ../source/newlib/_program_clean.s
compile  ../source/helloworld.c
assembly ../source/newlib/microblaze_disable_interrupts.s
assembly ../source/newlib/microblaze_enable_interrupts.s
assembly ../source/libgcc/crtn.s
create   helloworld.elf
   text    data     bss     dec     hex filename
   1516       0    2056    3572     df4 helloworld.elf
create   helloworld.lst
create   helloworld.bin
```

I copy source and startup codes from newlib,
and write driver myself. 























