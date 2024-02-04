QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17
CONFIG += console

linux-g++* {
    LIBS += -L/usr/lib64
    LIBS += -L/usr/local -L/usr -lboost_locale -lboost_system
}
win32 {
    INCLUDEPATH += C:\Boost\include\boost-1_81
    LIBS += "C:/Boost/lib/libboost_locale-mgw11-mt-x64-1_81.a"
    LIBS += "C:/Boost/lib/libboost_system-mgw11-mt-x64-1_81.a"
    #LIBS += -LC:\Boost\include\boost-1_81\
}



# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    main.cpp \
    ujldata.cpp \
    ujlimporter.cpp \
    ujlexporter.cpp \
    ujlparser.cpp \
    ujlpatcher.cpp \
    ujlui.cpp \

HEADERS += \
    ujldata.h \
    ujlimporter.h \
    ujlexporter.h \
    ujlparser.h \
    ujlpatcher.h \
    ujlui.h \
    stageinfo.h \
    types.h \

FORMS += \
    ujlui.ui \

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    resources.qrc
