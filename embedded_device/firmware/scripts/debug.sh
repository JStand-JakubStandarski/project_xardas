#!/bin/bash
#
# Author: Jakub Standarski
# Email: jstand.jakub.standarski@gmail.com
#
# Date: 14.01.2022
#

eval "make clean && make build"
BUILD_STATE=$?
if [ ${BUILD_STATE} -eq 0 ]; then
    echo "Build done successfully!"
else
    echo "Build failed... Exiting!"
    exit 1
fi

ELF_FILE=./build/project_xardas_firmware.elf
HEX_FILE=./build/project_xardas_firmware.hex



OPENOCD_VERSION=v0.11.0
OPENOCD_DIR=/opt/openocd/${OPENOCD_VERSION}
OPENOCD_BIN=${OPENOCD_DIR}/bin/openocd
OPENOCD_TARGET=${OPENOCD_DIR}/tcl/target/stm32l4x.cfg
OPENOCD_INTERFACE=${OPENOCD_DIR}/tcl/interface/stlink.cfg



TOOLCHAIN_VERSION=10-2020-q4-major
TOOLCHAIN_DIR=/opt/gnu-arm-embedded-toolchain/${TOOLCHAIN_VERSION}
TOOLCHAIN_DEBUGGER_BIN=${TOOLCHAIN_DIR}/bin/arm-none-eabi-gdb



${OPENOCD_BIN} -f ${OPENOCD_INTERFACE} -f ${OPENOCD_TARGET} > /dev/null \
2>/dev/null & ${TOOLCHAIN_DEBUGGER_BIN} ${ELF_FILE} \
 -ex "target remote localhost:3333" \
 -ex "monitor reset init" \
 -ex "monitor flash write_image erase ${HEX_FILE}" \
 -ex "monitor verify_image ${HEX_FILE}" \
 -ex "monitor reset halt"

killall openocd

