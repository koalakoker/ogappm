#include "browserlauncer.h"

#ifdef Q_OS_MSDOS
#include <windows.h>
#endif

BrowserLauncer::BrowserLauncer(QObject *parent) :
    QObject(parent)
{
}

void BrowserLauncer::doWork()
{
#ifdef Q_OS_LINUX
    // Only under Linux
    QString urlTxt("x-www-browser \"");
    urlTxt.append(m_urlTxt);
    urlTxt.append("\"");
    m_response = system(urlTxt.toLocal8Bit().data());
#endif
#ifdef Q_OS_MSDOS
    WCHAR wStr[1000];
    m_urlTxt.toWCharArray(wStr);
    WCHAR wCmd[100];
    QString sCmd("open");
    sCmd.toWCharArray(wCmd);
    ShellExecute(NULL, wCmd,wStr,NULL, NULL, SW_SHOWNORMAL);
#endif
}
