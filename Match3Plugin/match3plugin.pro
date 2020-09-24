TEMPLATE = lib
TARGET = Match3
QT += qml quick
QT += gui
QT += core
CONFIG += plugin c++11
DESTDIR = Match3

TARGET = $$qtLibraryTarget($$TARGET)
uri = Match3

SOURCES += \
    match3plugin_plugin.cpp \
    match3.cpp

HEADERS += \
    match3plugin_plugin.h \
    match3.h


!equals(_PRO_FILE_PWD_, $$OUT_PWD) {
    copy_qmldir.target = $$OUT_PWD/qmldir
    copy_qmldir.depends = $$_PRO_FILE_PWD_/qmldir
    copy_qmldir.commands = $(COPY_FILE) \"$$replace(copy_qmldir.depends, /, $$QMAKE_DIR_SEP)\" \"$$replace(copy_qmldir.target, /, $$QMAKE_DIR_SEP)\"
    QMAKE_EXTRA_TARGETS += copy_qmldir
    PRE_TARGETDEPS += $$copy_qmldir.target
}

qmldir.files = qmldir

unix {
    installPath = $$[QT_INSTALL_QML]/$$replace(uri, \\., /)
    qmldir.path = $$installPath
    target.path = $$installPath
    INSTALLS += target qmldir
}

cpqmldir.files = qmldir
cpqmldir.path = $$DESTDIR
COPIES += cpqmldir

DISTFILES +=
