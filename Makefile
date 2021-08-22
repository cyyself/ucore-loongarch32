EMPTY	:=
SPACE	:= $(EMPTY) $(EMPTY)
SLASH	:= /

V       := @


TOOLCHAIN:=  $(HOME)/use_for_linux/install
GCCPREFIX:= $(TOOLCHAIN)/bin/loongarch32-linux-gnu-


QEMU:= $(HOME)/use_for_linux/qemu-system-loongson32
QEMUOPTS:= -M ls3a5k32 -monitor tcp::4288,server,nowait -serial stdio -m 256 -fsdev local,security_model=mapped,id=fsdev0,path=./ -device virtio-9p-pci,id=fs0,fsdev=fsdev0,mount_tag=hostshare,bus=ls7a.0 -nographic
# use the same qemu as use_for_linux

TERMINAL := gnome-terminal
TERMINALOPT := -e

# eliminate default suffix rules
.SUFFIXES: .c .S .h

# define compiler and flags

HOSTCC		:= gcc
HOSTCFLAGS	:= -g -Wall -O2

GDB		:= $(HOME)/use_for_linux/loongarch32-linux-gnu-gdb --data-directory=/usr/share/gdb

CC :=$(GCCPREFIX)gcc
CFLAGS	:= -fno-builtin-fprintf -fno-builtin -nostdlib  -nostdinc -g -G0 -Wa,-O0 -fno-pic -mno-shared -msoft-float -ggdb -gstabs -mlcsr
CTYPE	:= c S

LD      := $(GCCPREFIX)ld
AR      := $(GCCPREFIX)ar
LDFLAGS	+= -nostdlib -m elf32loongarch

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

MODULES   := init driver libs trap mm debug sync process schedule syscall fs fs/vfs fs/sfs fs/devs

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
INCLUDES  += -I$(TOOLCHAIN)/lib/gcc/loongarch32-linux-gnu/8.3.0/include


USER_APPLIST:= sh ls cat
INITRD_BLOCK_CNT:=700 
#USER_APPLIST:= pwd cat sh ls forktest yield hello faultreadkernel faultread badarg waitkill pgdir exit sleep
#INITRD_BLOCK_CNT:=4000

USER_SRCDIR := user
USER_OBJDIR := $(OBJDIR)/$(USER_SRCDIR)
USER_LIB_OBJDIR := $(USER_OBJDIR)/libs
USER_INCLUDE := -I$(USER_SRCDIR)/libs

USER_APP_BINS:= $(addprefix $(USER_OBJDIR)/, $(USER_APPLIST))

USER_LIB_SRCDIR := $(USER_SRCDIR)/libs
USER_LIB_SRC := $(foreach sdir,$(USER_LIB_SRCDIR),$(wildcard $(sdir)/*.c))
USER_LIB_OBJ := $(patsubst $(USER_LIB_SRCDIR)/%.c, $(USER_LIB_OBJDIR)/%.o, $(USER_LIB_SRC))
USER_LIB_OBJ += $(USER_LIB_OBJDIR)/initcode.o
USER_LIB    := $(USER_OBJDIR)/libuser.a

BUILD_DIR   += $(USER_LIB_OBJDIR)
BUILD_DIR   += $(USER_OBJDIR)


DEPENDS := $(patsubst $(SRCDIR)/%.c, $(DEPDIR)/%.d, $(SRC))

MAKEDEPEND = $(CLANG) -M $(CFLAGS) $(INCLUDES) -o $(DEPDIR)/$*.d $<
#vpath %.c $(SRC_DIR)
#vpath %.S $(SRC_DIR)

.PHONY: all checkdirs clean qemu debug

all: checkdirs $(OBJDIR)/ucore-kernel-initrd

$(shell mkdir -p $(DEP_DIR))

$(OBJDIR)/ucore-kernel:  checkdirs $(OBJ) tools/kernel.ld
	@echo LINK $@
	$(LD) -nostdlib -n -G 0 -static -T tools/kernel.ld $(OBJ) -o $@

obj/ucore-kernel-piggy: $(BUILD_DIR)  $(OBJ) $(USER_APP_BINS) tools/kernel.ld
	@echo LINK $@
	$(LD) -nostdlib -n -G 0 -static -T tools/kernel.ld $(OBJ) \
					$(addsuffix .piggy.o, $(USER_APP_BINS)) -o $@

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
	-rm -rf boot/loader.o boot/loader boot/loader.bin
	-rm -rf $(OBJDIR)

qemu: $(OBJDIR)/ucore-kernel-initrd
	$(QEMU) $(QEMUOPTS) -kernel $(OBJDIR)/ucore-kernel-initrd

debug: $(OBJDIR)/ucore-kernel-initrd
	$(QEMU) $(QEMUOPTS) -kernel $(OBJDIR)/ucore-kernel-initrd -S -s

gdb: $(OBJDIR)/ucore-kernel-initrd
	$(GDB) $(OBJDIR)/ucore-kernel-initrd

ifneq ($(MAKECMDGOALS),clean)
-include $(DEPENDS)
endif

#user lib

$(USER_LIB): $(BUILD_DIR) $(USER_LIB_OBJ)
	@echo "Building USERLIB"
	$(AR) rcs $@ $(USER_LIB_OBJ)

#user applications
define make-user-app
$1: $(BUILD_DIR) $(addsuffix .o,$1) $(USER_LIB)
	@echo LINK $$@
	$(LD) -T $(USER_LIB_SRCDIR)/user.ld  $(addsuffix .o,$1) --whole-archive $(USER_LIB) -o $$@
	$(SED) 's/$$$$FILE/$(notdir $1)/g' tools/piggy.S.in > $(USER_OBJDIR)/piggy.S
	$(CC) -c $(USER_OBJDIR)/piggy.S -o $$@.piggy.o
endef

$(foreach bdir,$(USER_APP_BINS),$(eval $(call make-user-app,$(bdir))))

$(USER_OBJDIR)/%.o: $(USER_SRCDIR)/%.c
	$(CC) -c  $(USER_INCLUDE) -I$(SRCDIR)/include $(CFLAGS)  $<  -o $@

$(USER_OBJDIR)/%.o: $(USER_SRCDIR)/%.S
	$(CC) -c -fno-pic -mno-shared -D__ASSEMBLY__ $(USER_INCLUDE) -I$(SRCDIR)/include -g -G0  $<  -o $@


# filesystem
TOOL_MKSFS := tools/mksfs
ROOTFS_DIR:= $(USER_OBJDIR)/rootfs
ROOTFS_IMG:= $(USER_OBJDIR)/initrd.img
$(TOOL_MKSFS): tools/mksfs.c
	$(HOSTCC) $(HOSTCFLAGS) -o $@ $^

$(OBJDIR)/ucore-kernel-initrd:  $(BUILD_DIR) $(TOOL_MKSFS) $(OBJ) $(USER_APP_BINS) tools/kernel.ld
	rm -rf $(ROOTFS_DIR) $(ROOTFS_IMG)
	mkdir $(ROOTFS_DIR)
	cp $(USER_APP_BINS) $(ROOTFS_DIR)
	cp -r $(USER_SRCDIR)/_archive/* $(ROOTFS_DIR)/
	dd if=/dev/zero of=$(ROOTFS_IMG) count=$(INITRD_BLOCK_CNT)
	#touch $(ROOTFS_IMG)
	$(TOOL_MKSFS) $(ROOTFS_IMG) $(ROOTFS_DIR)
	$(SED) 's%_FILE_%$(ROOTFS_IMG)%g' tools/initrd_piggy.S.in > $(USER_OBJDIR)/initrd_piggy.S
	$(CC) -c $(USER_OBJDIR)/initrd_piggy.S -o $(USER_OBJDIR)/initrd.img.o
	@echo LINK $@
	$(LD) -nostdlib -n -G 0 -static -T tools/kernel.ld $(OBJ) \
				 $(USER_OBJDIR)/initrd.img.o -o $@
	$(OBJDUMP) -S $@ > $(OBJDIR)/kernel.asm
	rm -rf $(ROOTFS_DIR)