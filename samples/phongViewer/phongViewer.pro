#-------------------------------------------------
#
# Project created by QtCreator 2014-09-18T12:26:54
#
#-------------------------------------------------

QT       += core gui opengl

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = phongViewer
TEMPLATE = app

TUCANO_PATH = ../..
EIGEN_PATH  =  ../../../eigen
GLEW_PATH = ../../../glew

INCLUDEPATH += $$TUCANO_PATH/src $$TUCANO_PATH/effects $$EIGEN_PATH $$GLEW_PATH/include

LIBS += -L$$GLEW_PATH/lib
LIBS += -lGLEW -lGLU

OBJECTS_DIR = ./obj
DESTDIR = $$TUCANO_PATH/bin

QMAKE_CXXFLAGS += -DTUCANODEBUG

SOURCES += main.cpp\
        mainwindow.cpp \
        glwidget.cpp

HEADERS  += mainwindow.h \        
        glwidget.hpp \
        $$TUCANO_PATH/effects/phongShader.hpp \
        $$TUCANO_PATH/src/utils/qttrackballwidget.hpp

FORMS    += mainwindow.ui

OTHER_FILES += \
        $$TUCANO_PATH/effects/shaders/phongShader.frag \
        $$TUCANO_PATH/effects/shaders/phongShader.vert
