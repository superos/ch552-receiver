# CH552 Makefile for CH55xDuino
# Fully mimics Arduino IDE compilation process

# Windows Only
APPDATA_LOCAL = C:/Users/[USER_NAME]/AppData/Local
ARDUINO_BUILD_TEMP = CH55xDuino_mcs51_ch552_usb_settings_user266_9f1c75ba7cb2da0b34c95f3f255f4f76

# Project name
PROJECT = ch552-receiver

# Paths
ARDUINO15 = $(APPDATA_LOCAL)/Arduino15
CH55X_ROOT = $(ARDUINO15)/packages/CH55xDuino/hardware/mcs51/0.0.25
TOOLS_ROOT = $(ARDUINO15)/packages/CH55xDuino/tools
MCS51_TOOLS = $(TOOLS_ROOT)/MCS51Tools/2023.10.10
SDCC_ROOT = $(TOOLS_ROOT)/sdcc/build.13407_4
CORE_LIB = $(APPDATA_LOCAL)/arduino/cores/$(ARDUINO_BUILD_TEMP)/core.lib

# Tools (using busybox wrapper like Arduino IDE)
BUSYBOX = $(MCS51_TOOLS)/win/busybox
SDCC_WRAPPER = $(MCS51_TOOLS)/wrapper/sdcc.sh
SDCC_BIN = $(SDCC_ROOT)/bin/sdcc
SDLD_BIN = $(SDCC_ROOT)/bin/sdld

# Build directory
BUILD_DIR = build

# Source files
SKETCH_SRC = $(PROJECT).ino
C_SRCS = \
	src/HID.c \
	src/LED.c \
	src/NRF.c \
	src/RF24/RF24.c \
	src/CdcHidCombo/USBCDC.c \
	src/CdcHidCombo/USBconstant.c \
	src/CdcHidCombo/USBhandler.c \
	src/CdcHidCombo/USBHIDKeyboard.c

# Compiler flags (exactly as Arduino IDE)
CFLAGS = \
	-MMD \
	-c \
	-Ddouble=float \
	-DUSE_STDINT \
	-D__PROG_TYPES_COMPAT__ \
	--model-large \
	--int-long-reent \
	--less-pedantic \
	--opt-code-size \
	-mmcs51 \
	-DCH552 \
	-DF_CPU=24000000L \
	-DF_EXT_OSC=0L \
	-DARDUINO=10607 \
	-DARDUINO_ch55x \
	-DARDUINO_ARCH_MCS51 \
	-DUSER_USB_RAM=266

# Include paths
INCLUDES = \
	-I$(CH55X_ROOT)/cores/ch55xduino \
	-I$(CH55X_ROOT)/variants/ch552 \
	-I$(SDCC_ROOT)/include

# Object files (all in build directory)
SKETCH_OBJ = $(BUILD_DIR)/$(SKETCH_SRC).c.rel
C_OBJS = $(addprefix $(BUILD_DIR)/, $(notdir $(C_SRCS:.c=.c.rel)))
ALL_OBJS = $(SKETCH_OBJ) $(C_OBJS)

# Linker script
LINK_SCRIPT = $(BUILD_DIR)/$(PROJECT).lk

# Default target
all: $(BUILD_DIR) $(BUILD_DIR)/$(PROJECT).hex

# Create build directory
$(BUILD_DIR):
	@mkdir -p $(BUILD_DIR)

# Convert .ino to .c
$(BUILD_DIR)/$(SKETCH_SRC).c: $(SKETCH_SRC)
	@echo Converting $(SKETCH_SRC) to c...
	@cp -f $(SKETCH_SRC) $@

# Compile sketch using busybox wrapper (mimicking Arduino IDE)
$(BUILD_DIR)/$(SKETCH_SRC).c.rel: $(BUILD_DIR)/$(SKETCH_SRC).c
	@echo Compiling $(SKETCH_SRC)...
	@cd $(BUILD_DIR) && $(BUSYBOX) ash $(SDCC_WRAPPER) $(SDCC_BIN) $(notdir $<) $(notdir $@) re2 $(CFLAGS) $(INCLUDES) -I..

# Compile C files using busybox wrapper
$(BUILD_DIR)/USBCDC.c.rel: src/CdcHidCombo/USBCDC.c
	@echo Compiling $<...
	@$(BUSYBOX) ash $(SDCC_WRAPPER) $(SDCC_BIN) $< $@ re1 $(CFLAGS) $(INCLUDES)

$(BUILD_DIR)/NRF.c.rel: src/NRF.c
	@echo Compiling $<...
	@$(BUSYBOX) ash $(SDCC_WRAPPER) $(SDCC_BIN) $< $@ re1 $(CFLAGS) $(INCLUDES)

$(BUILD_DIR)/HID.c.rel: src/HID.c
	@echo Compiling $<...
	@$(BUSYBOX) ash $(SDCC_WRAPPER) $(SDCC_BIN) $< $@ re1 $(CFLAGS) $(INCLUDES)

$(BUILD_DIR)/LED.c.rel: src/LED.c
	@echo Compiling $<...
	@$(BUSYBOX) ash $(SDCC_WRAPPER) $(SDCC_BIN) $< $@ re1 $(CFLAGS) $(INCLUDES)

$(BUILD_DIR)/RF24.c.rel: src/RF24/RF24.c
	@echo Compiling $<...
	@$(BUSYBOX) ash $(SDCC_WRAPPER) $(SDCC_BIN) $< $@ re1 $(CFLAGS) $(INCLUDES)

$(BUILD_DIR)/USBconstant.c.rel: src/CdcHidCombo/USBconstant.c
	@echo Compiling $<...
	@$(BUSYBOX) ash $(SDCC_WRAPPER) $(SDCC_BIN) $< $@ re1 $(CFLAGS) $(INCLUDES)

$(BUILD_DIR)/USBhandler.c.rel: src/CdcHidCombo/USBhandler.c
	@echo Compiling $<...
	@$(BUSYBOX) ash $(SDCC_WRAPPER) $(SDCC_BIN) $< $@ re1 $(CFLAGS) $(INCLUDES)

$(BUILD_DIR)/USBHIDKeyboard.c.rel: src/CdcHidCombo/USBHIDKeyboard.c
	@echo Compiling $<...
	@$(BUSYBOX) ash $(SDCC_WRAPPER) $(SDCC_BIN) $< $@ re1 $(CFLAGS) $(INCLUDES)

# Generate linker script (mimicking Arduino IDE)
$(LINK_SCRIPT): $(ALL_OBJS)
	@echo Generating linker script...
	@echo "-muwx" > $@
	@echo "-i $(BUILD_DIR)/$(PROJECT).ihx" >> $@
	@echo "-M" >> $@
	@echo "-X 0x02f6" >> $@
	@echo "-C 0x3800" >> $@
	@echo "-b HOME=0x0000" >> $@
	@echo "-b XSEG=0x010a" >> $@
	@echo "-b PSEG=0x010a" >> $@
	@echo "-b ISEG=0x0000" >> $@
	@echo "-b BSEG=0x0000" >> $@
	@echo "-k $(BUILD_DIR)" >> $@
	@echo "-k $(SDCC_ROOT)/lib/large_int_calc_stack_auto" >> $@
	@echo "-l $(CORE_LIB)" >> $@
	@echo "-l mcs51" >> $@
	@echo "-l libsdcc" >> $@
	@echo "-l liblong" >> $@
	@echo "-l liblonglong" >> $@
	@echo "-l libint" >> $@
	@echo "-l libfloat" >> $@
	@for obj in $(ALL_OBJS); do echo "$$obj" >> $@; done
	@echo "-e" >> $@

# Link using sdld (mimicking Arduino IDE)
$(BUILD_DIR)/$(PROJECT).ihx: $(LINK_SCRIPT)
	@echo Linking $(PROJECT)...
	@$(SDLD_BIN) -f $(LINK_SCRIPT)

# Generate hex file
$(BUILD_DIR)/$(PROJECT).hex: $(BUILD_DIR)/$(PROJECT).ihx
	@echo Creating hex file...
	@cp $< $@
	@cp $@ $(PROJECT).hex
	@echo Build complete: $(BUILD_DIR)/$(PROJECT).hex
	@echo Firmware ready: $(PROJECT).hex

# Clean build files
clean:
	@echo Cleaning build files...
	@rm -rf $(BUILD_DIR)

# Display help
help:
	@echo CH552 Makefile targets:
	@echo   all     - Build the project (default)
	@echo   clean   - Remove build files
	@echo   help    - Display this help message

.PHONY: all clean help
