# Execute Validation Project

Validation project uses CMSIS-Toolbox to build and run test configurations.

The solution file `Validation.csolution.yml` defines all target types and build types. It is the input file for build and references the application layer and the board layer for each target device.

A test configuration can be built for any combination of RTOS, DEVICE and COMPILER. Possible values are listed below:

| RTOS     |  Device  | Compiler |
|----------|----------|----------|
| FreeRTOS |  CM0plus | AC6      |
| RTX5     |  CM3     | GCC      |
|          |  CM4     | CLANG    |
|          |  CM7     |          |
|          |  CM23    |          |
|          |  CM33    |          |
|          |  CM55    |          |
|          |  CM85    |          |

## Prerequisites

- [CMSIS-Toolbox 2.13.0 or later](https://github.com/Open-CMSIS-Pack/cmsis-toolbox/releases)
- [Arm Virtual Hardware FVP 11.31.28 or later](https://developer.arm.com/Tools%20and%20Software/Fixed%20Virtual%20Platforms)
- [Arm Compiler 6.24.0 or later](https://developer.arm.com/Tools%20and%20Software/Arm%20Compiler%20for%20Embedded)
- [GCC Compiler 15.2.1 or later](https://developer.arm.com/Tools%20and%20Software/GNU%20Toolchain)
- [IAR Compiler 9.70.4 or later](https://www.iar.com/embedded-development-tools)
- [LLVM/Clang Embedded Toolchain 22.1.0 or later](https://developer.arm.com/Tools%20and%20Software/Arm%20Toolchain%20for%20Embedded)
- [CMake 3.31.5 or later](https://cmake.org/download/)
- [Ninja 1.13.2 or later](https://github.com/ninja-build/ninja/releases)

These prerequisites can be installed automatically using `vcpkg` with the configuration file `.ci/vcpkg-configuration.json`:

```Shell
 $ vcpkg activate --vcpkg-configuration=.ci/vcpkg-configuration.json
```

Required CMSIS Packs:

- ARM.CMSIS
- ARM.CMSIS-RTX
- ARM.CMSIS-FreeRTOS
- ARM.Cortex_DFP

These get installed automatically if missing. One can explicitly install the packs using `cpackget` from CMSIS-Toolbox.

## Build and Execute Project with Arm Virtual Hardware

The following command will build the project for specified RTOS/Device/Compiler and its output is an executable (either `Validation.axf`, `Validation.elf` or `Validation.out`):

```Shell
 ./Project $ cbuild Validation.csolution.yml --update-rte --context .{RTOS}+{DEVICE} --toolchain {COMPILER}
```

Executable file is then used as input parameter when calling the FVP model to run the validation:

```Shell
 ./Project $ {FVP_MODEL} -q --simlimit 100 -f ../Layer/Board/{DEVICE}/fvp_config.txt -a out/Validation/{DEVICE}/{RTOS}/Validation.{EXT}
```

Test results are output to stdout in JUnit XML format.

Please see the tables below for possible values of `{FVP_MODEL}` and `{EXT}`:

| FVP_MODEL                 | DEVICE  |
|---------------------------|---------|
| FVP_MPS2_Cortex-M0plus    | CM0plus |
| FVP_MPS2_Cortex-M3        | CM3     |
| FVP_MPS2_Cortex-M4        | CM4     |
| FVP_MPS2_Cortex-M7        | CM7     |
| FVP_MPS2_Cortex-M23       | CM23    |
| FVP_MPS2_Cortex-M33       | CM33    |
| FVP_MPS2_Cortex-M55       | CM55    |
| FVP_MPS2_Cortex-M85       | CM85    |

| EXT  | COMPILER    |
|------|-------------|
| axf  | AC6         |
| elf  | GCC / CLANG |
| out  | IAR         |

## Build and Execute Project using Keil Studio for VS Code

### Quick Start

1. Install [Keil Studio for VS Code](https://marketplace.visualstudio.com/items?itemName=Arm.keil-studio-pack) from the VS Code marketplace.
2. Open the base directory in VS Code.
3. In VS Code Explorer View right click on `.ci/vcpkg-configuration.json` and select `Activate Environment`. This will download and install
   the related tools.
4. Open the [CMSIS View](https://mdk-packs.github.io/vscode-cmsis-solution-docs/userinterface.html#2-main-area-of-the-cmsis-view) in VS Code and use the *Open Solution in Workspace* to open `Project/Validation` solution.
5. In the CMSIS view, use the [Action buttons](https://github.com/ARM-software/vscode-cmsis-csolution?tab=readme-ov-file#action-buttons) to build, load and run the example on the Arm Virtual Hardware FVP models.
