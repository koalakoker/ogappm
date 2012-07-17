#include "browserlauncer.h"

#define LINUX_COMPILE
//#define WINDOWS_COMPILE

#ifdef WINDOWS_COMPILE
#include <windows.h>
#endif

BrowserLauncer::BrowserLauncer(QObject *parent) :
    QObject(parent)
{
}

void BrowserLauncer::doWork()
{
#ifdef LINUX_COMPILE
    // Only under Linux
    QString urlTxt("x-www-browser \"");
    urlTxt.append(m_urlTxt);
    urlTxt.append("\"");
    m_response = system(urlTxt.toLocal8Bit().data());
#endif
#ifdef WINDOWS_COMPILE
    WCHAR wStr[1000];
    m_urlTxt.toWCharArray(wStr);
    WCHAR wCmd[100];
    QString sCmd("open");
    sCmd.toWCharArray(wCmd);
    ShellExecute(NULL, wCmd,wStr,NULL, NULL, SW_SHOWNORMAL);
#endif
}
