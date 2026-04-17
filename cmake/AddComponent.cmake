include(ExternalProject)
include(AddTarget)

function(add_component project_number project_name toolchain)
    cmake_parse_arguments(TYPE "TEST" "" "" ${ARGN})
    set(toolchain_project_number ${project_number}-${toolchain})
    set(toolchain_file ${CMAKE_SOURCE_DIR}/cmake/toolchains/${toolchain}.cmake)
    set(directory_project ${CMAKE_SOURCE_DIR}/c/${project_number})
    set(directory_output_project ${CMAKE_BINARY_DIR}/${toolchain}/${project_number})
    set(toolchain_test ${project_number}-${toolchain}-test)
    set(directory_test ${CMAKE_SOURCE_DIR}/c/${project_number}/test)
    set(directory_output_test ${CMAKE_BINARY_DIR}/${toolchain}/${project_number}test)

    ExternalProject_Add(${toolchain_project_number}
            PREFIX ${directory_output_project}
            SOURCE_DIR ${directory_project}
            BINARY_DIR ${directory_output_project}
            CMAKE_GENERATOR "Unix Makefiles"
            CMAKE_ARGS
            -DCMAKE_TOOLCHAIN_FILE=${toolchain_file}
            -DCMAKE_MODULE_PATH=${CMAKE_SOURCE_DIR}/cmake
            -DCMAKE_C_FLAGS=-fno-omit-frame-pointer
            -B ${directory_output_project}
            BUILD_COMMAND make
            BUILD_ALWAYS ON
            INSTALL_COMMAND ""
    )

    if (${toolchain} MATCHES "riscv64-toolchain")
        add_custom_target(run_${project_number}
                DEPENDS ${toolchain_project_number}
                COMMAND qemu-system-riscv64
                -machine virt
                -nographic
                -serial mon:stdio
                -kernel ${directory_output_project}/${project_name}_kernel.elf
                WORKING_DIRECTORY ${directory_output_project}
        )

        if (TYPE_TEST)
            ExternalProject_Add(${toolchain_test}
                    PREFIX ${directory_output_test}
                    SOURCE_DIR ${directory_test}
                    BINARY_DIR ${directory_output_test}
                    CMAKE_GENERATOR "Unix Makefiles"
                    CMAKE_ARGS
                    -DCMAKE_TOOLCHAIN_FILE=${toolchain_file}
                    -DCMAKE_MODULE_PATH=${CMAKE_SOURCE_DIR}/cmake
                    -DCMAKE_C_FLAGS=-fno-omit-frame-pointer
                    -B ${directory_output_test}
                    BUILD_COMMAND make
                    BUILD_ALWAYS ON
                    INSTALL_COMMAND ""
            )

            add_custom_target(test_${project_number}
                    DEPENDS ${toolchain_test}
            )

            add_test(
                    NAME ${project_number}_integration
                    COMMAND qemu-system-riscv64
                    -machine virt
                    -nographic
                    -serial mon:stdio
                    -kernel ${directory_output_test}/${project_name}_test_kernel.elf
            )

            set_tests_properties(${project_number}_integration PROPERTIES
                    TIMEOUT 30
            )
        endif ()
    elseif (${toolchain} MATCHES "host")
        add_custom_target(run_${project_number}
                DEPENDS ${toolchain_project_number}
                COMMAND ${directory_output_project}/${project_name}
                WORKING_DIRECTORY ${directory_output_project}
        )
    endif ()

endfunction()