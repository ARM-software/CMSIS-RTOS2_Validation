# Execute Validation Project

Validation project uses CMSIS Project Manager to generate independent projects.

Independent project can be generated for specific RTOS, DEVICE and COMPILER. Possible values for all variables are listed below:

| RTOS     |  Device  | Compiler |
|----------|----------|----------|
| FreeRTOS |  CM0plus | AC6      |
| RTX5     |  CM3     | GCC      |
|          |  CM4_FP  |          |
|          |  CM7_DP  |          |
|          |  CM7_SP  |          |
|          |  CM23    |          |
|          |  CM33_FP |          |
|          |  CM55    |          |
|          |  CM85    |          |

Input file for cbuild is Validation.csolution.yml

## Unsupported contexts
The below table lists contexts that are currently not available:
| Context  | Toolchain |  Reason                                      |
|----------|-----------|----------------------------------------------|
| +CM55    |    GCC    |  Device:Startup component has no GCC support |

## Prerequisites

- [CMSIS-Toolbox 2.0.0](https://github.com/Open-CMSIS-Pack/cmsis-toolbox/releases)
- Arm Virtual Hardware executables installed with installation path added to the system environment path
- Python or Keil MDK Professional

Packages required to build and run with Python:
- python-matrix-runner 1.0
- lxml 4.8

Python packages can be installed by executing in bash shell:
```Shell
$ pip install -r requirements.txt
```

Required CMSIS Packs:
- ARM.CMSIS
- ARM.DMA350.1.0.0
- Keil.V2M-MPS2_CMx_BSP.1.8.0
- Keil.V2M-MPS2_IOTKit_BSP.1.5.0
- ARM.V2M_MPS3_SSE_300_BSP.1.3.0
- ARM.V2M_MPS3_SSE_310_BSP.1.2.0
- ARM.CMSIS-FreeRTOS

CMSIS packs can be installed by executing:
```Shell
$ cpackget update-index
$ cpackget add -a -f cpacklist.txt
```

## Build and Execute Projects with Arm Virtual Hardware
The simplest way to execute this validation suite is to build and run configured targets using Python script:
```Shell
$ python build.py build run
```
Use --verbose command to output more information:
```Shell
$ python build.py --verbose build run
```

Above commands will build all defined projects and run each target. One might be interested in only one RTOS/Device/Compiler combination and can also build and run a project for specific combination, see below.

## Build and Execute Project for Specific Target
To build and execute validation suite for specific target use the following command:
```Shell
$ python build.py build run -r {RTOS} -d {DEVICE} -c {COMPILER}
```

> Note: Mentioned commands can be executed either in Windows command prompt or bash shell.

Python script is used to simplify build commands and Virtual Hardware Target model execution. One can call cbuild and execute the model manually, see below.

## Manually Build and Execute Project for Specific Target
Behind the scenes, build.py script calls cbuild and VHT model executable to build and execute the test suite.

The following command will build the project for specified RTOS/Device/Compiler and its output is an executable (either Validation.axf or Validation.elf):
```Shell
$ cbuild Validation.csolution.yml --update-rte --context .{RTOS}+{DEVICE} --toolchain {COMPILER}
```
Executable file is then used as input parameter when calling Virtual Hardware Target model to run the validation:
```Shell
$ {VHT_MODEL} -f ../Layer/Target/{DEVICE}_VHT/vht_config.txt -a Validation.{RTOS}+{DEVICE}_{COMPILER}/Validation.{RTOS}+{DEVICE}_OutDir/Validation.axf
```
Please see a table below for a possible {VHT_MODEL} variable value when a project for specified {DEVICE} was built:
| VHT_MODEL                 | DEVICE  |
|---------------------------|---------|
| VHT_MPS2_Cortex-M0plus    | CM0plus |
| VHT_MPS2_Cortex-M3        | CM3     |
| VHT_MPS2_Cortex-M4        | CM4_FP  |
| VHT_MPS2_Cortex-M7        | CM7_DP  |
| VHT_MPS2_Cortex-M7        | CM7_SP  |
| VHT_MPS2_Cortex-M23       | CM23    |
| VHT_MPS2_Cortex-M33       | CM33_FP |
| VHT_MPS3_Corstone_SEE-300 | CM55    |
| VHT_Corstone_SSE-310      | CM85    |

## Debug the Project using Keil MDK-Professional:
All projects can be opened and debugged using Keil MDK-Professional no matter whether the project was build using Python script or manually with cbuild.
After the project was successfully built, follow the steps below:

1. Go into folder Validation.{RTOS}+{DEVICE}_{COMPILER} and double click the generated cprj file
2. In uVision, open dialog Options for Target->Debug and configure the debugger
    - Configuration File 'vht_config.txt' for each target can be found in folder [**.\Layer\Target**](https://github.com/ARM-software/CMSIS-RTOS2_Validation/tree/main/Layer/Target)

Documentation on how to [use Keil MDK with Arm Virtual Hardware](https://arm-software.github.io/AVH/main/infrastructure/html/run_mdk_pro.html) describes how to configure the debugger.
