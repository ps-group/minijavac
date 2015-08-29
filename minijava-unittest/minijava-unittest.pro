include(../~build/minijava-build.pri)
include(../libminijava/libminijava.pri)

CONFIG += c++11 precompile_header
PRECOMPILED_HEADER = ../libminijava/stdafx.h
TARGET = "$$inferTarget(minijava-unittest)"

LIBS += -lboost_unit_test_framework

HEADERS += \
    stdafx.h \
    mockerrorstream.h \
    mockscannerstream.h

SOURCES += \
    mockerrorstream.cpp \
    test_emittingparser.cpp \
    test_scanner.cpp \
    mockscannerstream.cpp \
    main.cpp
