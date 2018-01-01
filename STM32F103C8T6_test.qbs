import qbs
import qbs.FileInfo
import qbs.File

Project {
    CppApplication {
        consoleApplication: true

        property path chRoot: "/home/alex/Dev/ChibiOS"
        property path startupLd: chRoot + "/os/common/startup/ARMCMx/compilers/GCC/ld"
        property string startupLdScript: "STM32F103x8.ld"
        property string coreName: "STM32F1xx"

        property pathList commonSearch: [
                ".",
                // board.mk
                chRoot + "/os/hal/boards/STM32F103C8_MINIMAL",
                // osal.mk
                chRoot + "/os/hal/osal/rt",
                // platform.mk
                chRoot + "/os/hal/ports/STM32/LLD/CANv1",
                chRoot + "/os/hal/ports/STM32/LLD/DACv1",
                chRoot + "/os/hal/ports/STM32/LLD/DMAv1",
                chRoot + "/os/hal/ports/STM32/LLD/EXTIv1",
                chRoot + "/os/hal/ports/STM32/LLD/GPIOv1",
                chRoot + "/os/hal/ports/STM32/LLD/I2Cv1",
                chRoot + "/os/hal/ports/STM32/LLD/RTCv1",
                chRoot + "/os/hal/ports/STM32/LLD/SDIOv1",
                chRoot + "/os/hal/ports/STM32/LLD/SPIv1",
                chRoot + "/os/hal/ports/STM32/LLD/TIMv1",
                chRoot + "/os/hal/ports/STM32/LLD/USARTv1",
                chRoot + "/os/hal/ports/STM32/LLD/USBv1",
                chRoot + "/os/hal/ports/STM32/LLD/xWDGv1"
                ]

        files: {
            var output = [
                chRoot + "/os/hal/src/*.c",
                chRoot + "/os/hal/include/*.h",
                // startup_stm32f1xx.mk
                chRoot + "/os/common/startup/ARMCMx/compilers/GCC/crt1.c",
                chRoot + "/os/common/startup/ARMCMx/compilers/GCC/vectors.c",
                chRoot + "/os/common/startup/ARMCMx/compilers/GCC/crt0_v7m.S",
                chRoot + "/os/common/startup/ARMCMx/compilers/GCC/*.h",
                chRoot + "/os/common/startup/ARMCMx/devices/STM32F1xx/*.h",
                chRoot + "/os/common/ext/ARM/CMSIS/Core/Include/*.h",
                chRoot + "/os/common/ext/ST/" + coreName + "/*.h",
                // hal.mk
                chRoot + "/os/hal/include/*.h",
                chRoot + "/os/hal/src/*.c",
                // rt.mk
                chRoot + "/os/rt/src/*.c",
                chRoot + "/os/common/oslib/src/*.c",
                chRoot + "/os/rt/include/*.h",
                chRoot + "/os/common/oslib/include/*.h",
                // port_v7m.mk
                chRoot + "/os/common/ports/ARMCMx/chcore.c",
                chRoot + "/os/common/ports/ARMCMx/chcore_v7m.c",
                chRoot + "/os/common/ports/ARMCMx/compilers/GCC/chcoreasm_v7m.S",
                chRoot + "/os/common/ports/ARMCMx/*.h",
                chRoot + "/os/common/ports/ARMCMx/compilers/GCC/*.h",
                // platform.mk
                chRoot + "/os/hal/ports/common/ARMCMx/*.h",
                chRoot + "/os/hal/ports/STM32/" + coreName + "/*.h",
                chRoot + "/os/hal/ports/common/ARMCMx/nvic.c",
                chRoot + "/os/hal/ports/STM32/" + coreName + "/stm32_isr.c",
                chRoot + "/os/hal/ports/STM32/" + coreName + "/hal_lld.c",
                chRoot + "/os/hal/ports/STM32/" + coreName + "/hal_adc_lld.c",
                  ]

            var index;
            for ( index = 0; index < commonSearch.length; index++ )
            {
                output.push( commonSearch[index] + "/*.c" );
                output.push( commonSearch[index] + "/*.h" );
            }

            return output;
        }

        cpp.cFlags: ["-mcpu=cortex-m3", "-mthumb",
                     "-fno-builtin", "-std=c99", "-pipe", "-Wall", "-O2",
                     "-ggdb", "-fomit-frame-pointer", "-falign-functions=16",
                     "-Wall", "-Wextra", "-Wundef", "-Wstrict-prototypes",
                     "-mno-thumb-interwork"
                    ]

        cpp.linkerFlags: ["--specs=nosys.specs",
                          "-mcpu=cortex-m3",
                          "-T " + startupLdScript,
                          "-L " + startupLd]

        cpp.defines: ["THUMB"]

        cpp.executableSuffix: ".elf"
        cpp.positionIndependentCode: false

        cpp.includePaths: {

            var includes = [chRoot + "/os/license"]

            var index;
            for ( index = 0; index < files.length; index++ )
            {
                if ( files[index].endsWith('.h') )
                {
                    var path = FileInfo.path( files[index] );
                    includes.push( path );
                }
            }

            return base.concat(includes)
        }

        Group {     // Properties for the produced executable
            fileTagsFilter: "application"
            qbs.install: true
        }
    }
}
