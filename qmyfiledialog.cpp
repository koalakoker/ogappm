#include "qmyfiledialog.h"
#include "GAPP_Data.h"

#include <QDialogButtonBox>
#include <QBoxLayout>

QMyFileDialog::QMyFileDialog(QWidget *parent,
                             const QString &caption,
                             const QString &directory,
                             const QString &filter) :
    QFileDialog(parent,caption,directory,filter)
{
    QGridLayout* mainLayout = dynamic_cast<QGridLayout*>(layout());
    if (mainLayout)
    {
        QHBoxLayout *hbl = new QHBoxLayout();
        m_fileInfo.setFrameShape(QFrame::StyledPanel);
        hbl->addWidget(&m_fileInfo);
        int numRows = mainLayout->rowCount();
        mainLayout->addLayout( hbl, numRows,0,1,-1);
        m_defaultSize = size();
        connect(this,SIGNAL(currentChanged(QString)),this,SLOT(getFileInfo(QString)));
    }
}

void QMyFileDialog::UpdateFileInfo(QStringList info)
{
    int lineNum = info.count();
    if (lineNum > 9)
    {
        lineNum = 9;
    }
    int line;
    QString out;
    for (line = 0; line < lineNum; line++)
    {
        if (line != 0)
        {
            out.append("\n");
        }
        out.append(info.at(line));
    }
    m_fileInfo.setText(out);
    resize(m_defaultSize.width(),m_defaultSize.height()+(23*lineNum));
}

void QMyFileDialog::getFileInfo(QString fileName)
{
    QStringList fileInfo;
    fileInfo.append("File info");
    QString out;
    out = "Path: ";
    out.append(fileName);
    fileInfo.append(out);
    GAPP_Data dataFileChk;
    int retVal;
    dataFileChk.LoadDataOffline(fileName,&retVal,"");
    if (retVal == ERROR_READING_FILE)
    {
        fileInfo.append("Not valid or supported file");
    }
    else if (retVal == ERROR_PASSWORD_ERROR)
    {
        /* File is crypted */
        QString num;
        num.setNum(dataFileChk.notesCount());
        out = "Number of pages:";
        out.append(num);
        fileInfo.append(out);
        fileInfo.append("File is crypted");
    }
    else
    {
        /* File is not crypted */
        QString num;
        num.setNum(dataFileChk.notesCount());
        out = "Number of pages:";
        out.append(num);
        fileInfo.append(out);
        QStringList previewList;
        dataFileChk.NotePreview(&previewList);
        fileInfo.append(previewList);
    }
    UpdateFileInfo(fileInfo);
}

