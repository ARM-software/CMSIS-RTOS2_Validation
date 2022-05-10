#!/bin/bash

function usage {
  echo "$(basename $0) [-h|--help] [-f|--format <format>] [tag-prefix]"
  echo ""
  echo "Arguments:"
  echo "  -h|--help               Print this usage message and exit."
  echo "  -f|--format <format>    Print changelog in given format."
  echo "               text       Release notes in plain text."  
  echo "               pdsc       Release notes for PDSC"
  echo "               dxy        Release notes for Doxygen"
  echo "  tag-prefix              Prefix to filter tags."
  echo ""
}

function git_describe()
{
  local gitversion=$(git describe --tags --match "$1*" --abbrev=7 || echo "$1-dirty")
  local version=$(echo $gitversion | sed -r -e 's/-([0-9]+)-(g[0-9a-f]{7})/\1+\2/')
  if [[ $version != $1 ]] && [[ $version == $gitversion ]]; then
    version+=0
  fi
  echo "Git version: '$version'" >&2
  echo $version
}

function print_text_head {
  true
}

function print_text {
  if [ -z "$2" ]; then
    echo "$1"
  else
    echo "$1 ($2)"
  fi
  
  echo -e "$3"
}

function print_text_tail {
  true
}

function print_pdsc_head {
  echo "<releases>"
}

function print_pdsc {
  if [ -z "$2" ]; then
    echo "  <release version=\"$1\">"
  else
    echo "  <release version=\"$1\" date=\"$2\">"
  fi
  echo -e "$3" | sed "s/^/    /"
  echo "  </release>"
}

function print_pdsc_tail {
  echo "</releases>"
}

function print_dxy_head {
  echo "Version    | Description"
  echo ":----------|:------------------------------------------"
}

function print_dxy {
  printf "%-10s | %s\n" "$1" "$3"
}

function print_dxy_tail {
  true
}

POSITIONAL=()
FORMAT="text"
while [[ $# -gt 0 ]]
do
  key="$1"

  case $key in
    '-h'|'--help')
      usage
      exit 1
    ;;
    '-f'|'--format')
      shift
      FORMAT=$1
      shift
    ;;
    *)    # unknown option
      POSITIONAL+=("$1") # save it in an array for later
      shift # past argument
    ;;
  esac
done
set -- "${POSITIONAL[@]}" # restore positional parameters

PREFIX=$1
TAGS=$(git for-each-ref --format "%(objecttype) %(refname)" --sort="-v:refname" refs/tags/${PREFIX} 2>/dev/null | grep ^tag | cut -d\  -f2)
LATEST=$(git_describe "${PREFIX}")

print_${FORMAT}_head

if [[ "${LATEST}" != "$(head -1 <<< ${TAGS[0]#refs/tags/})" ]]; then
  print_$FORMAT "${LATEST}" "" "Active development ..."
fi

for TAG in $TAGS; do
  TAG="${TAG#refs/tags/}" 
  DESC=$(git tag -l -n99 --format "%(contents)" ${TAG} 2>/dev/null)
  DATE=$(git tag -l -n99 --format "%(taggerdate:short)" ${TAG} 2>/dev/null)
  print_$FORMAT "${TAG#${PREFIX}}" "${DATE}" "${DESC}"       
done

print_${FORMAT}_tail

exit 0
