include(ExternalProject)

function(add_multi_target_component srcRelPrefix project toolchainTuple generator exename)
    set(projectToolchainTuple ${project}-${toolchainTuple})
    set(toolchainFile ${CMAKE_SOURCE_DIR}/cmake/toolchains/${toolchainTuple}.cmake)
    set(outputDir ${CMAKE_BINARY_DIR}/${toolchainTuple}/${project})

    if (${generator} STREQUAL "Ninja")
        set(buildBin ninja)
    else ()
        set(buildBin make)
    endif ()

    ExternalProject_Add(${projectToolchainTuple}
            PREFIX ${outputDir}
            SOURCE_DIR ${CMAKE_SOURCE_DIR}/${srcRelPrefix}/${project}
            BINARY_DIR ${outputDir}
            CMAKE_GENERATOR ${generator}
            CMAKE_ARGS -DCMAKE_TOOLCHAIN_FILE=${toolchainFile} -B ${outputDir}
            BUILD_COMMAND ${buildBin}
            INSTALL_COMMAND ""
    )

    if (${toolchainTuple} MATCHES "riscv")
        add_custom_target(run_${project}
                DEPENDS ${projectToolchainTuple}
                COMMAND qemu-system-riscv64 -machine virt -bios none -serial mon:stdio -nographic -kernel ${outputDir}/kernel.elf
                WORKING_DIRECTORY ${outputDir}
        )
    else ()
        add_custom_target(run_${project}
                DEPENDS ${projectToolchainTuple}
                COMMAND ${outputDir}/${exename}
                WORKING_DIRECTORY ${outputDir}
        )
    endif ()
endfunction()