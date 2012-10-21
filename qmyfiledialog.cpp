#include "qmyfiledialog.h"

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
