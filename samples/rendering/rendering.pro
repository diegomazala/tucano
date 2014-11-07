#-------------------------------------------------
#
# Project created by QtCreator 2014-09-18T12:26:54
#
#-------------------------------------------------

QT       += core gui opengl

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = rendering
TEMPLATE = app

TUCANO_PATH = ../../

INCLUDEPATH += $$TUCANO_PATH/src $$TUCANO_PATH/effects

EIGEN_PATH  =  /usr/include/eigen3
INCLUDEPATH += $$EIGEN_PATH

#GLEW_PATH = ../../../glew
#INCLUDEPATH += $$GLEW_PATH/include
#LIBS += -L$$GLEW_PATH/lib

LIBS += -lGLEW -lGLU

OBJECTS_DIR = ./obj
DESTDIR = $$TUCANO_PATH/bin

QMAKE_CXXFLAGS += -DTUCANODEBUG

SOURCES += main.cpp\
        mainwindow.cpp \
        glwidget.cpp

HEADERS  += mainwindow.h \        
        glwidget.hpp \
        $$TUCANO_PATH/effects/ssao.hpp \
        $$TUCANO_PATH/effects/phongshader.hpp \
        $$TUCANO_PATH/src/utils/qttrackballwidget.hpp

FORMS    += mainwindow.ui

OTHER_FILES += \
        $$TUCANO_PATH/effects/shaders/ssao.frag \
        $$TUCANO_PATH/effects/shaders/ssao.vert \
        $$TUCANO_PATH/effects/shaders/viewspacebuffer.frag \
        $$TUCANO_PATH/effects/shaders/viewspacebuffer.vert \
        $$TUCANO_PATH/effects/shaders/gaussianblurfilter.frag \
        $$TUCANO_PATH/effects/shaders/gaussianblurfilter.vert \
        $$TUCANO_PATH/effects/shaders/phongshader.frag \
        $$TUCANO_PATH/effects/shaders/phongshader.vert

