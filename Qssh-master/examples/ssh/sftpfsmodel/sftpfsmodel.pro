include(../ssh.pri)
include(../../../../src/shared/modeltest/modeltest.pri)
#include(D:/Qt/Qt5.12.1/5.12.1/Src/qtremoteobjects/tests/auto/modelview)
INCLUDEPATH+="D:/Qt/Qt5.12.1/5.12.1/Src/qtremoteobjects/tests/auto/modelview"
QT += widgets
QT += network
TARGET=sftpfsmodel
SOURCES+=main.cpp window.cpp
HEADERS+=window.h
FORMS=window.ui
