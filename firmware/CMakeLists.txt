cmake_minimum_required(VERSION 3.22)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
project(stm32l476x_setup C ASM)

set(CMAKE_C_COMPILER arm-none-eabi-gcc)
set(CMAKE_ASM_COMPILER arm-none-eabi-gcc)
set(CMAKE_OBJCOPY arm-none-eabi-objcopy)

set(PROCESSOR_FLAGS "-mcpu=cortex-m4 -mthumb -mfpu=fpv4-sp-d16 -mfloat-abi=hard")

set(COMILATION_FLAG "-DSTM32L476xx")
set(CMAKE_C_STANDARD 99)
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${PROCESSOR_FLAGS} ${COMILATION_FLAG} -Wall -Wextra -g -O0")

set(LINKER_SCRIPT ${CMAKE_SOURCE_DIR}/STM32L476RGTX_FLASH.ld)
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -specs=nosys.specs -T${LINKER_SCRIPT}")

file(GLOB_RECURSE SOURCE "src/*.c"
                         "./lib/tinyusb/src/tusb.c"
                         "./lib/tinyusb/src/host/*.c"
                         "./lib/tinyusb/src/common/*.c"
                         "./lib/tinyusb/src/class/hid/*.c"
                         "./lib/tinyusb/src/class/hid/hid_host.c"
                         "./lib/tinyusb/src/class/cdc/*.c"
                         "./lib/tinyusb/src/class/msc/*.c"
                         "./lib/tinyusb/src/osal/*.c"
                         "./src/drivers/*.c"
                         "./src/device/*.s"
                         "./src/device/*.c")

include_directories(src)
include_directories(src/drivers)
include_directories(src/device)
include_directories(lib/tinyusb/src)
include_directories(lib/tinyusb/src/class/hid)
include_directories(lib/tinyusb/src/host)
include_directories(lib/tinyusb/src/common)

add_executable(${PROJECT_NAME}.elf ${SOURCE})

