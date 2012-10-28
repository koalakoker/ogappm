#include "gappmainwindow.h"
#include "GAPP_Data.h"
#include "GSettings.h"
#include "qmyfiledialog.h"

#include <QtGui>
#include <QApplication>
#include <QMessageBox>
#include <QStringList>

#ifdef Q_OS_LINUX
#include <unistd.h>
#include <sys/types.h>
#include <pwd.h>
#include <string.h>
#include "stdio.h"
#endif

int main(int argc, char *argv[])
{
    int retVal = 1;
    QApplication a(argc, argv);

//    QTranslator translator;
//    translator.load(QString("OGapp_")+QLocale::system().name());
//    //translator.load(QString("OGapp_it_IT.qm"));
//    a.installTranslator(&translator);

//    QTranslator qtTranslator;
//    qtTranslator.load(QString("qt_")+QLocale::system().name());
//    a.installTranslator(&qtTranslator);

    // Gapp Data
    GAPP_Data gap_Data;
    GSettings gap_settings("Koalakoker", "OGapp");
    gap_settings.append(new GSettingsItem(GSETTING_AUTOSAVE_CFGSTR,GSETTING_AUTOSAVE_DEFAULT));
    gap_settings.append(new GSettingsItem(GSETTING_SHOWTIPS_CFGSTR,GSETTING_SHOWTIPS_DEFAULT));
    gap_settings.append(new GSettingsItem(GSETTING_SAVEWINSTATE_CFGSTR,GSETTING_SAVEWINSTATE_DEFAULT));

    char *homedir;
#ifdef Q_OS_LINUX
    struct passwd *pw = getpwuid(getuid());
    homedir = pw->pw_dir;
    homedir = strcat (homedir,"/OGap/gapp.ogp");
#else
    char defaultHomeDir[] = "gapp.ogp";
    homedir = defaultHomeDir;
#endif

    gap_settings.append((new GSettingsItem(GSETTING_DEFNOTEFILE_CFGSTR,homedir)));

    gap_settings.LoadConfig();

    QString file(gap_settings.Get(GSETTING_DEFNOTEFILE_CFGSTR)->Value().toString());
    int update;
    do
    {
        gap_Data.LoadData(file,&update);
        switch(update)
        {
        case OPEN_FILE:
            {
                QMyFileDialog* fileDiag = new QMyFileDialog(NULL,"Open .ogp file","","*.ogp");
                if (fileDiag->exec())
                {
                    QStringList fileName = fileDiag->selectedFiles();
                    file = fileName[0];
                }
            }
            break;
        case NEW_FILE:
            {
                QMyFileDialog* fileDiag = new QMyFileDialog(NULL,"Create new .ogp file","","*.ogp");
                fileDiag->setAcceptMode(QFileDialog::AcceptSave);
                fileDiag->setDefaultSuffix("ogp");
                if (fileDiag->exec())
                {
                    QStringList fileName = fileDiag->selectedFiles();
                    file = fileName[0];
                    QFile fileToBeDeleted(file);
                    if (fileToBeDeleted.exists())
                    {
                        fileToBeDeleted.remove();
                    }
                }
            }
            break;
        }
    }
    while ((update == ERROR_PASSWORD_ERROR) ||
           (update == NEW_FILE) ||
           (update == OPEN_FILE));
    switch (update)
    {
    case DATA_CRYPTED_PASSWORD_MATCH:
    case FILE_EXISTING_NO_CRYPTED:
        {
            GappMainWindow w(&gap_Data,&gap_settings);
            w.updateGUI();
            w.show();
            retVal = a.exec();
        }
        break;
    case NEW_FILE_TO_BE_CREATED:
        {
            // Set default 4 page empty
            gap_Data.notesRemoveAll();
            gap_Data.notesAdd("");
            gap_Data.notesAdd("");
            gap_Data.notesAdd("");
            gap_Data.notesAdd("");
            GappMainWindow w(&gap_Data,&gap_settings);
            w.updateGUI();
            w.show();
            retVal = a.exec();
        }
        break;
    default:
        {

        }
        break;
    }

    gap_settings.SaveConfig();

    return retVal;
}
