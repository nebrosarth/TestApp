######################################################################
# Automatically generated by qmake (3.1) Mon Jan 29 13:52:16 2024
######################################################################

QT += widgets core gui

TEMPLATE = app
TARGET = TestApp
INCLUDEPATH += .
INCLUDEPATH += TestApp/x64/Release/uic
CONFIG += release
CONFIG += static


# You can make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# Please consult the documentation of the deprecated API in order to know
# how to port your code away from it.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

# Input
HEADERS += TestApp/Task.h \
           TestApp/TaskView.h \
           TestApp/TestApp.h \
           TestApp/Section/Section.h \
           TestApp/x64/Release/uic/ui_TaskView.h \
           TestApp/x64/Release/uic/ui_TestApp.h
FORMS += TestApp/TaskView.ui TestApp/TestApp.ui
SOURCES += TestApp/main.cpp \
           TestApp/Task.cpp \
           TestApp/TaskView.cpp \
           TestApp/TestApp.cpp \
           TestApp/Section/Section.cpp \
           TestApp/x64/Release/rcc/qrc_TestApp.cpp
RESOURCES += TestApp/TestApp.qrc
