#!/bin/make
export MAC_OS_MIN_VERSION = 10.15
export MAC_OS_CPP_VER = -std=c++17
export PLATFORM_DEFINES_EXTRA = OF_USING_STD_FS=1

OF_ROOT=${CURDIR}/third_party/openFrameworks
OF_INCLUDE =$(OF_ROOT)/libs/openFrameworksCompiled/project/makefileCommon/compile.project.mk
include $(OF_INCLUDE)
