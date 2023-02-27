#!/usr/bin/python
# -*- coding: utf-8 -*-

import logging

from datetime import datetime
from enum import Enum
from glob import glob, iglob
from pathlib import Path

from lxml.etree import XMLSyntaxError
from zipfile import ZipFile

from matrix_runner import main, matrix_axis, matrix_action, matrix_command, matrix_filter, \
    ConsoleReport, CropReport, TransformReport, JUnitReport


@matrix_axis("device", "d", "Device(s) to be considered.")
class DeviceAxis(Enum):
    CM0plus = ('CMSDK_CM0plus_VHT', 'CM0plus')
    CM3 = ('CMSDK_CM3_VHT', 'CM3')
    CM4_FP = ('CMSDK_CM4_FP_VHT', 'CM4_FP')
    CM7_DP = ('CMSDK_CM7_DP_VHT', 'CM7_DP')
    CM7_SP = ('CMSDK_CM7_SP_VHT', 'CM7_SP')
    CM23 = ('IOTKit_CM23_VHT', 'CM23')
    CM33_FP = ('IOTKit_CM33_FP_VHT', 'CM33_FP')
    CM55 = ('SSE-300-MPS3', 'CM55', 'SSE300')


@matrix_axis("rtos", "r", "RTOS(es) to be considered.")
class RtosAxis(Enum):
    RTX5 = ('RTX5')
    FREERTOS = ('FreeRTOS')


@matrix_axis("compiler", "c", "Compiler(s) to be considered.")
class CompilerAxis(Enum):
    AC6 = ('AC6')
    GCC = ('GCC')

    @property
    def image_ext(self):
        ext = {
            CompilerAxis.AC6: 'axf',
            CompilerAxis.GCC: 'elf'
        }
        return ext[self]


AVH_EXECUTABLE = {
    DeviceAxis.CM0plus: ("VHT_MPS2_Cortex-M0plus", []),
    DeviceAxis.CM3: ("VHT_MPS2_Cortex-M3", []),
    DeviceAxis.CM4_FP: ("VHT_MPS2_Cortex-M4", []),
    DeviceAxis.CM7_DP: ("VHT_MPS2_Cortex-M7", []),
    DeviceAxis.CM7_SP: ("VHT_MPS2_Cortex-M7", []),
    DeviceAxis.CM23: ("VHT_MPS2_Cortex-M23", []),
    DeviceAxis.CM33_FP: ("VHT_MPS2_Cortex-M33", []),
    DeviceAxis.CM55: ("VHT_MPS3_Corstone_SSE-300", []),
}


def config_suffix(config, timestamp=True):
    suffix = f"{config.rtos[0]}-{config.compiler[0]}-{config.device[1]}"
    if timestamp:
        suffix += f"-{datetime.now().strftime('%Y%m%d%H%M%S')}"
    return suffix


def config_name(config):
    return f"{config.rtos}_{config.compiler}+{config.device[1]}"


def project_name(config):
    return f"Validation.{config_name(config)}"


def output_dir(config):
    return f"out/Validation/{config.rtos}_{config.compiler}/{config.device[1]}"


def vht_config(config):
    return f"../Layer/Target/{config.device[1]}_VHT_{config.compiler}/vht_config.txt"


@matrix_action
def clean(config):
    """Build the selected configurations using CMSIS-Build."""
    yield cbuild_clean(f"{project_name(config)}/{project_name(config)}.cprj")


@matrix_action
def build(config, results):
    """Build the selected configurations using CMSIS-Build."""

    logging.info("Compiling Tests...")
    yield cbuild("Validation.csolution.yml", project_name(config))

    if not all(r.success for r in results):
        return

    file = f"RTOS2_Validation-{config_suffix(config)}.zip"
    logging.info(f"Archiving build output to {file}...")
    with ZipFile(file, "w") as archive:
        for content in iglob(f"{project_name(config)}/**/*", recursive=True):
            if Path(content).is_file():
                archive.write(content)


@matrix_action
def extract(config):
    """Extract the latest build archive."""
    archives = sorted(glob(f"RTOS2_Validation-{config_suffix(config, timestamp=False)}-*.zip"), reverse=True)
    yield unzip(archives[0])


@matrix_action
def run(config, results):
    """Run the selected configurations."""
    logging.info("Running RTOS2 Validation on Arm virtual hardware ...")
    yield avhrun(config)

    try:
        results[0].test_report.write(f"RTOS2_Validation-{config_suffix(config)}.junit")
    except RuntimeError as e:
        if isinstance(e.__cause__, XMLSyntaxError):
            logging.error("No valid test report found in model output!")
        else:
            logging.exception(e)


@matrix_command()
def cbuild_clean(project):
    return ["cbuild", "-c", project]


@matrix_command()
def unzip(archive):
    return ["bash", "-c", f"unzip {archive}"]


@matrix_command()
def cbuild(solution, context):
    return ["cbuild", solution, "--context", context, "--packs", "--update-rte"]


@matrix_command(test_report=ConsoleReport() |
                            CropReport('<\?xml version="1.0"\?>', '</report>') |
                            TransformReport('validation.xsl') |
                            JUnitReport(title=lambda title, result: f"{result.command.config.rtos}."
                                                                    f"{result.command.config.compiler}."
                                                                    f"{result.command.config.device}."
                                                                    f"{title}"))
def avhrun(config):
    cmdline = [AVH_EXECUTABLE[config.device][0], "-q", "--simlimit", 100, "-f", vht_config(config)]
    cmdline += AVH_EXECUTABLE[config.device][1]
    cmdline += ["-a", f"{project_name(config)}/{output_dir(config)}/{config_name(config)}.{config.compiler.image_ext}"]
    return cmdline


@matrix_filter
def filter_cm55_gcc(config):
    return config.compiler == CompilerAxis.GCC and config.device == DeviceAxis.CM55


if __name__ == "__main__":
    main()
