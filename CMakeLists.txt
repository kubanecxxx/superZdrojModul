cmake_minimum_required(VERSION 3.4)

project(zdroj_module)


#todo
#cmakefiles for my modules


set (USER_SOURCES 
main.c
board/board.c

remote/remote
remote/crc8

zdroj/zOpa
zdroj/zlock
zdroj/zLimit
zdroj/zdroj
zdroj/zDA
zdroj/zConverter
zdroj/zAD

)

set (EXTRA_HEADERS
halconf.h
chconf.h
mcuconf.h
platform.h
board/board.h

remote/remoteProtocol.h
)

set (OPTIMIZED_SOURCES

)

include_directories(
board
)

set (INCLUDE_DIRS
remote
port
zdroj


)

set (USER_PROGRAM_START 0x08002800)
set (USE_BOOTLOADER 0)

#set (LINKER_SCRIPT "${CMAKE_SOURCE_DIR}/STM32F100xB.ld")

set (MAIN_STACK_SIZE 0x400)
set (PROCESS_STACK_SIZE 0x400)
set (OPTIMIZE_RTOS 1)

set (GLOBAL_FLAGS
"-O0  -ggdb "
)

set (OPTIMIZED_FLAGS
"-O2"
)

set (CFLAGS

)

set (CPPFLAGS
"-fno-rtti -fno-exceptions"
)

#family or chip
set (CHIP "STM32F100C8")
#set (CHIP "STM32F407VG")
#chibios from https://github.com/kubanecxxx/chibios_fork commit hash
#4e1e344475204f1f255eb6fe78c55527988e502a
set (CHIBIOS_DIR "/home/kubanec/workspaces/chibios_3stable")
if (NOT EXISTS ${CHIBIOS_DIR})
	set (CHIBIOS_DIR "/home/kuba/chibi/chibios_fork")
endif()
#set (BOARD "ST_STM32F4_DISCOVERY")

set (FLASH_SIZE "64k")
set (RAM_SIZE "8k")

set (TOOLCHAIN "arm-none-eabi")


set(USE_COMMON_LIBRARY 1)
set(LIBRARY_MODULES scheduler)

set(USE_PIRIS 0)

include (${CHIBIOS_DIR}/CMakeLists.txt)
