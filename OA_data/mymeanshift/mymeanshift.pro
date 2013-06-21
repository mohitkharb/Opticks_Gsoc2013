TEMPLATE = app
CONFIG += console
CONFIG -= qt

SOURCES += segm/rlist.cpp \
    segm/RAList.cpp \
    segm/msSysPrompt.cpp \
    segm/msImageProcessor.cpp \
    segm/ms.cpp \
    edge/BgImage.cpp \
    edge/BgGlobalFc.cpp \
    edge/BgEdgeList.cpp \
    edge/BgEdgeDetect.cpp \
    edge/BgEdge.cpp \
    prompt/parser.cpp \
    prompt/main.cpp \
    prompt/libppm.cpp \
    prompt/globalFnc.cpp \
    prompt/edison.cpp \


HEADERS  +=edge/BgImage.h \
    edge/BgEdgeList.h \
    edge/BgEdgeDetect.h \
    edge/BgEdge.h \
    edge/BgDefaults.h \
    prompt/parser.h \
    prompt/libppm.h \
    prompt/imfilt.h \
    prompt/flags.h \
    prompt/error.h \
    prompt/edison.h \
    prompt/defs.h \
    segm/tdef.h \
    segm/rlist.h \
    segm/RAList.h \
    segm/msSys.h \
    segm/msImageProcessor.h \
    segm/ms.h \

TARGET = meanshift
