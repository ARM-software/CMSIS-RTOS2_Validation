#!/usr/bin/python
# -*- coding: utf-8 -*-

import logging

from datetime import datetime
from enum import Enum
from glob import glob

from lxml.etree import XMLSyntaxError
from zipfile import ZipFile

from matrix_runner import main, matrix_axis, matrix_action, matrix_command, ConsoleReport, CropReport, \
    TransformReport, JUnitReport


@matrix_axis("device", "d", "Device(s) to be considered.")
class DeviceAxis(Enum):
    SSE_300 = ('SSE-300-MPS3', 'SSE300')


@matrix_axis("rtos", "r", "RTOS(es) to be considered.")
class RtosAxis(Enum):
    RTX5 = ('RTX5')
    FREERTOS = ('FreeRTOS')


AVH_EXECUTABLE = {
    DeviceAxis.SSE_300: ("VHT_Corstone_SSE-300_Ethos-U55", []),
}


def config_suffix(config, timestamp=True):
    suffix = f"{config.rtos[0]}-{config.device[1]}"
    if timestamp:
        suffix += f"-{datetime.now().strftime('%Y%m%d%H%M%S')}"
    return suffix


def project_dir(config):
    return f"{config.rtos}/{config.device}"


@matrix_action
def clean(config):
    """Build the selected configurations using CMSIS-Build."""
    yield rmdir(f"{project_dir(config)}/Output")


@matrix_action
def build(config, results):
    """Build the selected configurations using CMSIS-Build."""

    logging.info("Compiling Tests...")
    yield cbuildsh(f"{project_dir(config)}/Validation.{config.device}.cprj")

    if not all(r.success for r in results):
        return

    file = f"RTOS2_Validation-{config_suffix(config)}.zip"
    logging.info(f"Archiving build output to {file}...")
    with ZipFile(file, "w") as archive:
        archive.write(f"{project_dir(config)}/Output/Validation.axf")
        archive.write(f"{project_dir(config)}/Output/Validation.axf.map")
        archive.write(f"{project_dir(config)}/Output/Validation.{config.device}.clog")


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
def rmdir(dir):
    return ["bash", "-c", f"rm -rf {dir}"]


@matrix_command()
def unzip(archive):
    return ["bash", "-c", f"unzip {archive}"]


@matrix_command()
def cbuildsh(project):
    return ["bash", "-c", f"cbuild.sh --quiet {project}"]


@matrix_command(test_report=ConsoleReport() |
                            CropReport('<\?xml version="1.0"\?>', '</report>') |
                            TransformReport('validation.xsl') |
                            JUnitReport(title=lambda title, result: f"{result.command.config.device}."
                                                                    f"{result.command.config.compiler}."
                                                                    f"{result.command.config.optimize}."
                                                                    f"{result.command.config.rtxcfg}."
                                                                    f"{title}"))
def avhrun(config):
    cmdline = [AVH_EXECUTABLE[config.device][0], "-q", "--cyclelimit", 1000000000, "-f", f"{project_dir(config)}/vht_config.txt"]
    cmdline += AVH_EXECUTABLE[config.device][1]
    cmdline += ["-a", f"{project_dir(config)}/Output/Validation.axf"]
    return cmdline


if __name__ == "__main__":
    main()
