include(ExternalProject)

function(add_component project_number project_name toolchain generator)
    set(toolchain_project_number ${project_number}-${toolchain})
    set(toolchain_file ${CMAKE_SOURCE_DIR}/cmake/toolchains/${toolchain}.cmake)
    set(directory_output_project ${CMAKE_BINARY_DIR}/${toolchain}/${project_number})

    set(toolchain_tests ${project_number}-${toolchain}-tests)
    set(directory_tests ${CMAKE_SOURCE_DIR}/c/${project_number}/tests)
    set(directory_output_tests ${CMAKE_BINARY_DIR}/${toolchain}/${project_number}_tests)

    if (${generator} STREQUAL "Ninja")
        set(buildBin ninja)
    else ()
        set(buildBin make)
    endif ()

    ExternalProject_Add(${toolchain_project_number}
            PREFIX ${directory_output_project}
            SOURCE_DIR ${CMAKE_SOURCE_DIR}/c/${project_number}
            BINARY_DIR ${directory_output_project}
            CMAKE_GENERATOR ${generator}
            CMAKE_ARGS -DCMAKE_TOOLCHAIN_FILE=${toolchain_file} -B ${directory_output_project}
            BUILD_COMMAND ${buildBin}
            INSTALL_COMMAND ""
    )

    if (${toolchain} MATCHES "riscv")
        add_custom_target(run_${project_number}
                DEPENDS ${toolchain_project_number}
                COMMAND qemu-system-riscv64
                -machine virt
                -serial mon:stdio
                -nographic
                -kernel ${directory_output_project}/kernel.elf
                WORKING_DIRECTORY ${directory_output_project}
        )

        if (EXISTS ${directory_tests}/CMakeLists.txt)
            ExternalProject_Add(${toolchain_tests}
                    PREFIX ${directory_output_tests}
                    SOURCE_DIR ${directory_tests}
                    BINARY_DIR ${directory_output_tests}
                    CMAKE_GENERATOR ${generator}
                    CMAKE_ARGS -DCMAKE_TOOLCHAIN_FILE=${toolchain_file} -B ${directory_output_tests}
                    BUILD_COMMAND ${buildBin}
                    INSTALL_COMMAND ""
            )

            add_custom_target(test_${project_number}
                    DEPENDS ${toolchain_tests}
            )

            add_test(
                    NAME ${project_number}_integration
                    COMMAND qemu-system-riscv64
                    -machine virt
                    -nographic
                    -serial mon:stdio
                    -kernel ${directory_output_tests}/test_isis.elf
            )

            set_tests_properties(${project_number}_integration PROPERTIES
                    TIMEOUT 10
            )
        endif ()
    else ()
        add_custom_target(run_${project_number}
                DEPENDS ${toolchain_project_number}
                COMMAND ${directory_output_project}/${project_name}
                WORKING_DIRECTORY ${directory_output_project}
        )
    endif ()

endfunction()
