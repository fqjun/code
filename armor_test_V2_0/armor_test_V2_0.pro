TEMPLATE = app

CONFIG += console c++11

CONFIG -= app_bundle

CONFIG -= qt

SOURCES += rm_link.cpp \
    main.cpp \
    rm_videocapture.cpp \
    rm_armorfitted_1.cpp \
    rm_roi.cpp \
    serialport.cpp \
    kalmantest.cpp \
    buffDetect/detect_buff/buff_detect.cpp \
    buffDetect/solve_buff/solve_pnp.cpp

INCLUDEPATH += /usr/local/include \
                /usr/local/include/opencv4
                /usr/local/include/opencv2
LIBS += /usr/local/lib/libopencv_* \
        /lib/libMVSDK.so
#LIBS += /usr/local/lib/libopencv_calib3d.so.4.1 \
#        /usr/local/lib/libopencv_core.so.4.1 \
#        /usr/local/lib/libopencv_dnn.so.4.1 \
#        /usr/local/lib/libopencv_features2d.so.4.1 \
#        /usr/local/lib/libopencv_flann.so.4.1 \
#        /usr/local/lib/libopencv_highgui.so.4.1 \
#        /usr/local/lib/libopencv_imgcodecs.so.4.1 \
#        /usr/local/lib/libopencv_imgproc.so.4.1 \
#        /usr/local/lib/libopencv_ml.so.4.1 \
#        /usr/local/lib/libopencv_objdetect.so.4.1 \
#        /usr/local/lib/libopencv_photo.so.4.1 \
#        /usr/local/lib/libopencv_shape.so.4.1 \
#        /usr/local/lib/libopencv_stitching.so.4.1 \
#        /usr/local/lib/libopencv_superres.so.4.1 \
#        /usr/local/lib/libopencv_video.so.4.1 \
#        /usr/local/lib/libopencv_videoio.so.4.1 \
#        /lib/libMVSDK.so

/usr/local/lib/libopencv_imgcodecs.so.4.1

HEADERS += \
    configure.h \
    rm_link.h \
    rm_armorfitted_1.h \
    rm_videocapture.h \
    rm_roi.h \
    serialport.h \
    kalmantest.h \
    buffDetect/detect_buff/buff_detect.h \
    buffDetect/solve_buff/solve_pnp.h \
    buffDetect/base.h
