#-------------------------------------------------
#
# Project created by QtCreator 2012-02-13T15:12:32
#
#-------------------------------------------------

QT       += core gui

TARGET = Object_Attribution
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
        deal.cpp \
        paint_box.cpp \
    choosesample.cpp



HEADERS  += mainwindow.h \
            deal.h \
            paint_box.h \
    choosesample.h




FORMS    += mainwindow.ui \
    choosesample.ui

INCLUDEPATH+="C:\\gdal_libs\\include"

LIBS+= -LC:\gdal_libs -lgdal


