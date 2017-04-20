#-------------------------------------------------
#
# Project created by QtCreator 2017-04-02T15:34:29
#
#-------------------------------------------------

QT       += core gui
QT += serialport

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = cnc_controller
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


SOURCES += main.cpp\
        mainwindow.cpp \
    cfeeders.cpp \
    cnozzles.cpp \
    cpackages.cpp \
    cparts.cpp \
    cgcode.cpp \
    ccontrollers.cpp \
    cnozzle.cpp \
    cpackage.cpp \
    cfeeder.cpp \
    cpart.cpp \
    cbottomvision.cpp \
    cflyingvision.cpp \
    cutils.cpp \
    copencv_utils.cpp \
    cflyingcamera.cpp \
    cboard.cpp \
    dialog_feedercalibrate.cpp \
    my_qlabel.cpp \
    cboards.cpp \
    cutils_math.cpp \
    cpick_n_place.cpp

HEADERS  += mainwindow.h \
    cfeeders.h \
    cnozzles.h \
    cpackages.h \
    cparts.h \
    cgcode.h \
    ccontrollers.h \
    cnozzle.h \
    cpackage.h \
    cfeeder.h \
    cpart.h \
    cbottomvision.h \
    cflyingvision.h \
    cutils.h \
    copencv_utils.h \
    cflyingcamera.h \
    cboard.h \
    dialog_feedercalibrate.h \
    my_qlabel.h \
    cboards.h \
    cutils_math.h \
    cpick_n_place.h

FORMS    += mainwindow.ui \
    dialog_feedercalibrate.ui

RESOURCES += \
    resources.qrc

INCLUDEPATH += .
INCLUDEPATH += /usr/local/include
INCLUDEPATH += /usr/local/include/opencv
INCLUDEPATH += /usr/local/include/opencv2
INCLUDEPATH += /usr/local/include/opencv2/core
INCLUDEPATH += /usr/local/include/opencv2/highgui
INCLUDEPATH += /usr/local/include/opencv2/imgproc
INCLUDEPATH += /usr/local/include/opencv2/flann
INCLUDEPATH += /usr/local/include/opencv2/photo
INCLUDEPATH += /usr/local/include/opencv2/video
INCLUDEPATH += /usr/local/include/opencv2/features2d
INCLUDEPATH += /usr/local/include/opencv2/objdetect
INCLUDEPATH += /usr/local/include/opencv2/calib3d
INCLUDEPATH += /usr/local/include/opencv2/ml
INCLUDEPATH += /usr/local/include/opencv2/contrib

# Set OpenCV library paths
# LIBS += `pkg-config opencv --cflags --libs`
LIBS += -L/usr/local/lib -lopencv_shape -lopencv_stitching -lopencv_objdetect -lopencv_superres -lopencv_videostab -lopencv_calib3d -lopencv_features2d -lopencv_highgui -lopencv_videoio -lopencv_imgcodecs -lopencv_video -lopencv_photo -lopencv_ml -lopencv_imgproc -lopencv_flann -lopencv_viz -lopencv_core
