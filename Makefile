include scripts/config.mk

.PHONY: all floppy_image kernel bootloader clean always test
all: floppy_image test

include scripts/toolchain.mk

#	FLOPPY DISK
FLOPPY_DIR=build/out/floppy.img
floppy_image: $(FLOPPY_DIR)

$(FLOPPY_DIR): bootloader kernel
	@dd if=/dev/zero of=$(FLOPPY_DIR) bs=512 count=2880
	@mkfs.fat -F 12 -n "NBOS" $@ >/dev/null
	@dd if=build/stage1.bin of=$@ conv=notrunc >/dev/null
	@mcopy -i $(FLOPPY_DIR) build/stage2.bin "::stage2.bin"
	@mcopy -i $(FLOPPY_DIR) build/kernel.bin "::kernel.bin"

#	BOOTLOADER
bootloader: stage1 stage2

stage1: build/stage1.bin
stage2: build/stage2.bin
build/stage1.bin: always
	@$(MAKE) -C src/bootloader/stage1 BUILD_DIR=$(abspath build)
build/stage2.bin: always
	@$(MAKE) -C src/bootloader/stage2 BUILD_DIR=$(abspath build)

#	KERNEL
kernel: build/kernel.bin
build/kernel.bin: always
	@$(MAKE) -C src/kernel BUILD_DIR=$(abspath build)

#	TEST
test: $(BUILD_DIR)/test
$(BUILD_DIR)/test: always
	@mkdir -p $(BUILD_DIR)/test
	@$(MAKE) -C test BUILD_DIR=$(abspath $(BUILD_DIR))

#	ALWAYS
always:
	@mkdir -p build/out

#	CLEAN
clean:
	$(MAKE) -C src/bootloader/stage1 BUILD_DIR=$(abspath build) clean
	$(MAKE) -C src/bootloader/stage2 BUILD_DIR=$(abspath build) clean
	$(MAKE) -C src/kernel BUILD_DIR=$(abspath build) clean
	rm -rf build/*
