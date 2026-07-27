include(../ssh.pri)
LIBS += -LE:\qt_project\Qssh-master\lib -lQSshd
TARGET=remoteprocess
SOURCES=main.cpp remoteprocesstest.cpp argumentscollector.cpp
HEADERS=remoteprocesstest.h argumentscollector.h
