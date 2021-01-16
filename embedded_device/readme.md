# Project XARDAS (Embedded device)

- [Introduction](#introduction)
- [Device overview](#device-overview)
    - [Development board](#development-board)
    - [Microcontroller specification](#microcontroller-specification)
    - [Programming model](#programming-model)
    - [WiFi module](#wifi-module)
- [Host system requirements](#host-system-requirements)
- [Directories hierarchy](#directories-hierarchy)
- [Programmers info](#programmers-info)



## INTRODUCTION

---
The embedded device, one of the `Project XARDAS` elements, is a small
electronic device responsible of collecting measurement data from the
environment (indoor or outdoor depending on device placement) and then sending
these data to the web application using external WiFi module.



## DEVICE OVERVIEW

---
So far, device is based on off-the-shelf development board `NUCLEO-L432KC`.
It's easy, fast and reliable way to develop and test firmware on a target
platform, i.e. `STM32L432KC` microcontroller. Nevertheless, there are plans to
design and assemble own PCB.


### DEVELOPMENT BOARD

**Development board**: `NUCLEO-L432KC`

![NUCLEO-L432KC dev board](./images/nucleo_l432kc.jpg)

![NUCLEO-L432KC pinout](./images/nucleo_l432kc_pinout.png)


### MICROCONTROLLER SPECIFICATION

**Microcontroller model**: `STM32L432KC`

**Key features**:

- Power supply: 1.71 [V] to 3.6 [V];

- Core: Arm 32-bit Cortex-M4 CPU with FPU

- Flash memory: 256 [kB]

- SRAM memory: 64 [kB]


### PROGRAMMING MODEL

**Programming model**: bare-metal (no OS/RTOS support)

**Drivers**: CMSIS and LL API from
[STM32CubeL4 MCU Package](https://github.com/STMicroelectronics/STM32CubeL4)


### WIFI MODULE

**WiFi module**: `ESP-01 ESP8266`


## HOST SYSTEM REQUIREMENTS

---
- **Operating system**: `Linux Ubuntu 20.04` or `Mac OS`

- **Toolchain**: `GNU Arm Embedded Toolchain (Version 10-2020-q4-major)`

- **Build tool**: `Make`



## DIRECTORIES HIERARCHY

---
`embedded_device` directory consists of the following subdirectories:

- `workspace`: collects all the files related strictly with the device
   firmware, i.e. CMSIS, LL API (peripheral drivers), application code,
   compiled binaries, etc.;

- `docs`: collects all the diagrams, architecture documents, planning
   documents, instructions, etc.;

- `images`: collects all the images like pinouts, device appearance and other
   images needed for different documents.



## PROGRAMMERS INFO

---
These are some basic information for programmers:

- **Style of code writing**: `snake_case` (directories and files are named
  using the same style).

