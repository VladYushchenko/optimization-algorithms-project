#-------------------------------------------------
#
# Project created by QtCreator 2016-12-30T16:31:54
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): {
QT += widgets
CONFIG += c++11
}
else {
QMAKE_CXXFLAGS += -std=c++0x
}

TARGET = Opt
TEMPLATE = app


SOURCES += main.cpp\
        widget.cpp \
    optimizationproblem.cpp \
    generator.cpp \
    solver.cpp \
    neighborhood.cpp

HEADERS  += widget.h \
    optimizationproblem.h \
    generator.h \
    solver.h \
    neighborhood.h

FORMS    += widget.ui
