cmake_minimum_required(VERSION 3.15)

# Configure ARM Cortex toolchain
include(cmake/ToolchainARMCortex.cmake)

project(rivet_gun
    DESCRIPTION "Firmware for Rivet Gun Electronic"
    LANGUAGES CXX C)

# Project configuration
set(CMAKE_C_STANDARD 17)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_INTERPROCEDURAL_OPTIMIZATION TRUE)
set(CMAKE_BUILD_TYPE Release)
set(MCU_TYPE "CM0plus")
set(MCU_NAME "SAMD21E15L")
set(ATPACK_PATH "lib/MCU_SAMD21C")
set(LINKER_FILE "${ATPACK_PATH}/linker/samd21e15l_flash.ld")

include(cmake/PreventInSourceBuilds.cmake)
include(cmake/ARMFunctions.cmake)

# Include directories
include_directories(lib/CMSIS/include)
include_directories(${ATPACK_PATH}/include)

# Startup code from atpack
set(STARTUP_FILES
        ${ATPACK_PATH}/src/startup_samd21.c
        ${ATPACK_PATH}/src/system_samd21.c
        src/IR_transmitter.hpp)

add_library(startup ${STARTUP_FILES} src/IR_transmitter.hpp)
set_cpu_name(startup ${MCU_NAME})
set_compiler_c_flags(startup ${MCU_TYPE} ${LINKER_FILE})

# fmt
#set(FMT_OS OFF CACHE BOOL "Disable OS")
#add_subdirectory(lib/fmt)
#set_cpu_name(fmt SAME51J20A)
#set_compiler_cxx_flags(fmt ${MCU_TYPE} ${LINKER_FILE})
#target_compile_definitions(fmt  PRIVATE
#        FMT_STATIC_THOUSANDS_SEPARATOR=','
#        FMT_USE_NOEXCEPT=1
#        FMT_USE_FLOAT=0
#        FMT_USE_DOUBLE=0
#        FMT_USE_LONG_DOUBLE=0)

# Main app
set(SOURCE_FILES
        src/main.cpp
        src/System.hpp
        src/System.cpp
        )

set(LIB_FILES
        lib/peripherals/gpio.hpp
        lib/peripherals/gpio_definitions.hpp
        lib/peripherals/systick.hpp
        lib/peripherals/watchdog.hpp
)

add_arm_executable(${PROJECT_NAME} ${SOURCE_FILES} ${LIB_FILES})
set_cpu_name(${PROJECT_NAME} ${MCU_NAME})
set_compiler_cxx_flags(${PROJECT_NAME} ${MCU_TYPE} ${LINKER_FILE})
target_link_libraries(${PROJECT_NAME} startup)
target_include_directories(${PROJECT_NAME} PRIVATE lib/peripherals)
