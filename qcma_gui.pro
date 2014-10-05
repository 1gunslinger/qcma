include(qcma_common.pri)

QT += gui widgets

TARGET = qcma

SOURCES += \
    src/gui/main.cpp \
    src/gui/mainwidget.cpp \
    src/gui/singleapplication.cpp \    
    src/gui/clientmanager.cpp \
    src/gui/filterlineedit.cpp \
    src/indicator/qtrayicon.cpp \
# forms
    src/forms/backupitem.cpp \
    src/forms/backupmanagerform.cpp \
    src/forms/configwidget.cpp \
    src/forms/confirmdialog.cpp \
    src/forms/pinform.cpp \
    src/forms/progressform.cpp

HEADERS += \
    src/gui/mainwidget.h \
    src/gui/singleapplication.h \
    src/gui/clientmanager.h \
    src/gui/filterlineedit.h \
    src/indicator/trayindicator_import.h \
    src/indicator/qtrayicon.h \
# forms
    src/forms/backupitem.h \
    src/forms/backupmanagerform.h \
    src/forms/configwidget.h \
    src/forms/confirmdialog.h \
    src/forms/pinform.h \
    src/forms/progressform.h

FORMS += \
    src/forms/configwidget.ui \
    src/forms/backupmanagerform.ui \
    src/forms/backupitem.ui \
    src/forms/confirmdialog.ui \
    src/forms/progressform.ui \
    src/forms/pinform.ui

#Linux-only config
unix:!macx {
    PKGCONFIG += libnotify

    DATADIR = $$PREFIX/share

    # config for desktop file and icon
    desktop.path = $$DATADIR/applications/$${TARGET}
    desktop.files += resources/$${TARGET}.desktop

    icon64.path = $$DATADIR/icons/hicolor/64x64/apps
    icon64.files += resources/images/$${TARGET}.png

    target.path = $$BINDIR

    INSTALLS += target desktop icon64
}

unix:!macx {
    QT += dbus
    # Create the introspection XML
    QT5_SUFFIX {
        system(qdbuscpp2xml-qt5 -M -s src/gui/mainwidget.h -o org.qcma.ClientManager.xml)
    } else {
        system(qdbuscpp2xml -M -s src/gui/mainwidget.h -o org.qcma.ClientManager.xml)
    }
    # Create the helper class
    DBUS_ADAPTORS = org.qcma.ClientManager.xml
}
