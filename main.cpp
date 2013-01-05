#include <QtGui>

#include "gappmainwindow.h"
#include "GAPP_Data.h"
#include "GSettings.h"
#include "qmyfiledialog.h"

int main(int argc, char *argv[])
{
    int retVal = 1;
    QApplication a(argc, argv);

    // Gapp Data
    GAPP_Data gap_Data;
    GSettings gap_settings("Koalakoker", "OGapp");
    gap_settings.append(new GSettingsItem(GSETTING_AUTOSAVE_CFGSTR,GSETTING_AUTOSAVE_DEFAULT));
    gap_settings.append(new GSettingsItem(GSETTING_SHOWTIPS_CFGSTR,GSETTING_SHOWTIPS_DEFAULT));
    gap_settings.append(new GSettingsItem(GSETTING_SHOWTIPSSEL_CFGSTR,GSETTING_SHOWTIPSSEL_DEFAULT));
    gap_settings.append(new GSettingsItem(GSETTING_SAVEWINSTATE_CFGSTR,GSETTING_SAVEWINSTATE_DEFAULT));
    gap_settings.append(new GSettingsItem(GSETTING_LANGUAGE_CFGSTR,QLocale::system().name()));

    QDir dir = QDir::homePath();
    if (!dir.cd("OGap"))
    {
        dir.mkdir("OGap");
        dir.cd("OGap");
    }
    QString defaultFile = dir.absolutePath();
    defaultFile.append("/gapp.ogp");

    gap_settings.append((new GSettingsItem(GSETTING_DEFNOTEFILE_CFGSTR,defaultFile)));

    gap_settings.LoadConfig();

    QTranslator translator;
    translator.load(QString("OGapp_")+gap_settings.Get(GSETTING_LANGUAGE_CFGSTR)->Value().toString(),":/OGapp/set1");
    a.installTranslator(&translator);

    QTranslator qtTranslator;
    qtTranslator.load(QString("qt_")+gap_settings.Get(GSETTING_LANGUAGE_CFGSTR)->Value().toString(),":/OGapp/set1");
    a.installTranslator(&qtTranslator);

    QString file;
    if (argc == 1)
    {
        file = QString(gap_settings.Get(GSETTING_DEFNOTEFILE_CFGSTR)->Value().toString());
    }
    else
    {
        file = QString(argv[1]);
    }
    int update;
    do
    {
        gap_Data.LoadData(file,&update);
        switch(update)
        {
        case OPEN_FILE:
            {
            QMyFileDialog* fileDiag = new QMyFileDialog(NULL,a.translate("mainStr","Open .ogp file"),"","*.ogp");
                if (fileDiag->exec())
                {
                    QStringList fileName = fileDiag->selectedFiles();
                    file = fileName[0];
                }
            }
            break;
        case NEW_FILE:
            {
                QMyFileDialog* fileDiag = new QMyFileDialog(NULL,a.translate("mainStr","Create new .ogp file"),"","*.ogp");
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
            gap_Data.notesAdd("","Pag1");
            gap_Data.notesAdd("","Pag2");
            gap_Data.notesAdd("","Pag3");
            gap_Data.notesAdd("","Pag4");
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
