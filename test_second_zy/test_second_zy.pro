TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += main.cpp \
    about_thread4.cpp

INCLUDEPATH += /usr/local/include \
                /usr/local/include/opencv4
                /usr/local/include/opencv2
LIBS += -lpthread \
        /usr/local/lib/libopencv_* \
        /lib/libMVSDK.so
        /usr/local/lib/libopencv_imgcodecs.so.4.1

HEADERS += \
    about_thread4.h
