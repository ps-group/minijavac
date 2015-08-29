include(../~build/minijava-build.pri)

CONFIG += c++11 precompile_header
PRECOMPILED_HEADER = ../libminijava/stdafx.h
TARGET = "$$inferTarget(mjc)"
LIBS += -lboost_system -lboost_filesystem

include(../libminijava/libminijava.pri)

HEADERS += \
    compilerdriver.h \
    stdafx.h \
    stderrorstream.h

SOURCES += \
    compilerdriver.cpp \
    main.cpp \
    stderrorstream.cpp
