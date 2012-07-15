#ifndef BROWSERLAUNCER_H
#define BROWSERLAUNCER_H

#include <QObject>
#include <QString>

class BrowserLauncer : public QObject
{
    Q_OBJECT
public:
    explicit BrowserLauncer(QObject *parent = 0);
    void SetUrl(QString url) {m_urlTxt = url;}
    
signals:
    
public slots:
    void doWork();

private:
    QString m_urlTxt;
    int m_response;
    
};

#endif // BROWSERLAUNCER_H
