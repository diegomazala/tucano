#-------------------------------------------------
#
# Project created by QtCreator 2014-09-18T12:26:54
#
#-------------------------------------------------

QT       += core gui opengl

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = imgproc
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
        $$TUCANO_PATH/effects/meanfilter.hpp \
        $$TUCANO_PATH/effects/gradientfilter.hpp \
        $$TUCANO_PATH/effects/rendertexture.hpp \
        $$TUCANO_PATH/src/shader.hpp \
        $$TUCANO_PATH/src/effect.hpp \
        $$TUCANO_PATH/src/framebuffer.hpp \
        $$TUCANO_PATH/src/bufferobject.hpp \
        $$TUCANO_PATH/src/mesh.hpp \
        $$TUCANO_PATH/src/model.hpp \
        $$TUCANO_PATH/src/texturemanager.hpp \
        $$TUCANO_PATH/src/texture.hpp \
        $$TUCANO_PATH/src/trackball.hpp \
        $$TUCANO_PATH/src/camera.hpp \
        $$TUCANO_PATH/src/utils/qtplainwidget.hpp \
        $$TUCANO_PATH/src/utils/objimporter.hpp \
        $$TUCANO_PATH/src/utils/plyimporter.hpp

FORMS    += mainwindow.ui

OTHER_FILES += \
        $$TUCANO_PATH/effects/shaders/meanfilter.frag \
        $$TUCANO_PATH/effects/shaders/meanfilter.vert \
        $$TUCANO_PATH/effects/shaders/gradientfilter.frag \
        $$TUCANO_PATH/effects/shaders/gradientfilter.vert \
        $$TUCANO_PATH/effects/shaders/rendertexture.frag \
        $$TUCANO_PATH/effects/shaders/rendertexture.vert
