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
    QApplication a(argc, argv);

    // Gapp Data
    GAPP_Data gap_Data;

    char *homedir;
#ifdef LINUX_COMPILE
    struct passwd *pw = getpwuid(getuid());
    homedir = pw->pw_dir;
    homedir = strcat (homedir,"/OGap/gapp.ogp");
    printf("%s",homedir);
#else
    char defaultHomeDir[] = "gapp.ogp";
    homedir = defaultHomeDir;
#endif

    QString file(homedir);
    int update;
    if (gap_Data.LoadData(file,&update))
    {
    	// Gapp GUI
        //GAPP_GUI w;
        GappMainWindow w;

        w.addData(&gap_Data);
		if (update == UPDATE_ON_LOAD)
		{
			w.updateGUI();
		}
		w.show();
		return a.exec();
    }
    return 1;
}
