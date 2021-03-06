TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

INCLUDEPATH += /usr/local/include \
                /usr/local/include/opencv4
                /usr/local/include/opencv2

LIBS += -lpthread \
        /usr/local/lib/libopencv_calib3d.so.4.1 \
        /usr/local/lib/libopencv_core.so.4.1 \
        /usr/local/lib/libopencv_dnn.so.4.1 \
        /usr/local/lib/libopencv_features2d.so.4.1 \
        /usr/local/lib/libopencv_flann.so.4.1 \
        /usr/local/lib/libopencv_highgui.so.4.1 \
        /usr/local/lib/libopencv_imgcodecs.so.4.1 \
        /usr/local/lib/libopencv_imgproc.so.4.1 \
        /usr/local/lib/libopencv_ml.so.4.1 \
        /usr/local/lib/libopencv_objdetect.so.4.1 \
        /usr/local/lib/libopencv_photo.so.4.1 \
        /usr/local/lib/libopencv_shape.so.4.1 \
        /usr/local/lib/libopencv_stitching.so.4.1 \
        /usr/local/lib/libopencv_superres.so.4.1 \
        /usr/local/lib/libopencv_video.so.4.1 \
        /usr/local/lib/libopencv_videoio.so.4.1 \
        /lib/libMVSDK.so \
        /usr/local/lib/libopencv_imgcodecs.so.4.1 \


SOURCES += main.cpp \
    about_thread2.cpp

HEADERS += \
    about_thread2.h
