# CMSIS-RTOS2 Validation

This repository contains a test suite that validates CMSIS-RTOS2 implementations. It uses [**Arm Virtual Hardware**](https://www.arm.com/virtual-hardware) to run a CI/CD flow to verify correct operation of the real-time operating systems (RTOS) under test on various Arm Cortex-M based processors.

**Arm Virtual Hardware** provides simulation models, software tooling, and infrastructure that can be integrated into CI/CD and MLOps development flows. The simulation models (called Arm Virtual Hardware Targets) are an implementation of a Cortex-M device sub-systems and are designed for complex software verification and testing. This allows simulation-based test automation of various software workloads, including unit tests, integration tests, and fault injection. Refer to the [Arm Virtual Hardware documentation](https://arm-software.github.io/AVH/main/overview/html/index.html) for more information.

## Repository structure

| Directory         | Contents                                                                          |
|-------------------|-----------------------------------------------------------------------------------|
| .github/workflows | Workflow YML files for running the test suite and for creating the documentation. |
| Doxygen           | Doxygen input files for creating the documentation.                               |
| Include           | Include files for test cases etc.                                                 |
| Layer             | Layers for creating the projects.                                                 |
| Project           | An example project that shows unit testing.                                       |
| Script            | Various shell scripts.                                                            |
| Source            | Test case source code.                                                            |

## Test matrix

Currently, the following tests are executed in the [CMSIS_RV2](./.github/workflows/cmsis_rv2.yml) workflow:

| Compiler |  Device             | RTOS     |
|----------|---------------------|----------|
| AC6      |  CMSDK_CM0plus_VHT  | RTX5     |
| AC6      |  CMSDK_CM0plus_VHT  | FreeRTOS |
| GCC      |  CMSDK_CM0plus_VHT  | RTX5     |
| GCC      |  CMSDK_CM0plus_VHT  | FreeRTOS |
| AC6      |  CMSDK_CM3_VHT      | RTX5     |
| AC6      |  CMSDK_CM3_VHT      | FreeRTOS |
| GCC      |  CMSDK_CM3_VHT      | RTX5     |
| GCC      |  CMSDK_CM3_VHT      | FreeRTOS |
| AC6      |  CMSDK_CM4_FP_VHT   | RTX5     |
| AC6      |  CMSDK_CM4_FP_VHT   | FreeRTOS |
| GCC      |  CMSDK_CM4_FP_VHT   | RTX5     |
| GCC      |  CMSDK_CM4_FP_VHT   | FreeRTOS |
| AC6      |  CMSDK_CM7_DP_VHT   | RTX5     |
| AC6      |  CMSDK_CM7_DP_VHT   | FreeRTOS |
| GCC      |  CMSDK_CM7_DP_VHT   | RTX5     |
| GCC      |  CMSDK_CM7_DP_VHT   | FreeRTOS |
| AC6      |  CMSDK_CM7_SP_VHT   | RTX5     |
| AC6      |  CMSDK_CM7_SP_VHT   | FreeRTOS |
| GCC      |  CMSDK_CM7_SP_VHT   | RTX5     |
| GCC      |  CMSDK_CM7_SP_VHT   | FreeRTOS |
| AC6      |  IOTKit_CM23_VHT    | RTX5     |
| AC6      |  IOTKit_CM23_VHT    | FreeRTOS |
| GCC      |  IOTKit_CM23_VHT    | RTX5     |
| GCC      |  IOTKit_CM23_VHT    | FreeRTOS |
| AC6      |  IOTKit_CM33_FP_VHT | RTX5     |
| AC6      |  IOTKit_CM33_FP_VHT | FreeRTOS |
| GCC      |  IOTKit_CM33_FP_VHT | RTX5     |
| GCC      |  IOTKit_CM33_FP_VHT | FreeRTOS |
| AC6      |  SSE-300-MPS3       | RTX5     |
| AC6      |  SSE-300-MPS3       | FreeRTOS |

## License

[![License](https://img.shields.io/badge/License-Apache_2.0-blue.svg)](https://opensource.org/licenses/Apache-2.0)
