function(set_cpu_name TARGET CPU_NAME)
    target_compile_definitions(${TARGET} PRIVATE -D__${CPU_NAME}__)
endfunction()

function(set_compiler_c_flags TARGET CPU_TYPE LINKER_SCRIPT_PATH)
    if("${CPU_TYPE}" STREQUAL "CM0")
        set(MCU_FLAGS -mcpu=cortex-m0 -mlittle-endian -mthumb -mfloat-abi=soft -mtune=cortex-m0)
    elseif("${CPU_TYPE}" STREQUAL "CM0plus")
        set(MCU_FLAGS -mcpu=cortex-m0plus -mlittle-endian -mthumb -mfloat-abi=soft -mtune=cortex-m0plus)
    elseif("${CPU_TYPE}" STREQUAL "CM4")
        set(MCU_FLAGS -mcpu=cortex-m4 -mlittle-endian -mthumb -mfloat-abi=soft -mtune=cortex-m4)
    elseif("${CPU_TYPE}" STREQUAL "CM4F")
        set(MCU_FLAGS -mcpu=cortex-m4 -mlittle-endian -mthumb -mfloat-abi=hard -mfpu=fpv4-sp-d16 -mtune=cortex-m4)
    else()
        message(FATAL_ERROR "Unknown CPU type")
    endif()

    set(WARNINGS
            -Wall
            -Wextra
            -Wunused
            -Wnull-dereference
            -Wduplicated-cond
            -Wduplicated-branches
            -Wshadow
            -Wcast-align
            -Wconversion
            -Wdouble-promotion
            -Wfloat-conversion
            -Wformat=2
            -Wmisleading-indentation
            -Wduplicated-cond
            -Wduplicated-branches
            -Wlogical-op
            -Wno-unknown-pragmas
            -Wstack-usage=128
    )

    set(COMPILE_FLAGS -ffunction-sections -fdata-sections)
    set(LINKER_FLAGS  -Wl,--gc-sections -specs=nano.specs -specs=nosys.specs -Wl,--wrap=malloc -Wl,--undefined=exception_table)
    set(LINKER_SCRIPT -Wl,-T${PROJECT_SOURCE_DIR}/${LINKER_SCRIPT_PATH})
    #set(DEBUG_FLAGS -g3)

    target_compile_options(${TARGET} PRIVATE ${MCU_FLAGS} ${COMPILE_FLAGS} ${WARNINGS})
    target_link_options(${TARGET} PRIVATE ${MCU_FLAGS} ${LINKER_FLAGS} ${LINKER_SCRIPT})
endfunction()

function(set_compiler_cxx_flags TARGET CPU_TYPE LINKER_SCRIPT_PATH)
    if("${CPU_TYPE}" STREQUAL "CM0")
        set(MCU_FLAGS -mcpu=cortex-m0 -mlittle-endian -mthumb -mfloat-abi=soft -mtune=cortex-m0)
    elseif("${CPU_TYPE}" STREQUAL "CM0plus")
        set(MCU_FLAGS -mcpu=cortex-m0plus -mlittle-endian -mthumb -mfloat-abi=soft -mtune=cortex-m0plus)
    elseif("${CPU_TYPE}" STREQUAL "CM4")
        set(MCU_FLAGS -mcpu=cortex-m4 -mlittle-endian -mthumb -mfloat-abi=soft -mtune=cortex-m4)
    elseif("${CPU_TYPE}" STREQUAL "CM4F")
        set(MCU_FLAGS -mcpu=cortex-m4 -mlittle-endian -mthumb -mfloat-abi=hard -mfpu=fpv4-sp-d16 -mtune=cortex-m4)
    else()
        message(FATAL_ERROR "Unknown CPU type")
    endif()

    set(WARNINGS
            -Wall
            -Wextra
            -Wunused
            -Wnull-dereference
            -Wduplicated-cond
            -Wduplicated-branches
            -Wshadow
            -Wcast-align
            -Wconversion
            -Wdouble-promotion
            -Wfloat-conversion
            -Wformat=2
            -Wmisleading-indentation
            -Wduplicated-cond
            -Wduplicated-branches
            -Wlogical-op
            -Wno-unknown-pragmas
            -Wstack-usage=128
            )

    set(WARNINGS_CXX
            -Wpedantic
            -Wnon-virtual-dtor
            -Woverloaded-virtual
            -Wno-volatile
            #-Wold-style-cast
            #-Wuseless-cast
            )

    set(CMAKE_C_FLAGS "")
    set(CMAKE_CXX_FLAGS "")

    set(COMPILE_FLAGS -ffunction-sections -fdata-sections)
    set(COMPILE_FLAGS_CXX -fno-rtti -fno-exceptions -fno-non-call-exceptions -fno-threadsafe-statics -fno-unwind-tables -fno-use-cxa-atexit -ffreestanding)
    set(LINKER_FLAGS  -Wl,--gc-sections -specs=nano.specs -specs=nosys.specs -Wl,--wrap=malloc -Wl,--undefined=exception_table -Wl,--print-memory-usage)
    set(LINKER_SCRIPT -Wl,-L${PROJECT_SOURCE_DIR}/linker -Wl,-T${PROJECT_SOURCE_DIR}/${LINKER_SCRIPT_PATH})
    #set(DEBUG_FLAGS -g3)

    target_compile_options(${TARGET} PRIVATE ${MCU_FLAGS} ${COMPILE_FLAGS} ${COMPILE_FLAGS_CXX} ${WARNINGS} ${WARNINGS_CXX})
    target_link_options(${TARGET} PRIVATE ${MCU_FLAGS} ${LINKER_FLAGS} ${LINKER_SCRIPT})
endfunction()

function(add_arm_executable TARGET)
    set(OUT_NAME ${TARGET}.elf)
    set(HEX_NAME ${TARGET}.hex)
    set(BIN_NAME ${TARGET}.bin)

    add_executable(${TARGET} ${ARGN})

    set_target_properties(${TARGET} PROPERTIES OUTPUT_NAME ${TARGET} SUFFIX ".elf")

    add_custom_command(TARGET ${TARGET} POST_BUILD
            COMMAND arm-none-eabi-size ${OUT_NAME}
            COMMENT "Size of ${OUT_NAME}")

    add_custom_command(TARGET ${TARGET} POST_BUILD
            COMMAND ${CMAKE_OBJCOPY} -Oihex ${OUT_NAME} ${HEX_NAME}
            COMMENT "Generating ${HEX_NAME}")

    add_custom_command(TARGET ${TARGET} POST_BUILD
            COMMAND ${CMAKE_OBJCOPY} -Obinary ${OUT_NAME} ${BIN_NAME}
            COMMENT "Generating ${BIN_NAME}")
endfunction()
