#!/bin/bash
# Version: 1.3
# Date: 2021-04-27
# This bash script generates a CMSIS Software Pack:
#
# Pre-requisites:
# - bash shell (for Windows: install git for Windows)
# - git in path (for Windows: install git for Windows)
# - 7z in path (zip archiving utility)
#   e.g. Ubuntu: sudo apt-get install p7zip-full p7zip-rar)
# - PackChk is taken from latest install CMSIS Pack installed in $CMSIS_PACK_ROOT
# - xmllint in path (XML schema validation; available only for Linux)

############### EDIT BELOW ###############
# Extend Path environment variable locally
#

set -o pipefail

function usage {
  echo "$(basename $0) [-h|--help] [<pdsc>]"
  echo ""
  echo "Arguments:"
  echo "  -h|--help  Print this usage message and exit."
  echo "  pdsc       The pack description to generate the pack for."
  echo ""
  echo "Environment:"
  echo " 7z"
  echo " PackChk"
  if [ $(uname -s) = "Linux" ]; then
    echo " xmllint"
  fi
  echo ""
}

function pack_version()
{
  local version=$(grep -Pzo "(?s)<releases>\s+<release version=\"([^\"]+)\"" "$1" | tr -d '\0' | tail -n 1 | sed -r -e 's/.*version="([^"]+)"/\1/g')
  echo "PDSC version: '$version'" >&2
  echo $version
}

function git_describe()
{
  local gitversion=$(git describe --match $1* --abbrev=9 || echo "$1-dirty")
  local version=$(echo $gitversion | sed -r -e 's/-([0-9]+)-(g[0-9a-f]{9})/\1+\2/')
  if [[ $version != $1 ]] && [[ $version == $gitversion ]]; then
    version+=0
  fi
  echo "Git version: '$version'" >&2
  echo $version
}

function patch_pdsc()
{
  if [[ "$2" != "$3" ]]; then
    echo "Updating latest release tag with version '$3'"
    sed -r -i -e "s/<release version=\"$2\"/<release version=\"$3\"/" $1
  fi
}

POSITIONAL=()
while [[ $# -gt 0 ]]
do
  key="$1"

  case $key in
    '-h'|'--help')
      usage
      exit 1
    ;;
    *)    # unknown option
      POSITIONAL+=("$1") # save it in an array for later
      shift # past argument
    ;;
  esac
done
set -- "${POSITIONAL[@]}" # restore positional parameters

OS=$(uname -s)
case $OS in
  'Linux')
    if [ -z ${CMSIS_PACK_ROOT+x} ] ; then
      CMSIS_PACK_ROOT="/home/$USER/.arm/Packs"
    fi
    CMSIS_TOOLSDIR="$(ls -drv ${CMSIS_PACK_ROOT}/ARM/CMSIS/* | head -1)/CMSIS/Utilities/Linux64"
    ;;
  'WindowsNT'|MINGW*|CYGWIN*)
    if [ -z ${CMSIS_PACK_ROOT+x} ] ; then
      CMSIS_PACK_ROOT="$LOCALAPPDATA/Arm/Packs"
    fi
    CMSIS_PACK_ROOT="/$(echo ${CMSIS_PACK_ROOT} | sed -e 's/\\/\//g' -e 's/://g' -e 's/\"//g')"
    CMSIS_TOOLSDIR="$(ls -drv ${CMSIS_PACK_ROOT}/ARM/CMSIS/* | head -1)/CMSIS/Utilities/Win32"
    ;;
  'Darwin')
    echo "Error: CMSIS Tools not available for Mac at present."
    exit 1
    ;;
  *)
    echo "Error: unrecognized OS $OS"
    exit 1
    ;;
esac

PATH_TO_ADD="$CMSIS_TOOLSDIR"

[[ ":$PATH:" != *":${PATH_TO_ADD}:"* ]] && PATH="${PATH}:${PATH_TO_ADD}"
echo $PATH_TO_ADD appended to PATH
echo " "

# Pack warehouse directory - destination
PACK_WAREHOUSE=./output

# Temporary pack build directory
PACK_BUILD=./build

# Specify directory names to be added to pack base directory
PACK_DIRS="
  license_terms
  CMSIS/Documentation
  CMSIS/RTOS2/Include
  CMSIS/RTOS2/Source
  CMSIS/RTOS2/RTX/Config
  CMSIS/RTOS2/RTX/Include
  CMSIS/RTOS2/RTX/Template
  FuSaCLibs/include
  FuSaCLibs/template
  ArmCompiler/Config
  ArmCompiler/Include
  ArmCompiler/Source
"

# Specify file names to be added to pack base directory
PACK_BASE_FILES="
  SafetyManual.html
  CMSIS/Core/Include/cmsis_compiler.h
  CMSIS/Core/Include/cmsis_armclang.h
  CMSIS/Core/Include/cmsis_armclang_ltm.h
  CMSIS/Core/Include/mpu_armv7.h
  CMSIS/Core/Include/cmsis_version.h
  CMSIS/RTOS2/RTX/RTX5.scvd
  CMSIS/RTOS2/RTX/Source/rtx_*.c
  CMSIS/RTOS2/RTX/Source/rtx_lib.h
  CMSIS/RTOS2/RTX/Source/rtx_core_c.h
  CMSIS/RTOS2/RTX/Source/rtx_core_cm.h
  ArmCompiler/EventRecorder.scvd
"

# Specify file names to be deleted from pack build directory
PACK_DELETE_FILES=""

# Specify patches to be applied
PACK_PATCH_FILES=""

############ DO NOT EDIT BELOW ###########
echo Starting CMSIS-Pack Generation: `date`
# Zip utility check
ZIP=7z
type -a "${ZIP}"
errorlevel=$?
if [ $errorlevel -gt 0 ]
  then
  echo "Error: No 7zip Utility found"
  echo "Action: Add 7zip to your path"
  echo " "
  exit 1
fi

# Pack checking utility check
PACKCHK=PackChk
type -a ${PACKCHK}
errorlevel=$?
if [ $errorlevel != 0 ]; then
  echo "Error: No PackChk Utility found"
  echo "Action: Add PackChk to your path"
  echo "Hint: Included in CMSIS Pack:"
  echo "$CMSIS_PACK_ROOT/ARM/CMSIS/<version>/CMSIS/Utilities/<os>/"
  echo " "
  exit 1
fi
echo " "

# Locate Package Description file
# check whether there is more than one pdsc file
NUM_PDSCS=$(ls -1 *.pdsc | wc -l)
PACK_DESCRIPTION_FILE=$(ls *.pdsc)
if [[ -n $1 && -f $1 ]]; then
  PACK_DESCRIPTION_FILE=$1
elif [ ${NUM_PDSCS} -lt 1 ]; then
  echo "Error: No *.pdsc file found in current directory"
  echo " "
  exit 1
elif [ ${NUM_PDSCS} -gt 1 ]; then
  echo "Error: Only one PDSC file allowed in directory structure:"
  echo "Found:"
  echo "$PACK_DESCRIPTION_FILE"
  echo "Action: Provide PDSC file explicitly!"
  echo " "
  usage
  exit 1
fi

SAVEIFS=$IFS
IFS=.
set ${PACK_DESCRIPTION_FILE}
# Pack Vendor
PACK_VENDOR=$1
# Pack Name
PACK_NAME=$2
echo "Generating Pack: for $PACK_VENDOR.$PACK_NAME"
echo " "
IFS=$SAVEIFS

case $2 in
  'FuSa_RTS_CM0')
    PACK_DIRS+="
      Examples/FuSa_Blinky_CM0_noMPU
      Examples/FuSa_Blinky_CM0plus_MPU
    "
    PACK_BASE_FILES+="
      CMSIS/Core/Include/core_cm0.h
      CMSIS/Core/Include/core_cm0plus.h
      CMSIS/RTOS2/RTX/Source/GCC/irq_armv6m.S
      FuSaCLibs/lib/fusa_clib_armv6m_softfp_ropi.l
      FuSaCLibs/lib/fusa_clib_armv6m_softfp_ropi.b
      FuSaCLibs/lib/fusa_clib_armv6m_softfp_ropi_rwpi.l
      FuSaCLibs/lib/fusa_clib_armv6m_softfp_ropi_rwpi.b
    "
  ;;
  'FuSa_RTS_CM3_Evaluation')
    PACK_DIRS+="
      Examples/FuSa_Blinky_CM3_noMPU_Eval
    "
    PACK_BASE_FILES+="
      CMSIS/Core/Include/core_cm3.h
      CMSIS/RTOS2/RTX/Source/GCC/irq_armv7m.S
    "
    PACK_DELETE_FILES+="
      FuSaCLibs/include/*
    "
    PACK_PATCH_FILES+="FuSaCLibs/eval/cm3_eval.patch"
  ;;
  'FuSa_RTS_CM3')
    PACK_DIRS+="
      Examples/FuSa_Blinky_CM3_noMPU
      Examples/FuSa_Blinky_CM3_MPU
    "
    PACK_BASE_FILES+="
      CMSIS/Core/Include/core_cm3.h
      CMSIS/RTOS2/RTX/Source/GCC/irq_armv7m.S
      FuSaCLibs/lib/fusa_clib_armv7m_softfp_ropi.l
      FuSaCLibs/lib/fusa_clib_armv7m_softfp_ropi.b
      FuSaCLibs/lib/fusa_clib_armv7m_softfp_ropi_rwpi.l
      FuSaCLibs/lib/fusa_clib_armv7m_softfp_ropi_rwpi.b
    "
  ;;
  'FuSa_RTS_CM4_Evaluation')
    PACK_DIRS+="
      Examples/FuSa_Blinky_CM4_noMPU_Eval
    "
    PACK_BASE_FILES+="
      CMSIS/Core/Include/core_cm4.h
      CMSIS/RTOS2/RTX/Source/GCC/irq_armv7m.S
    "
    PACK_DELETE_FILES+="
      FuSaCLibs/include/*
    "
    PACK_PATCH_FILES+="FuSaCLibs/eval/cm4_eval.patch"
  ;;
  'FuSa_RTS_CM4')
    PACK_DIRS+="
      Examples/FuSa_Blinky_CM4_noMPU
      Examples/FuSa_Blinky_CM4_MPU
    "
    PACK_BASE_FILES+="
      CMSIS/Core/Include/core_cm4.h
      CMSIS/RTOS2/RTX/Source/GCC/irq_armv7m.S
      FuSaCLibs/lib/fusa_clib_armv7m_softfp_ropi.l
      FuSaCLibs/lib/fusa_clib_armv7m_softfp_ropi.b
      FuSaCLibs/lib/fusa_clib_armv7m_softfp_ropi_rwpi.l
      FuSaCLibs/lib/fusa_clib_armv7m_softfp_ropi_rwpi.b
      FuSaCLibs/lib/fusa_clib_armv7m_sphardfp_ropi.l
      FuSaCLibs/lib/fusa_clib_armv7m_sphardfp_ropi.b
      FuSaCLibs/lib/fusa_clib_armv7m_sphardfp_ropi_rwpi.l
      FuSaCLibs/lib/fusa_clib_armv7m_sphardfp_ropi_rwpi.b
    "
  ;;
  'FuSa_RTS_CM7')
    PACK_DIRS+="
      Examples/FuSa_Blinky_CM7_noMPU
      Examples/FuSa_Blinky_CM7_MPU
    "
    PACK_BASE_FILES+="
      CMSIS/Core/Include/core_cm7.h
      CMSIS/Core/Include/cachel1_armv7.h
      CMSIS/RTOS2/RTX/Source/GCC/irq_armv7m.S
      FuSaCLibs/lib/fusa_clib_armv7m_softfp_ropi.l
      FuSaCLibs/lib/fusa_clib_armv7m_softfp_ropi.b
      FuSaCLibs/lib/fusa_clib_armv7m_softfp_ropi_rwpi.l
      FuSaCLibs/lib/fusa_clib_armv7m_softfp_ropi_rwpi.b
      FuSaCLibs/lib/fusa_clib_armv7m_sphardfp_ropi.l
      FuSaCLibs/lib/fusa_clib_armv7m_sphardfp_ropi.b
      FuSaCLibs/lib/fusa_clib_armv7m_sphardfp_ropi_rwpi.l
      FuSaCLibs/lib/fusa_clib_armv7m_sphardfp_ropi_rwpi.b
      FuSaCLibs/lib/fusa_clib_armv7m_hardfp_ropi.l
      FuSaCLibs/lib/fusa_clib_armv7m_hardfp_ropi.b
      FuSaCLibs/lib/fusa_clib_armv7m_hardfp_ropi_rwpi.l
      FuSaCLibs/lib/fusa_clib_armv7m_hardfp_ropi_rwpi.b
    "
  ;;
esac

#if $PACK_BUILD directory does not exist, create it.
if [ ! -d "$PACK_BUILD" ]; then
  mkdir -p "$PACK_BUILD"
fi

# Copy files into build base directory: $PACK_BUILD
# pdsc file is mandatory in base directory:
cp -f "./${PACK_VENDOR}.${PACK_NAME}.pdsc" "${PACK_BUILD}"

# Add directories
echo Adding directories to pack:
echo "${PACK_DIRS}"
echo " "
for d in ${PACK_DIRS}; do
  cp -r --parents "$d" "${PACK_BUILD}"
done

# Add files
echo Adding files to pack:
echo "${PACK_BASE_FILES}"
echo " "
if [ ! -x ${PACK_BASE_FILES+x} ]; then
  for f in ${PACK_BASE_FILES}; do
    cp -f --parents "$f" $PACK_BUILD/
  done
fi

# Delete files
echo Deleting files from pack:
echo "${PACK_DELETE_FILES}"
echo " "
if [ ! -x ${PACK_DELETE_FILES+x} ]; then
  for f in ${PACK_DELETE_FILES}; do
    find $PACK_BUILD/$(dirname "$f") -name $(basename "$f") -delete
  done
fi

# Apply patches
echo Applying patches to pack:
echo "${PACK_PATCH_FILES}"
echo " "
if [ ! -x ${PACK_PATCH_FILES+x} ]; then
  CWD=$(pwd)
  pushd $PACK_BUILD > /dev/null
  for f in ${PACK_PATCH_FILES}; do
    patch -p0 -t -i "${CWD}/${f}"
  done
  popd > /dev/null
fi

# Run Schema Check (for Linux only):
# sudo apt-get install libxml2-utils

if [ $(uname -s) = "Linux" ]; then
  echo "Running schema check for ${PACK_VENDOR}.${PACK_NAME}.pdsc"
  xmllint --noout --schema "$(realpath -m ${CMSIS_TOOLSDIR}/../PACK.xsd)" "${PACK_BUILD}/${PACK_VENDOR}.${PACK_NAME}.pdsc"
  errorlevel=$?
  if [ $errorlevel -ne 0 ]; then
    echo "build aborted: Schema check of $PACK_VENDOR.$PACK_NAME.pdsc against PACK.xsd failed"
    echo " "
    exit 1
  fi
else
  echo "Use MDK PackInstaller to run schema validation for $PACK_VENDOR.$PACK_NAME.pdsc"
fi

# Patch pack version
echo "Checking PDCS version against Git..."
pdsc_version=$(pack_version "${PACK_BUILD}/${PACK_VENDOR}.${PACK_NAME}.pdsc")
git_version=$(git_describe ${pdsc_version})
patch_pdsc "${PACK_BUILD}/${PACK_VENDOR}.${PACK_NAME}.pdsc" ${pdsc_version} ${git_version}

# Run Pack Check and generate PackName file with version
"${PACKCHK}" "${PACK_BUILD}/${PACK_VENDOR}.${PACK_NAME}.pdsc" \
  -i "${CMSIS_PACK_ROOT}/.Web/ARM.CMSIS.pdsc" \
  -n PackName.txt
errorlevel=$?
if [ $errorlevel -ne 0 ]; then
  echo "build aborted: pack check failed"
  echo " "
  exit 1
fi

PACKNAME=$(cat PackName.txt)
rm -rf PackName.txt

# Create checksum file
echo Creating checksum file:
pushd $PACK_BUILD > /dev/null
find . -type f -exec sha1sum -b {} + > ../${PACK_VENDOR}.${PACK_NAME}.sha1
mv ../${PACK_VENDOR}.${PACK_NAME}.sha1 .
popd > /dev/null

# Archiving
# $ZIP a $PACKNAME
echo "creating pack file $PACKNAME"
#if $PACK_WAREHOUSE directory does not exist create it
if [ ! -d "$PACK_WAREHOUSE" ]; then
  mkdir -p "$PACK_WAREHOUSE"
fi
pushd "$PACK_WAREHOUSE" > /dev/null
PACK_WAREHOUSE=$(pwd)
popd  > /dev/null
pushd "$PACK_BUILD" > /dev/null
PACK_BUILD=$(pwd)
"$ZIP" a "$PACK_WAREHOUSE/$PACKNAME" -tzip
popd  > /dev/null
errorlevel=$?
if [ $errorlevel -ne 0 ]; then
  echo "build aborted: archiving failed"
  exit 1
fi

echo "build of pack succeeded"
# Clean up
echo "cleaning up ..."

rm -rf "$PACK_BUILD"
echo " "

echo Completed CMSIS-Pack Generation: $(date)
