#!/bin/bash

local ARCH_OLD
local PLATFORM_OLD
if [[ -e .config ]]; then
	source .config
	ARCH_OLD=$ARCH
	PLATFORM_OLD=$PLATFORM
fi

function build_arch {
	./config.sh ARCH=$1 PLATFORM=$2
	make clean
	make
	if [[ $? != "0" ]]; then
		echo "[!!!] PLATFORM FAILED:"
		echo "[!!!] $1/$2"
		exit
	fi
}

make -i distclean

build_arch m68k emu68k
build_arch arm versatilepb
build_arch ppc psim

if [[ ! -z $ARCH_OLD ]] || [[ ! -z $PLATFORM_OLD ]]; then
	./config.sh ARCH=$ARCH_OLD PLATFORM=$PLATFORM_OLD
else
	make distclean
fi
