#!/bin/bash
# Version: 1.0
# Date: 2022-03-08
# This bash script generates CMSIS-RTOS2 Validation Documentation:
#
# Pre-requisites:
# - bash shell (for Windows: install git for Windows)
# - doxygen 1.9.2

DOXYGEN_VERSION="1.9.2"

set -o pipefail

DIRNAME=$(dirname $(readlink -f $0))
DOXYGEN=$(which doxygen)
CHANGELOG=$(readlink -f ${DIRNAME}/../Script/gen_changelog.sh)

if [[ ! -f "${DOXYGEN}" ]]; then
    echo "Doxygen not found!" >&2
    echo "Did you miss to add it to PATH?"
    exit 1
else
    version=$("${DOXYGEN}" --version | sed -r -e 's/.*([0-9]+\.[0-9]+\.[0-9]+).*/\1/')
    echo "DOXYGEN is ${DOXYGEN} at version ${version}"
    if [[ "${version}" != "${DOXYGEN_VERSION}" ]]; then
        echo " >> Version is different from ${DOXYGEN_VERSION}!" >&2
    fi
fi

function git_describe()
{
  if git rev-parse --git-dir 2>&1 >/dev/null; then
    local gitversion=$(git describe --tags --long --match "$1*" --abbrev=7 || echo "0.0.0-dirty-0-g$(git describe --tags --match "$1*" --always --abbrev=7 2>/dev/null)")
    local version=$(echo ${gitversion#$1} | sed -r -e 's/-([a-zA-Z]+)-([0-9]+)-(g[0-9a-f]{7})/-\1\2+\3/' | sed -r -e 's/-([0-9]+)-(g[0-9a-f]{7})//')
    echo "Git version: '$version'" >&2
    echo $version
  else
    echo "No Git repository: '0.0.0-nogit'" >&2
    echo "0.0.0-nogit"
  fi
}

echo "Checking PDCS version against Git..."

if [ -z $VERSION ]; then
  VERSION_FULL=$(git_describe "v")
  VERSION=${VERSION_FULL%+*}
fi
 
echo "Generating documentation ..."
 
pushd ${DIRNAME} > /dev/null

rm -rf ${DIRNAME}/html
sed -e "s/{projectNumber}/${VERSION}/" "${DIRNAME}/CMSIS_RV2.dxy.in" \
  > "${DIRNAME}/CMSIS_RV2.dxy"

echo "${CHANGELOG} -f dxy > History.txt"
"${CHANGELOG}" -f dxy 1> History.txt 2>/dev/null

echo "${DOXYGEN} ${DIRNAME}/CMSIS_RV2.dxy"
"${DOXYGEN}" "${DIRNAME}/CMSIS_RV2.dxy"

projectName=$(grep -E "PROJECT_NAME\s+=" "${DIRNAME}/CMSIS_RV2.dxy" | sed -r -e 's/[^"]*"([^"]+)"/\1/')
datetime=$(date -u +'%a %b %e %Y %H:%M:%S')
year=$(date -u +'%Y')
if [[ "${year}" != "2022" ]]; then 
  year="2022-${year}"
fi
sed -e "s/{datetime}/${datetime}/" "${DIRNAME}/_DoxyTemplates/footer.js.in" \
  | sed -e "s/{year}/${year}/" \
  | sed -e "s/{projectName}/${projectName}/" \
  | sed -e "s/{projectNumber}/${VERSION_FULL}/" \
  > "${DIRNAME}/html/footer.js"

popd > /dev/null

exit 0
