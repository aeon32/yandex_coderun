#!/bin/sh
TARGET_CONF=Develop
CMAKE=/opt/clion-2020.3.3/bin/cmake/linux/bin/cmake
TOOLCHAIN_FILE=x64.cmake
ARCH=x64

scriptdir=$(cd `dirname $0` && pwd)
srcdir=$(readlink -e "${scriptdir}/../")
target_conf_lowercase=$(echo $TARGET_CONF | tr '[:upper:]' '[:lower:]')
cmakelistsdir="$srcdir"
build_dir="$scriptdir/bin_${target_conf_lowercase}_$ARCH"

mkdir -p "$build_dir" && cd "$build_dir" &&
${CMAKE} --trace  -G"Unix Makefiles"  -DTARGET_CONF=$TARGET_CONF  -DCMAKE_TOOLCHAIN_FILE="$scriptdir/${TOOLCHAIN_FILE}" "$srcdir"

