TEMPLATE = app
TARGET = OGapp
QT += core \
    gui \
    xml
HEADERS += gapp.h \
    ChangePasswordDiag.h \
    QMyFileFunctions.h \
    RandY.h \
    GCrypt.h \
    qpassworddiag.h \
    GAPP_Data.h \
    QMyPlainTextEdit.h \
    QMyTabWidget.h \
    cnoteindex.h \
    gapp_gui.h \
    browserlauncer.h
SOURCES += gapp.cpp \
    ChangePasswordDiag.cpp \
    QMyFileFunctions.cpp \
    GCrypt.cpp \
    qpassworddiag.cpp \
    GAPP_Data.cpp \
    QMyPlainTextEdit.cpp \
    QMyTabWidget.cpp \
    cnoteindex.cpp \
    gapp_gui.cpp \
    main.cpp \
    browserlauncer.cpp
FORMS += gapp.ui \
    ChangePasswordDiag.ui \
    qpassworddiag.ui \
    cnoteindex.ui \
    gapp_gui.ui
RESOURCES += OGappResources.qrc
RC_FILE = gapp.rc
