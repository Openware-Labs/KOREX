NAME    := KOREX
VERSION := 0.1
ARCH    ?= x86_64

CC      := gcc
LD      := ld
NASM    := nasm
OBJCOPY := objcopy
QEMU    ?= qemu-system-x86_64
QEMUFLAGS ?=

BUILD     := build
ARCH_DIR  := arch/$(ARCH)
GEN_DIR   := $(BUILD)/include/generated
VERSION_H := $(GEN_DIR)/version.h

BOOT_BIN      := $(BUILD)/boot.bin
STAGE2_BIN    := $(BUILD)/stage2.bin
KERNEL_ELF    := $(BUILD)/kernel.elf
KERNEL_BIN    := $(BUILD)/kernel.bin
IMAGE         := $(BUILD)/korex.img
LINKER_SCRIPT := $(ARCH_DIR)/kernel.ld

IMAGE_SECTORS    := 2048
STAGE2_LBA       := 1
KERNEL_LBA       := 9
STAGE2_MAX_BYTES := 2048
KERNEL_MAX_BYTES := 32768

SRC_DIRS := $(ARCH_DIR)/kernel init kernel drivers lib usr
C_SRCS   := $(shell find $(SRC_DIRS) -name '*.c' | sort)
ASM_SRCS := $(shell find $(ARCH_DIR)/kernel -name '*.asm' | sort)
OBJS     := $(C_SRCS:%.c=$(BUILD)/%.o) $(ASM_SRCS:%.asm=$(BUILD)/%.o)
DEPS     := $(OBJS:.o=.d)

cc-option = $(shell $(CC) $(1) -Werror -x c -c /dev/null -o /dev/null 2>/dev/null && echo $(1))
ld-option = $(shell $(LD) $(1) -v >/dev/null 2>&1 && echo $(1))

INCLUDES := -Iinclude -I$(ARCH_DIR)/include -Iusr/include -I$(BUILD)/include

CFLAGS := -std=gnu11 -O2 -g -m64 -nostdinc -ffreestanding \
          -fno-stack-protector -fno-pic -fno-pie -fno-common \
          -fno-asynchronous-unwind-tables -mno-red-zone -mgeneral-regs-only \
          -Wall -Wextra -Wstrict-prototypes $(INCLUDES)
CFLAGS += $(call cc-option,-fno-tree-loop-distribute-patterns)
CFLAGS += $(call cc-option,-fcf-protection=none)
CFLAGS += $(call cc-option,-fno-stack-clash-protection)
ifeq ($(WERROR),1)
CFLAGS += -Werror
endif

LDFLAGS := -m elf_x86_64 -nostdlib -z noexecstack -z max-page-size=0x1000 \
           --build-id=none -T $(LINKER_SCRIPT)
LDFLAGS += $(call ld-option,--no-warn-rwx-segments)

ifeq ($(V),1)
Q :=
else
Q := @
endif

log = @printf '  %-8s%s\n' '$(1)' '$(2)'

.PHONY: all image run debug clean distclean help

all: image

image: $(IMAGE)

$(VERSION_H): Makefile
	@mkdir -p $(dir $@)
	$(call log,GEN,$@)
	$(Q)printf '#ifndef GENERATED_VERSION_H\n#define GENERATED_VERSION_H\n\n#define KOREX_NAME "%s"\n#define KOREX_VERSION "%s"\n\n#endif\n' '$(NAME)' '$(VERSION)' > $@

$(BUILD)/%.o: %.c | $(VERSION_H)
	@mkdir -p $(dir $@)
	$(call log,CC,$<)
	$(Q)$(CC) $(CFLAGS) -MMD -MP -c $< -o $@

$(BUILD)/%.o: %.asm
	@mkdir -p $(dir $@)
	$(call log,NASM,$<)
	$(Q)$(NASM) -f elf64 $< -o $@

$(KERNEL_ELF): $(OBJS) $(LINKER_SCRIPT)
	$(call log,LD,$@)
	$(Q)$(LD) $(LDFLAGS) -o $@ $(OBJS)

$(KERNEL_BIN): $(KERNEL_ELF)
	$(call log,OBJCOPY,$@)
	$(Q)$(OBJCOPY) -O binary $< $@
	$(Q)size=$$(stat -c %s $@); \
	if [ $$size -gt $(KERNEL_MAX_BYTES) ]; then \
		echo "error: $@ is $$size bytes, stage 2 loads at most $(KERNEL_MAX_BYTES)"; \
		rm -f $@; exit 1; \
	fi

$(BOOT_BIN): $(ARCH_DIR)/boot/boot.asm
	@mkdir -p $(dir $@)
	$(call log,NASM,$<)
	$(Q)$(NASM) -f bin $< -o $@

$(STAGE2_BIN): $(ARCH_DIR)/boot/stage2.asm
	@mkdir -p $(dir $@)
	$(call log,NASM,$<)
	$(Q)$(NASM) -f bin $< -o $@
	$(Q)size=$$(stat -c %s $@); \
	if [ $$size -gt $(STAGE2_MAX_BYTES) ]; then \
		echo "error: $@ is $$size bytes, stage 1 loads at most $(STAGE2_MAX_BYTES)"; \
		rm -f $@; exit 1; \
	fi

$(IMAGE): $(BOOT_BIN) $(STAGE2_BIN) $(KERNEL_BIN)
	$(call log,IMAGE,$@)
	$(Q)dd if=/dev/zero of=$@ bs=512 count=$(IMAGE_SECTORS) status=none
	$(Q)dd if=$(BOOT_BIN) of=$@ bs=512 conv=notrunc status=none
	$(Q)dd if=$(STAGE2_BIN) of=$@ bs=512 seek=$(STAGE2_LBA) conv=notrunc status=none
	$(Q)dd if=$(KERNEL_BIN) of=$@ bs=512 seek=$(KERNEL_LBA) conv=notrunc status=none

run: $(IMAGE)
	$(QEMU) $(QEMUFLAGS) -drive format=raw,file=$(IMAGE)

debug: $(IMAGE)
	$(QEMU) $(QEMUFLAGS) -drive format=raw,file=$(IMAGE) -s -S

clean:
	rm -rf $(BUILD)

distclean: clean
	rm -f kernel.bin kernel.elf os.img

help:
	@echo "Targets:"
	@echo "  all        build the disk image (default)"
	@echo "  run        build and boot the image in QEMU"
	@echo "  debug      boot in QEMU paused, GDB stub on :1234"
	@echo "  clean      remove the build directory"
	@echo "  distclean  clean and remove legacy root-level artifacts"
	@echo "Variables: V=1 (verbose), WERROR=1, QEMUFLAGS=..."

-include $(DEPS)
