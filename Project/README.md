# Execute Validation Project

Validation project uses CMSIS Project Manager to generate independent projects.

Independent project can be generated for:
- Compiler:
    - Arm Compiler 6
    - GCC
- RTOS:
    - FreeRTOS
    - RTX5
- Device:
    - Cortex-M0
    - Cortex-M3
    - Cortex-M4
    - Cortex-M7
    - Cortex-M23
    - Cortex-M33
    - Cortex-M55

Input files for csolution are Validation.csolution.yml and Validation.cproject.yml

## Prerequisites

- [CMSIS-Toolbox](https://github.com/Open-CMSIS-Pack/devtools/releases)
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
- Keil.V2M-MPS2_CMx_BSP.1.8.0
- Keil.V2M-MPS2_IOTKit_BSP.1.5.0
- ARM.V2M_MPS3_SSE_300_BSP.1.3.0
- ARM.CMSIS-FreeRTOS

CMSIS packs can be installed by executing in bash shell:
```Shell
$ cpackget update-index
$ cpackget add -a -f cpacklist.txt
```

## Convert, Build and Run Target Projects with Arm Virtual Hardware

The simplest way to execute this validation suite is to convert, build and run solution for all specified targets by executing the following commands in bash shell:

1. Convert all project targets
```Shell
$ csolution convert -s Validation.csolution.yml -p Validation.cproject.yml
```

2. Build and Run using Python script:
```Shell
$ python build.py build run
```
Use --verbose command to output more information:
```Shell
$ python build.py --verbose build run
```

Above commands will convert all specified projects, build them and run each target. One might be interested in only one RTOS/Compiler/Device combination and can also convert a project for specific combination, see below.

## Convert a Project for Specific Target
To convert validation suite for specific target use the following command in bash shell:

```Shell
$ csolution convert -s Validation.csolution.yml -c Validation.{RTOS}_{COMPILER}+{DEVICE}
```

Example: convert Validation project for RTX5, compiled with AC6 for Cortex-M55
```Shell
$ csolution convert -s Validation.csolution.yml -c Validation.RTX5_AC6+CM55
```


## Build and Execute the Project

### Build and Run using Python script:
```Shell
$ python build.py build run -r {RTOS} -c {COMPILER} -d {DEVICE}
```

### Build and Run using Keil MDK:
1. Go into folder Validation.{RTOS}_{COMPILER}.{DEVICE} and double click the generated cprj file
2. In uVision, open dialog Options for Target->Debug and configure the debugger
    - Configuration File 'vht_config.txt' for each target can be found in folder [**.\Layer\Target**](https://github.com/ARM-software/CMSIS-RTOS2_Validation/tree/main/Layer/Target)

Documentation on how to [use Keil MDK with Arm Virtual Hardware](https://arm-software.github.io/AVH/main/infrastructure/html/run_mdk_pro.html) describes how to configure the debugger.
