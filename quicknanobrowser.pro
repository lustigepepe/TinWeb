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
        singleton.h \
    helperWindowTool.h \
    printer.h

SOURCES += \
        main.cpp \
        mainwindow.cpp \
        previewpage.cpp \
        document.cpp \
        xmllibrary.cpp \
    printer.cpp

OTHER_FILES += ApplicationRoot.qml \
               BrowserDialog.qml \
               BrowserWindow.qml \
               DownloadView.qml \
               FullScreenNotification.qml

RESOURCES += resources.qrc

FORMS += \
        mainwindow.ui

QT += qml quick webengine webenginewidgets webchannel xml core printsupport
#declarative
qtHaveModule(widgets) {
    greaterThan(QT_MAJOR_VERSION, 4): QT += widgets
    # QApplication is required to get native styling with QtQuickControls
}
DEFINES += QT_DEPRECATED_WARNINGS
target.path = /Users/bic_mac/Documents/QT/quicknanobrowser
INSTALLS += target

macx:unix
{
    ICON = icon.icns
    ROOT = ~
    #using to for xmlSettings
    copydata.commands = $(COPY_DIR) $$PWD/xml/tinWeb.xml $$ROOT
    first.depends = $(first) copydata
    export(first.depends)
    export(copydata.commands)
    QMAKE_EXTRA_TARGETS += first copydata
}
win32
{
    RC_FILE = icon.rc
    WI_ROOT = C:/Users/<USER>
    #using to for xmlSettings
    copydataWIN.commandsWIN = $(COPY_DIR) $$PWD/xml/main.xml $$WI_ROOT
    firstWIN.dependsWIN = $(first) copydataWIN
    export(firstWIN.dependsWIN)
    export(copydataWIN.commandsWIN)
    QMAKE_EXTRA_TARGETS += first copydataWIN
}
#unix
#{
#    RC_FILE = icon.rc
#    WI_ROOT = C:/Users/<USER>
#    #using to for xmlSettings
#    copydataUN.commandsUN = $(COPY_DIR) $$PWD/xml/main.xml $$WI_ROOT
#    firstUN.dependsUN = $(first) copydataUN
#    export(firstUN.dependsUN)
#    export(copydataUN.commandsUN)
#    QMAKE_EXTRA_TARGETS += first copydataUN
#}


DISTFILES += \
    xml/tinWeb.xml




