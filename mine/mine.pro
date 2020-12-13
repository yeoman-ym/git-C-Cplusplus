TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt
CONFIG += sdk_no_version_check

SOURCES += \
        main.cpp \
        menu.cpp \
        mine.cpp

HEADERS += \
    com.h \
    menu.h \
    mine.h
