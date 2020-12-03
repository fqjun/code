TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle

SOURCES += main.cpp \
    solve_pnp.cpp

INCLUDEPATH +=  /usr/local/include \
                /usr/local/include/opencv4 \
                /usr/local/include/opencv4/opencv2 \

LIBS += /usr/local/lib/libopencv_* \
        /lib/libMVSDK.so

HEADERS += \
    solve_pnp.h
