#ifndef QMYFILEDIALOG_H
#define QMYFILEDIALOG_H

#include <QFileDialog>
#include <QLabel>

class QMyFileDialog : public QFileDialog
{
    Q_OBJECT
public:
    explicit QMyFileDialog(QWidget *parent = 0,
                         const QString &caption = QString(),
                         const QString &directory = QString(),
                         const QString &filter = QString());
    void UpdateFileInfo(QStringList info);
signals:
    
public slots:

private:
    QLabel m_fileInfo;
    QSize m_defaultSize;

private slots:
    void getFileInfo(QString fileName);
};

#endif // QMYFILEDIALOG_H
