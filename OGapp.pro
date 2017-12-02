TEMPLATE = app
TARGET = OGapp
QT += core \
    gui \
    xml \
    widgets
HEADERS += \
    ChangePasswordDiag.h \
    QMyFileFunctions.h \
    RandY.h \
    GCrypt.h \
    qpassworddiag.h \
    GAPP_Data.h \
    QMyPlainTextEdit.h \
    QMyTabWidget.h \
    cnoteindex.h \
    browserlauncer.h \
    aboutdialog.h \
    defines.h \
    gappmainwindow.h \
    qmyfiledialog.h \
    noteselection.h \
    finddialog.h \
    preferencesdialog.h \
    GSettingsItem.h \
    GSettings.h \
    GSettingDefines.h \
    TipsDialog.h \
    Tips.h \
    QMyLineEdit.h \
    QMyListWidget.h
SOURCES += \
    ChangePasswordDiag.cpp \
    QMyFileFunctions.cpp \
    GCrypt.cpp \
    qpassworddiag.cpp \
    GAPP_Data.cpp \
    QMyPlainTextEdit.cpp \
    QMyTabWidget.cpp \
    cnoteindex.cpp \
    main.cpp \
    browserlauncer.cpp \
    aboutdialog.cpp \
    gappmainwindow.cpp \
    qmyfiledialog.cpp \
    noteselection.cpp \
    finddialog.cpp \
    preferencesdialog.cpp \
    GSettingsItem.cpp \
    GSettings.cpp \
    TipsDialog.cpp \
    Tips.cpp \
    QMyLineEdit.cpp \
    QMyListWidget.cpp
FORMS += \
    ChangePasswordDiag.ui \
    qpassworddiag.ui \
    cnoteindex.ui \
    aboutdialog.ui \
    gappmainwindow.ui \
    noteselection.ui \
    finddialog.ui \
    preferencesdialog.ui \
    TipsDialog.ui
RESOURCES += OGappResources.qrc \
    AboutDialog.qrc
RC_FILE = gapp.rc
TRANSLATIONS += OGapp_it_IT.ts \
    qt_it_IT.ts

target.path += /usr/bin
INSTALLS += target

iconfile.files += OGapp.png
iconfile.path = /usr/share/pixmaps/
INSTALLS += iconfile

launcher.files += OGapp.desktop
launcher.path = /usr/share/applications/
INSTALLS += launcher
