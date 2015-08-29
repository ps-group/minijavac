include(../~build/minijava-build.pri)

TEMPLATE = lib
CONFIG += staticlib precompile_header c++11

PRECOMPILED_HEADER = stdafx.h
TARGET = "$$inferTarget(minijava)"

HEADERS += \
    stdafx.h \
    cxxgen/ccxxbodybuilder.h \
    cxxgen/ccxxexpr.h \
    lexical/basescannerstream.h \
    lexical/scanner.h \
    lexical/token.h \
    support/ierrorstream.h \
    syntax/emittingparser.h \
    syntax/parsecontext.h \
    lexical/filestream.h

SOURCES += \
    cxxgen/ccxxbodybuilder.cpp \
    cxxgen/ccxxexpr.cpp \
    lexical/filestream.cpp \
    lexical/scanner.cpp \
    syntax/emittingparser.cpp \
    syntax/parsecontext.cpp
