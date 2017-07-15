#-------------------------------------------------
#
# Project created by QtCreator 2016-04-07T09:48:14
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Stdf_V4_Reader
TEMPLATE = app


SOURCES += main.cpp\
        stdf_window.cpp \
    stdf_v4_api.cpp \
    stdf_v4_internal.cpp \
    stdf_v4_file.cpp \
    debug_api.cpp

HEADERS  += stdf_window.h \
    stdf_v4_api.h \
    stdf_v4_internal.h \
    stdf_v4_file.h \
    debug_api.h

FORMS    += mainwindow.ui

RC_FILE += stdf_v4.rc
