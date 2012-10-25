TEMPLATE = app
TARGET = OGapp
QT += core \
    gui \
    xml
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
    finddialog.h
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
    finddialog.cpp
FORMS += \
    ChangePasswordDiag.ui \
    qpassworddiag.ui \
    cnoteindex.ui \
    aboutdialog.ui \
    gappmainwindow.ui \
    noteselection.ui \
    finddialog.ui
RESOURCES += OGappResources.qrc \
    AboutDialog.qrc
RC_FILE = gapp.rc
