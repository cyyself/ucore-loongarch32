EMPTY	:=
SPACE	:= $(EMPTY) $(EMPTY)
SLASH	:= /

V       := @

GCCPREFIX:= /home/cyy/use_for_linux/install/bin/loongarch32-linux-gnu-


QEMU:= /home/cyy/use_for_linux/qemu-system-loongson32
QEMUOPTS:= -M ls3a5k32 -monitor tcp::4288,server,nowait -serial stdio -m 256 -fsdev local,security_model=mapped,id=fsdev0,path=./ -device virtio-9p-pci,id=fs0,fsdev=fsdev0,mount_tag=hostshare,bus=ls7a.0 -nographic
# use the same qemu as use_for_linux

TERMINAL := gnome-terminal
TERMINALOPT := -e

# eliminate default suffix rules
.SUFFIXES: .c .S .h

# define compiler and flags

HOSTCC		:= gcc
HOSTCFLAGS	:= -g -Wall -O2

GDB		:= /home/cyy/use_for_linux/loongarch32-linux-gnu-gdb

CC :=$(GCCPREFIX)gcc
CFLAGS	:= -fno-builtin-fprintf -fno-builtin -nostdlib  -nostdinc -g -G0 -Wa,-O0 -fno-pic -mno-shared -mfp32 -ggdb -gstabs
CTYPE	:= c S

LD      := $(GCCPREFIX)ld
AS      := $(GCCPREFIX)as -g
AR      := $(GCCPREFIX)ar
LDFLAGS	+= -nostdlib

OBJCOPY := $(GCCPREFIX)objcopy
OBJDUMP := $(GCCPREFIX)objdump

COPY	:= cp
MKDIR   := mkdir -p
MV		:= mv
RM		:= rm -f
AWK		:= awk
SED		:= sed
SH		:= sh
TR		:= tr
TOUCH	:= touch -c

TAR		:= tar
ZIP		:= gzip

OBJDIR	:= obj
BINDIR	:= bin
SRCDIR  := kern
DEPDIR  := dep

MODULES   := init

SRC_DIR   := $(addprefix $(SRCDIR)/,$(MODULES))
BUILD_DIR := $(addprefix $(OBJDIR)/,$(MODULES))
DEP_DIR   := $(addprefix $(DEPDIR)/,$(MODULES))
VPATH     += $(SRC_DIR)

SRC       := $(foreach sdir,$(SRC_DIR),$(wildcard $(sdir)/*.c))
OBJ       := $(patsubst $(SRCDIR)/%.c, $(OBJDIR)/%.o, $(SRC))
ASMSRC    := $(foreach sdir,$(SRC_DIR),$(wildcard $(sdir)/*.S))
OBJ       += $(patsubst $(SRCDIR)/%.S, $(OBJDIR)/%.o, $(ASMSRC))
INCLUDES  := $(addprefix -I,$(SRC_DIR))
INCLUDES  += -I$(SRCDIR)/include

USER_APPLIST:= pwd cat sh ls forktest yield hello faultreadkernel faultread badarg waitkill pgdir exit sleep
INITRD_BLOCK_CNT:= 4000

# TODO: User

DEPENDS := $(patsubst $(SRCDIR)/%.c, $(DEPDIR)/%.d, $(SRC))

.PHONY: all checkdirs clean qemu debug

all: checkdirs $(OBJDIR)/ucore-kernel

$(shell mkdir -p $(DEP_DIR))

$(OBJDIR)/ucore-kernel:   $(OBJ) tools/kernel.ld
	@echo LINK $@
	$(LD) -nostdlib -n -G 0 -static -T tools/kernel.ld $(OBJ) -o $@

$(DEPDIR)/%.d: $(SRCDIR)/%.c
	@echo DEP $<
	@set -e; rm -f $@; \
		$(CC) -MM -MT "$(OBJDIR)/$*.o $@" $(CFLAGS) $(INCLUDES) $< > $@; 

$(OBJDIR)/%.o: $(SRCDIR)/%.c
	$(CC) -c $(INCLUDES) $(CFLAGS) $(MACH_DEF) $<  -o $@

$(OBJDIR)/%.o: $(SRCDIR)/%.S
	$(CC) -c -fno-pic -mno-shared -D__ASSEMBLY__ $(MACH_DEF) $(INCLUDES) -g -G0  $<  -o $@

checkdirs: $(BUILD_DIR) $(DEP_DIR)

$(BUILD_DIR):
	@mkdir -p $@

$(DEP_DIR):
	@mkdir -p $@

clean:
	-rm -rf $(DEPDIR)
	-rm -rf $(OBJDIR)

qemu: $(OBJDIR)/ucore-kernel
	$(QEMU) $(QEMUOPTS) -kernel $(OBJDIR)/ucore-kernel

debug: $(OBJDIR)/ucore-kernel
	$(QEMU) $(QEMUOPTS) -kernel $(OBJDIR)/ucore-kernel -S -s

gdb: $(OBJDIR)/ucore-kernel
	$(GDB) $(OBJDIR)/ucore-kernel

ifneq ($(MAKECMDGOALS),clean)
-include $(DEPENDS)
endif