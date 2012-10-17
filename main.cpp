//#include "gapp_gui.h"
#include "gappmainwindow.h"
#include "GAPP_Data.h"

#include <QtGui>
#include <QDialog>
#include <QApplication>
#include <QMessageBox>
#include <QStringList>

#define LINUX_COMPILE

#ifdef LINUX_COMPILE
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

    // Gapp Data
    GAPP_Data gap_Data;

    char *homedir;
#ifdef LINUX_COMPILE
    struct passwd *pw = getpwuid(getuid());
    homedir = pw->pw_dir;
    homedir = strcat (homedir,"/OGap/gapp.ogp");
#else
    char defaultHomeDir[] = "gapp.ogp";
    homedir = defaultHomeDir;
#endif

    QString file(homedir);
    int update;
    do
    {
        gap_Data.LoadData(file,&update);
        switch(update)
        {
        case OPEN_FILE:
            {
                QFileDialog* fileDiag = new QFileDialog(NULL,"Open .ogp file","","*.ogp");
                if (fileDiag->exec())
                {
                    QStringList fileName = fileDiag->selectedFiles();
                    file = fileName[0];
                }
            }
            break;
        case NEW_FILE:
            {
                QFileDialog* fileDiag = new QFileDialog(NULL,"Create new .ogp file","","*.ogp");
                fileDiag->setAcceptMode(QFileDialog::AcceptSave);
                fileDiag->setDefaultSuffix("ogp");
                if (fileDiag->exec())
                {
                    QStringList fileName = fileDiag->selectedFiles();
                    file = fileName[0];
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
            GappMainWindow w;
            w.addData(&gap_Data);
            w.updateGUI();
            w.show();
            retVal = a.exec();
        }
        break;
    case NEW_FILE_TO_BE_CREATED:
        {
            GappMainWindow w;
            w.addData(&gap_Data);
            w.updateTitle();
            w.show();
            retVal = a.exec();
        }
        break;
    default:
        {

        }
        break;
    }
    return retVal;
}
