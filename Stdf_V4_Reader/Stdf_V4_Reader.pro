#-------------------------------------------------
#
# Project created by QtCreator 2016-04-07T09:48:14
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Stdf_V4_Reader
TEMPLATE = app

SOURCES += \ 
    stdf_api/stdf_v4_api.cpp \
    stdf_api/stdf_v4_internal.cpp \
    stdf_file/stdf_v4_file.cpp \
    ui/stdf_window.cpp \
    debug_api/debug_api.cpp \
    main.cpp

HEADERS  += \ 
    stdf_api/stdf_v4_api.h \
    stdf_api/stdf_v4_internal.h \
    stdf_file/stdf_v4_file.h \
    ui/stdf_window.h \
    debug_api/debug_api.h \
    stdf_v4.rc

FORMS    += \
    ui/mainwindow.ui

RC_FILE += stdf_v4.rc

DISTFILES += \
    stdfv4.ico
