#include "browserlauncer.h"

BrowserLauncer::BrowserLauncer(QObject *parent) :
    QObject(parent)
{
}

void BrowserLauncer::doWork()
{
    // Only under Linux
    QString urlTxt("x-www-browser \"");
    urlTxt.append(m_urlTxt);
    urlTxt.append("\"");
    m_response = system(urlTxt.toLocal8Bit().data());
}
