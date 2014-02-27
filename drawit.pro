TEMPLATE = app
CONFIG -= console
CONFIG -= app_bundle
CONFIG += qt

QMAKE_CXXFLAGS_RELEASE = -Ofast

# The name of the executable
TARGET = drawit

SOURCES += main.cpp \
    linefinder.cpp \
    entities.cpp \
    play.cpp \
    game.cpp \
    mainwindow.cpp \
    tilemap.cpp \
    settingsdialog.cpp \
    qdoubleslider.cpp \
    selectplayersprite.cpp

QMAKE_CXXFLAGS += -std=c++11

# Libraries and headers needed for the image processing

INCLUDEPATH += /usr/local/include/opencv2

LIBS += -L/usr/local/lib \
-lopencv_core \
-lopencv_imgproc \
-lopencv_highgui \
#-lopencv_ml \
#-lopencv_video \
#-lopencv_features2d \
#-lopencv_calib3d \
#-lopencv_objdetect \
#-lopencv_contrib \
#-lopencv_legacy \
#-lopencv_flann

# Libraries and headers needed for the gameplay

INCLUDEPATH += /usr/local/include/sfml

LIBS += -L/usr/local/lib \
-lsfml-system-d \
-lsfml-window-d \
-lsfml-graphics-d \
#-lsfml-audio-d \
#-lsfml-network-d \

HEADERS += \
    linefinder.hpp \
    play.hpp \
    entities.hpp \
    game.hpp \
    errcodes.hpp \
    mainwindow.hpp \
    tilemap.hpp \
    settingsdialog.hpp \
    qdoubleslider.h \
    selectplayersprite.h

# Header needed for the random number generation in tilemap.cpp

INCLUDEPATH += /home/manuel/Development/boost_1_54_0/boost

FORMS += \
    mainwindow.ui \
    settingsdialog.ui \
    selectplayersprite.ui
