# Execute Validation Project

Validation project uses CMSIS Project Manager to generate independent projects.

Independent project can be generated for specific RTOS, DEVICE and COMPILER. Possible values for all variables are listed below:

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

Input file for cbuild is Validation.csolution.yml

## Prerequisites

- [CMSIS-Toolbox 2.13.0 or later](https://github.com/Open-CMSIS-Pack/cmsis-toolbox/releases)
- [Arm Virtual Hardware FVP 11.31.28 or later](https://developer.arm.com/Tools%20and%20Software/Fixed%20Virtual%20Platforms)
- [Arm Compiler 6.24.0 or later](https://developer.arm.com/Tools%20and%20Software/Arm%20Compiler%20for%20Embedded)
- [GCC Compiler 15.2.1 or later](https://developer.arm.com/Tools%20and%20Software/GNU%20Toolchain)
- [IAR Compiler 9.70.4 or later](https://www.iar.com/embedded-development-tools)
- [LLVM/Clang Embedded Toolchain 22.1.0 or later](https://developer.arm.com/Tools%20and%20Software/Arm%20Toolchain%20for%20Embedded)
- [CMake 3.31.5 or later](https://cmake.org/download/)
- [Ninja 1.13.2 or later](https://github.com/ninja-build/ninja/releases)
- Python (to use `build.py` script)

These prerequisites can be installed automatically using `vcpkg`:

```Shell
 ./Project $ vcpkg activate
```

Python packages required to build and run with `build.py`:

- python-matrix-runner 1.3

These can be installed with `pip`:

```Shell
 ./Project $ pip install -r requirements.txt
```

Required CMSIS Packs:

- ARM.CMSIS
- ARM.CMSIS-RTX
- ARM.CMSIS-FreeRTOS
- ARM.Cortex_DFP

These get installed automatically if missing. One can explicitly install the packs using `cpackget` from CMSIS-Toolbox.

## Build and Execute Projects with Arm Virtual Hardware

The simplest way to execute this validation suite is to build and run configured targets using Python script:

```Shell
 ./Project $ python build.py build run
```

Use `--verbose` command to output more information:

```Shell
 ./Project $ python build.py --verbose build run
```

Above commands will build all defined projects and run each target. One might be interested in only one RTOS/Device/Compiler combination and can also build and run a project for specific combination, see below.

## Build and Execute Project for Specific Target

To build and execute validation suite for specific target use the following command:

```Shell
 ./Project $ python build.py build run -r {RTOS} -d {DEVICE} -c {COMPILER}
```

> Note: Mentioned commands can be executed either in Windows command prompt or bash shell.

Python script is used to simplify build commands and Virtual Hardware Target model execution. One can call cbuild and execute the model manually, see below.

## Manually Build and Execute Project for Specific Target

Behind the scenes, build.py script calls cbuild and VHT model executable to build and execute the test suite.

The following command will build the project for specified RTOS/Device/Compiler and its output is an executable (either Validation.axf or Validation.elf):

```Shell
 ./Project $ cbuild Validation.csolution.yml --update-rte --context .{RTOS}+{DEVICE} --toolchain {COMPILER}
 ```

Executable file is then used as input parameter when calling Virtual Hardware Target model to run the validation:

```Shell
 ./Project $ {AVH_MODEL} -f ../Layer/Board/{DEVICE}/fvp_config.txt -a out/Validation/{DEVICE}/{RTOS}/Validation.axf
```

Please see a table below for a possible {AVH_MODEL} variable value when a project for specified {DEVICE} was built:

| AVH_MODEL                 | DEVICE  |
|---------------------------|---------|
| FVP_MPS2_Cortex-M0plus    | CM0plus |
| FVP_MPS2_Cortex-M3        | CM3     |
| FVP_MPS2_Cortex-M4        | CM4     |
| FVP_MPS2_Cortex-M7        | CM7     |
| FVP_MPS2_Cortex-M23       | CM23    |
| FVP_MPS2_Cortex-M33       | CM33    |
| FVP_MPS2_Cortex-M55       | CM55    |
| FVP_MPS2_Cortex-M85       | CM85    |

## Debug the Project using Keil MDK-Professional

All projects can be opened and debugged using Keil MDK-Professional no matter whether the project was build using Python script or manually with cbuild.
After the project was successfully built, follow the steps below:

1. Go into folder Validation.{RTOS}+{DEVICE}_{COMPILER} and double click the generated cprj file
2. In uVision, open dialog Options for Target->Debug and configure the debugger
    - Configuration File 'fvp_config.txt' for each target can be found in folder [**.\Layer\Board**](https://github.com/ARM-software/CMSIS-RTOS2_Validation/tree/main/Layer/Board)

Documentation on how to [use Keil MDK with Arm Virtual Hardware](https://arm-software.github.io/AVH/main/infrastructure/html/run_mdk_pro.html) describes how to configure the debugger.
