#-*- mode: makefile; tab-width: 4; -*-
#
include(../../qmake.inc)
#
#
# PACKAGE = fwbuilder-procurve_acl-$$FWB_VERSION
#
# QMAKE_CXXFLAGS_DEBUG += -DPACKAGE="\"$$PACKAGE\""
# QMAKE_CXXFLAGS_RELEASE += -DPACKAGE="\"$$PACKAGE\""

SOURCES	 =  procurve_acl.cpp

HEADERS	 = ../../config.h

!win32 {
	QMAKE_COPY    = ../../install.sh -m 0755 -s
}

win32:CONFIG += console

INCLUDEPATH += ../common ../cisco_lib/ ../compiler_lib

win32:LIBS  += ../common/release/common.lib ../cisco_lib/release/fwbcisco.lib  ../compiler_lib/release/compilerdriver.lib
!win32:LIBS += ../common/libcommon.a ../cisco_lib/libfwbcisco.a ../compiler_lib/libcompilerdriver.a

win32:PRE_TARGETDEPS  = ../common/release/common.lib ../cisco_lib/release/fwbcisco.lib ../compiler_lib/release/compilerdriver.lib
!win32:PRE_TARGETDEPS = ../common/libcommon.a ../cisco_lib/libfwbcisco.a ../compiler_lib/libcompilerdriver.a


LIBS  += $$LIBS_FWCOMPILER

TARGET      = fwb_procurve_acl
