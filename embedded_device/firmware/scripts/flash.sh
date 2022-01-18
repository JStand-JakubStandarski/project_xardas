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



OPENOCD_VERSION=v0.11.0
OPENOCD_DIR=/opt/openocd/${OPENOCD_VERSION}
OPENOCD_BIN=${OPENOCD_DIR}/bin/openocd
OPENOCD_TARGET=${OPENOCD_DIR}/tcl/target/stm32l4x.cfg
OPENOCD_INTERFACE=${OPENOCD_DIR}/tcl/interface/stlink.cfg



${OPENOCD_BIN} -f ${OPENOCD_INTERFACE} -f ${OPENOCD_TARGET} -c targets \
    -c "program ${ELF_FILE} verify reset exit"

