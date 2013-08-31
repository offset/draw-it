TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += main.cpp \
    linefinder.cpp \
    linedetection.cpp

QMAKE_CXXFLAGS += -std=c++11

INCLUDEPATH += /usr/local/include/opencv2

LIBS += -L/usr/local/lib \
-lopencv_core \
-lopencv_imgproc \
-lopencv_highgui \
-lopencv_ml \
-lopencv_video \
-lopencv_features2d \
-lopencv_calib3d \
-lopencv_objdetect \
-lopencv_contrib \
-lopencv_legacy \
-lopencv_flann

HEADERS += \
    linefinder.hpp \
    linedetection.hpp
