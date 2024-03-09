TARGET_CFLAGS += -ffreestanding -I.
TARGET_LINKFLAGS += -T linker.ld
TARGET_LIBS += -lgcc

HEADERS_C = $(shell find . -type f -name "*.h")
SOURCES_C = $(shell find . -type f -name "*.c")
OBJECTS_C = $(patsubst %.c, $(BUILD_DIR)/test/%.obj, $(SOURCES_C))

.PHONY: all clean always

all: $(BUILD_DIR)/test.out

$(BUILD_DIR)/test.out: $(OBJECTS_C)
	@$(CC) $(CFLAGS) -o $@ $(OBJECTS_C)

$(BUILD_DIR)/test/%.obj: %.c $(HEADERS_C)
	@mkdir -p $(@D)
	@$(CC) $(CFLAGS) -c -o $@ $<

clean:
	@rm -f $(BUILD_DIR)/test
