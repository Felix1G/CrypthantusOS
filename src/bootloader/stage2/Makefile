TARGET_ASMFLAGS += -f elf
TARGET_CFLAGS += -ffreestanding -nostdlib -I. -I../../libs
TARGET_LINKFLAGS += -T linker.ld -nostdlib
TARGET_LIBS += -lgcc

HEADERS_C = $(shell find . -type f -name "*.h")
HEADERS_ASM = $(shell find . -type f -name "*.inc")
SOURCES_C = $(shell find . -type f -name "*.c")
SOURCES_ASM = $(shell find . -type f -name "*.asm")
OBJECTS_C = $(patsubst %.c, $(BUILD_DIR)/stage2/c/%.obj, $(SOURCES_C))
OBJECTS_ASM = $(patsubst %.asm, $(BUILD_DIR)/stage2/asm/%.obj, $(SOURCES_ASM))

.PHONY: all stage2 clean always

all: stage2

stage2: $(BUILD_DIR)/stage2.bin

$(BUILD_DIR)/stage2.bin: $(OBJECTS_ASM) $(OBJECTS_C)
	@$(TARGET_LD) $(TARGET_LINKFLAGS) -Wl,-Map=$(BUILD_DIR)/stage2.map -o $@ $^ $(TARGET_LIBS)

$(BUILD_DIR)/stage2/c/%.obj: %.c $(HEADERS_C)
	@mkdir -p $(@D)
	@$(TARGET_CC) $(TARGET_CFLAGS) -c -o $@ $<
	@echo "compiled " $<

$(BUILD_DIR)/stage2/asm/%.obj: %.asm $(HEADERS_ASM)
	@mkdir -p $(@D)
	@$(TARGET_ASM) $(TARGET_ASMFLAGS) -o $@ $<
	@echo "compiled " $<

clean:
	@rm -f $(BUILD_DIR)/stage2.bin
