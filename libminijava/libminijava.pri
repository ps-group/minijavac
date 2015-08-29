
CONFIG += c++11
INCLUDEPATH += "$$PWD"
LIBS += -L"$$inferTargetDir()" -Wl,-Bstatic -lminijava -Wl,-Bdynamic
PRE_TARGETDEPS += $$inferTargetDir()/libminijava.a
