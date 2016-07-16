DEFINES += TARGET_QT

include(libKazmath/libKazmath.pri)

SOURCES += \
    $$PWD/Canvas.cpp \
    $$PWD/Camera.cpp \
    $$PWD/Color.cpp \
    $$PWD/Drawable.cpp \
    $$PWD/Matrix_.cpp \
    $$PWD/Program.cpp \
    $$PWD/Shape.cpp \
    $$PWD/Shaders.cpp \
    $$PWD/Vector.cpp

HEADERS += \
    $$PWD/Canvas.h \
    $$PWD/Camera.h \
    $$PWD/Color.h \
    $$PWD/Drawable.h \
    $$PWD/Enum.h \
    $$PWD/Graphics.h \
    $$PWD/Macro.h \
    $$PWD/Matrix.h \
    $$PWD/Program.h \
    $$PWD/Shape.h \
    $$PWD/Shaders.h \
    $$PWD/Vector.h
