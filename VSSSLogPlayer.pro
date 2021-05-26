# Qt libs to import
QT += core    \
      gui     \
      widgets  \
      network

# Project configs
TEMPLATE = app
DESTDIR  = ../bin
TARGET   = VSSSLogPlayer
VERSION  = 1.0.0

CONFIG += c++14 console
CONFIG -= app_bundle

# Temporary dirs
OBJECTS_DIR = tmp/obj
MOC_DIR = tmp/moc
UI_DIR = tmp/moc
RCC_DIR = tmp/rc

# Project libs
LIBS *= -lprotobuf -lQt5Core -lGLU

# Compiling .proto files
system(echo "Compiling protobuf files" && cd include/proto && protoc --cpp_out=../ *.proto && cd ../..)

# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS
DEFINES += APP_NAME=\\\"$$TARGET\\\"
DEFINES += APP_VERSION=\\\"$$VERSION\\\"
DEFINES += PROJECT_PATH=\\\"$${PWD}\\\"

# Project source files
SOURCES += \
        include/command.pb.cc \
        include/common.pb.cc \
        include/packet.pb.cc \
        include/replacement.pb.cc \
        include/vssref_command.pb.cc \
        include/vssref_common.pb.cc \
        include/vssref_placement.pb.cc \
        main.cpp \
        src/player/gui/fieldview/fieldobjects/fieldobjects.cpp \
        src/player/gui/fieldview/fieldview.cpp \
        src/player/gui/fieldview/gltext/gltext.cpp \
        src/player/gui/playergui.cpp \
        src/player/player.cpp \
        src/utils/exithandler/exithandler.cpp \
        src/utils/text/text.cpp \
        src/utils/timer/timer.cpp

# Project header files
HEADERS += \
        include/command.pb.h \
        include/common.pb.h \
        include/packet.pb.h \
        include/replacement.pb.h \
        include/vssref_command.pb.h \
        include/vssref_common.pb.h \
        include/vssref_placement.pb.h \
        src/player/gui/fieldview/fieldobjects/default_3v3_field.h \
        src/player/gui/fieldview/fieldobjects/fieldobjects.h \
        src/player/gui/fieldview/fieldview.h \
        src/player/gui/fieldview/gltext/gltext.h \
        src/player/gui/playergui.h \
        src/player/player.h \
        src/utils/exithandler/exithandler.h \
        src/utils/text/text.h \
        src/utils/timer/timer.h \
        src/utils/types/messagetype/messagetype.h

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

FORMS += \
    src/player/gui/playergui.ui

RESOURCES += \
    resources/resources.qrc

