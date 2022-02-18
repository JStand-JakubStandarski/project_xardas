#
# Author: Jakub Standarski
# Email: jstand.jakub.standarski@gmail.com
#
# Date: 02.02.2021
#

###############################################################################
# GENERAL INFO #
###############################################################################

PROJECT_NAME = project_xardas



###############################################################################
# TOOLCHAIN INFO #
###############################################################################

TOOLCHAIN_PATH = /opt/gnu-arm-embedded-toolchain/10-2020-q4-major



###############################################################################
# MICROCONTROLLER & CPU INFO #
###############################################################################

CPU = cortex-m4
MCU = STM32L432xx
MSI_VALUE_HZ = 4000000



###############################################################################
# APPLICATION INFO #
###############################################################################

APP_DIR = ./app

APP_HEADER_FILES = $(APP_DIR)/include
APP_SOURCE_FILES = $(wildcard $(APP_DIR)/source/*.c)



###############################################################################
# SERVICES INFO #
###############################################################################

SERVICES_DIR = ./services

SERVICES_HEADER_FILES = $(SERVICES_DIR)/include
SERVICES_SOURCE_FILES = $(wildcard $(SERVICES_DIR)/source/*.c)



###############################################################################
# FREERTOS INFO #
###############################################################################

FREERTOS_DIR = ./freertos

FREERTOS_HEADER_FILES = $(FREERTOS_DIR)/include
FREERTOS_SOURCE_FILES = $(wildcard $(FREERTOS_DIR)/source/*.c)



###############################################################################
# DRIVERS INFO #
###############################################################################

DRIVERS_CMSIS_CORE_DIR = ./drivers/cmsis/core

DRIVERS_CMSIS_CORE_HEADER_FILES = $(DRIVERS_CMSIS_CORE_DIR)/include


DRIVERS_CMSIS_DEVICE_DIR = ./drivers/cmsis/device

DRIVERS_CMSIS_DEVICE_HEADER_FILES = $(DRIVERS_CMSIS_DEVICE_DIR)/include
DRIVERS_CMSIS_DEVICE_SOURCE_FILES =\
$(wildcard $(DRIVERS_CMSIS_DEVICE_DIR)/source/*.c)
DRIVERS_CMSIS_DEVICE_SOURCE_FILES +=\
$(wildcard $(DRIVERS_CMSIS_DEVICE_DIR)/source/*.s)


DRIVERS_LL_API_DIR = ./drivers/peripherals_ll_api

DRIVERS_LL_API_HEADER_FILES = $(DRIVERS_LL_API_DIR)/include
DRIVERS_LL_API_SOURCE_FILES = $(wildcard $(DRIVERS_LL_API_DIR)/source/*.c)



###############################################################################
# COMPILER SETTINGS #
###############################################################################

WARNING_OPTIONS = -Wall -Wextra -Werror -Wdouble-promotion -Wconversion
WARNING_OPTIONS += -Wshadow -Wformat=2 -Wno-unused-parameter

DEBUGGING_OPTIONS = -g3

OPTIMIZATION_OPTIONS = -Os -ffunction-sections -fdata-sections

CODE_GENERATION_OPTIONS = -fno-common

ARM_OPTIONS = -mthumb -mlittle-endian -mcpu=$(CPU)
ARM_OPTIONS += -mfloat-abi=hard -mfpu=fpv4-sp-d16

PREPROCESSOR_OPTIONS = -D$(MCU) -DMSI_VALUE=$(MSI_VALUE_HZ)
PREPROCESSOR_OPTIONS += -DUSE_FULL_LL_DRIVER


CC = $(TOOLCHAIN_PATH)/bin/arm-none-eabi-gcc

CFLAGS = $(WARNING_OPTIONS)
CFLAGS += $(DEBUGGING_OPTIONS)
CFLAGS += $(OPTIMIZATION_OPTIONS)
CFLAGS += $(CODE_GENERATION_OPTIONS)
CFLAGS += $(ARM_OPTIONS)
CFLAGS += $(PREPROCESSOR_OPTIONS)



###############################################################################
# LINKER SETTINGS #
###############################################################################

LINKER_SCRIPT_FILE = ./linker/stm32l432kc_flash.ld

LINKER_OPTIONS = -Wl,--gc-sections
LINKER_OPTIONS += -Wl,-Map="$(BUILD_DIR)/$(PROJECT_NAME)_firmware.map"
LINKER_OPTIONS += -Wl,--print-memory-usage
LINKER_OPTIONS += -specs=nosys.specs
LINKER_OPTIONS += -lm


LD = $(TOOLCHAIN_PATH)/bin/arm-none-eabi-gcc

LDFLAGS = -T$(LINKER_SCRIPT_FILE)
LDFLAGS += $(LINKER_OPTIONS)



###############################################################################
# OBJCOPY SETTINGS #
###############################################################################

OBJ = $(TOOLCHAIN_PATH)/bin/arm-none-eabi-objcopy



###############################################################################
# BUILD PROCEDURE #
###############################################################################

BUILD_DIR = ./build

ELF_FILE = $(addprefix $(BUILD_DIR)/, $(PROJECT_NAME)_firmware.elf)
HEX_FILE = $(addprefix $(BUILD_DIR)/, $(PROJECT_NAME)_firmware.hex)
BIN_FILE = $(addprefix $(BUILD_DIR)/, $(PROJECT_NAME)_firmware.bin)


HEADER_FILES = -I./
HEADER_FILES += -I$(APP_HEADER_FILES)
HEADER_FILES += -I$(SERVICES_HEADER_FILES)
HEADER_FILES += -I$(FREERTOS_HEADER_FILES)
HEADER_FILES += -I$(DRIVERS_CMSIS_CORE_HEADER_FILES)
HEADER_FILES += -I$(DRIVERS_CMSIS_DEVICE_HEADER_FILES)
HEADER_FILES += -I$(DRIVERS_LL_API_HEADER_FILES)

SOURCE_FILES = $(APP_SOURCE_FILES)
SOURCE_FILES += $(SERVICES_SOURCE_FILES)
SOURCE_FILES += $(FREERTOS_SOURCE_FILES)
SOURCE_FILES += $(DRIVERS_CMSIS_DEVICE_SOURCE_FILES)
SOURCE_FILES += $(DRIVERS_LL_API_SOURCE_FILES)


build: $(ELF_FILE) $(HEX_FILE) $(BIN_FILE)

$(BIN_FILE): $(ELF_FILE)
	$(OBJ) -O binary $< $@

$(HEX_FILE): $(ELF_FILE)
	$(OBJ) -O ihex $< $@

$(ELF_FILE): $(SOURCE_FILES)
	mkdir -p $(BUILD_DIR)
	$(CC) $(CFLAGS) $(HEADER_FILES) $^ -o $@ $(LDFLAGS)


clean:
	rm -rf $(BUILD_DIR)

