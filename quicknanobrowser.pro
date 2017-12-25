requires(qtConfig(accessibility))

TEMPLATE = app
TARGET = TinWeb
CONFIG += c++14

HEADERS += \
        utils.h \
        mainwindow.h \
        previewpage.h \
        document.h \
        xmllibrary.h \
        singleton.h

SOURCES += \
        main.cpp \
        mainwindow.cpp \
        previewpage.cpp \
        document.cpp \
        xmllibrary.cpp

OTHER_FILES += ApplicationRoot.qml \
               BrowserDialog.qml \
               BrowserWindow.qml \
               DownloadView.qml \
               FullScreenNotification.qml \
               xml/main.xml

RESOURCES += resources.qrc

FORMS += \
        mainwindow.ui

QT += qml quick webengine webenginewidgets webchannel xml core
qtHaveModule(widgets) {
    greaterThan(QT_MAJOR_VERSION, 4): QT += widgets
    # QApplication is required to get native styling with QtQuickControls
}
DEFINES += QT_DEPRECATED_WARNINGS
target.path = /Users/bic_mac/Documents/QT/quicknanobrowser
INSTALLS += target

#using to for xmlSettings
copydata.commands = $(COPY_DIR) $$PWD/xml/main.xml $$OUT_PWD
first.depends = $(first) copydata
export(first.depends)
export(copydata.commands)
QMAKE_EXTRA_TARGETS += first copydata

macx
{
    ICON = icon.icns
}
win32
{
    RC_FILE = icon.rc
}
