#support GCC compiler:
#- gcc version 4.6.4 20120924 (Xilinx 14.1 Build EDK_P.13 28 Sep 2013) (GCC)
#	Using built-in specs.
#	COLLECT_GCC=mb-gcc
#	COLLECT_LTO_WRAPPER=c:/xilinx/14.7/ise_ds/edk/gnu/microblaze/nt/bin/../libexec/g
#	cc/microblaze-xilinx-elf/4.6.4/lto-wrapper.exe
#	Target: microblaze-xilinx-elf
#
#- gcc version 5.4.0 (GCC)
#	Using built-in specs.
#	COLLECT_GCC=mb-gcc
#	COLLECT_LTO_WRAPPER=c:/gcc/mbgcc/bin/../libexec/gcc/microblaze-elf/5.4.0/lto-wrapper.exe
#	Target: microblaze-elf
#	Configured with: ../../build/gcc-5.4.0/configure 
#		--program-prefix=mb- 
#		--target=microblaze-elf 
#		--build=x86_64-linux-gnu 
#		--host=i686-w64-mingw32 
#		--prefix=/home/fish/work/mb-elf-toolchain-win32
#		--with-gnu-as 
#		--with-gnu-ld 
#		--enable-static 
#		--disable-shared 
#		--with-gmp=/home/fish/work/mb-elf-toolchain-win32-dep 
#		--with-mpfr=/home/fish/work/mb-elf-toolchain-win32-dep 
#		--with-mpc=/home/fish/work/mb-elf-toolchain-win32-dep 
#		--with-isl=/home/fish/work/mb-elf-toolchain-win32-dep 
#		--with-newlib 
#		--enable-languages=c,c++ 
#		--enable-interwork 
#		--enable-multilib 
#		--disable-decimal-float 
#		--disable-libffi 
#		--disable-libgomp 
#		--disable-libmudflap 
#		--disable-libquadmath 
#		--disable-libssp 
#		--disable-libstdcxx-pch 
#		--disable-nls 
#		--disable-tls 
#		--disable-threads 
#	Thread model: single
#	gcc version 5.4.0 (GCC)
#
DATA2MEM = C:/Xilinx/14.7/ISE_DS/ISE/bin/nt/data2mem.exe
PROJECT	 = helloworld
OBJS_PATH = objs

BIT_FILE = ../project/top.bit
BMM_FILE = ../source/cpu_bd.bmm
LDSCRIPT = ../source/xilinx.ld

VPATH 	+= ../source
VPATH 	+= ../source/newlib
VPATH 	+= ../source/libgcc


# ----------------------------------------------------------
# !!! Microblaze MCS only accept this !!!
#
# 		-m-soft-float			software implement 
# 		-mxl-soft-mul			software implement 
# 		-mxl-soft-div			software implement 
# 		-mno-xl-barrel-shift	shift 1 bit a time	
#		-mno-xl-reorder			not support reorder
#
# !!! don't try other flags !!!
#
# other flags must test in XPS with custom microblaze setting!
# you can add iomodule to microblaze in XPS,
# and may it work like Microblaze MCS:)
#
# When I want to use XPS, I always changed to other solution.
# 1. Cortex-M4 (STM32F4) with FPGA.
# 2. Cortex-A9 (Freescall i.MX6) with FPGA.
# 3. Zynq (too expensive).
#
# The Microblaze MCS is the simplest XPS one,
# which easier for verilog logic with string processing,
# such like UART configrable need. most of time it is suitable.
# If too simple, using Picoblaze instead.
#
# But Xilinx SDK generated framework is too big!
# So I wrote a simple one (this project:)
#
CFLAGS 	+= -mlittle-endian -mcpu=v8.40.a -Os -I include
CFLAGS  += -nostartfiles
CFLAGS  += -nodefaultlibs


# --------------------------------------
# microblaze.C_USE_REORDER_INSTR=0
#
CFLAGS  += -mno-xl-reorder

# --------------------------------------
# microblaze.C_USE_HW_MUL=1
# -mxl-soft-mul 		default
# -mno-xl-soft-mul
#
CFLAGS  += -mxl-soft-mul
#CFLAGS  += -mno-xl-soft-mul

# --------------------------------------
# microblaze.C_USE_DIV=1
# -mxl-soft-div			default
# -mnoxl-soft-div
#
CFLAGS  += -mxl-soft-div
#CFLAGS  += -mno-xl-soft-div

# --------------------------------------
# microblaze.C_USE_FPU=1
# -msoft-float			default
# -mhard-float
#
CFLAGS  += -msoft-float
#CFLAGS  += -mhard-float

# microblaze.C_USE_FPU=2
# -mxl-float-convert
# -mxl-float-sqrt
#
#CFLAGS  += -mxl-float-convert
#CFLAGS  += -mxl-float-sqrt

# --------------------------------------
# microblaze.C_USE_BARREL=1
# -mxl-barrel-shift
#
CFLAGS  += -mno-xl-barrel-shift
#CFLAGS  += -mxl-barrel-shift


# ----------------------------------------------------------
LDFLAGS += -L . 

# xilinx suggest 0x00-0x4F as vector
# I use 2KB (0x00800) stack
LDFLAGS += -Wl,--defsym=_TEXT_START_ADDR=0x050
LDFLAGS += -Wl,--defsym=_HEAP_SIZE=0x00000
LDFLAGS += -Wl,--defsym=_STACK_SIZE=0x00800
LDFLAGS += -Wl,-Map=$(PROJECT).map \
		   -Wl,--no-relax \
		   -Wl,--gc-sections 
LDFLAGS += -T $(LDSCRIPT)


# ----------------------------------------------------------
# r: replace exists or insert new
# c: create target if need
# v: verbose
ARFLAGS = rc


# ----------------------------------------------------------
C_OBJS0	= $(OBJS_PATH)/crt0.s.o \
		  $(OBJS_PATH)/crti.s.o \
		  $(OBJS_PATH)/crtinit.s.o \
		  $(OBJS_PATH)/_exception_handler.s.o \
		  $(OBJS_PATH)/_hw_exception_handler.s.o \
		  $(OBJS_PATH)/_program_init.s.o \
		  $(OBJS_PATH)/_program_clean.s.o \

C_OBJS	= $(OBJS_PATH)/helloworld.c.o \
		  $(OBJS_PATH)/microblaze_disable_interrupts.s.o \
		  $(OBJS_PATH)/microblaze_enable_interrupts.s.o 

C_OBJSN = $(OBJS_PATH)/crtn.s.o 


# ----------------------------------------------------------
all:$(OBJS_PATH) $(PROJECT).lst $(PROJECT).bin

$(OBJS_PATH):
	@mkdir.exe $@

bit:$(PROJECT).elf $(DATA2MEM) $(BMM_FILE) $(BIT_FILE)
	$(DATA2MEM) \
		-bm $(BMM_FILE) \
		-bt $(BIT_FILE) \
		-bd $< tag cpu \
		-o b app.bit

$(PROJECT).lst:$(PROJECT).elf
	@echo create   $@
	@mb-objdump -d $< > $@

$(PROJECT).bin:$(PROJECT).elf
	@echo create   $@
	@mb-objcopy -O binary $< $@

$(PROJECT).elf:$(C_OBJS0) $(C_OBJS) $(C_OBJSN)
	@echo create   $@
	@mb-gcc $(CFLAGS) $(LDFLAGS) $^ -o $@
	@mb-size $@

# --------------------------------------
$(OBJS_PATH)/%.c.o:%.c
	@echo compile  $<
	@mb-gcc $(CFLAGS) -S $< -o $@.s
	@mb-gcc $(CFLAGS) -c $< -o $@

$(OBJS_PATH)/%.s.o:%.s
	@echo assembly $<
	@mb-gcc $(CFLAGS) -c $< -o $@

clean:
	rm -rf $(OBJS_PATH)

cleanall:clean
	rm -rf *.elf
	rm -rf *.bin
	rm -rf *.map
	rm -rf *.lst
	rm -rf *.bit


