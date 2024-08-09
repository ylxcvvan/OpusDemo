QT       += core gui
QT       += multimedia
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    main.cpp \
    mainwidget.cpp \
    receiver.cpp \
    sender.cpp

HEADERS += \
    mainwidget.h \
    receiver.h \
    sender.h

FORMS += \
    mainwidget.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/opus/ -lopus
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/opus/ -lopusd
else:unix:!macx: LIBS += -L$$PWD/opus/ -lopus

INCLUDEPATH += $$PWD/opus/include
DEPENDPATH += $$PWD/opus/include

win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$PWD/opus/libopus.a
else:win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$PWD/opus/libopusd.a
else:win32:!win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$PWD/opus/opus.lib
else:win32:!win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$PWD/opus/opusd.lib
else:unix:!macx: PRE_TARGETDEPS += $$PWD/opus/libopus.a
