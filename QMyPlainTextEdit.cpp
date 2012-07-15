/*
 * QMyPlainTextEdit.cpp
 *
 *  Created on: 31/dic/2009
 *      Author: koala
 */
#include <QFile>
#include <QTextStream>
#include <QMimeData>
#include <QUrl>
#include <QTextCursor>
#include <QTextBlock>
#include <QThread>
#include "QMyPlainTextEdit.h"
#include "browserlauncer.h"

QMyPlainTextEdit::QMyPlainTextEdit(QWidget *parent)
: QPlainTextEdit(parent) {

    m_controlKeyPressed = false;
    m_WKeyPressed = false;

}

QMyPlainTextEdit::~QMyPlainTextEdit() {
	// TODO Auto-generated destructor stub
}

void QMyPlainTextEdit::dropEvent(QDropEvent * event)
{
	const QMimeData* mData = event->mimeData();

	QStringList fileNameList;
	fileNameList = mData->formats();

	QString fileName = this->toPlainText();

	int i;
	for (i=0;i<fileNameList.count();i++)
	{
		QByteArray bString = mData->data(fileNameList.at(i));

		// Debug
//		fileName.append(fileNameList.at(i) + "\n");

		if (fileNameList.at(i)=="text/uri-list")
		{
			QList<QUrl> urlList = mData->urls();
			foreach (QUrl url,urlList)
			{
//				fileName.append("\n\n" + QString(80,'*') + "\nRipped from: " +
//								url.toLocalFile() + "\n" + QString(80,'*') + "\n\n");
				QFile file(url.toLocalFile());
				if( !file.exists() )
				{
					fileName.append("File:" + url.toLocalFile() + " do not exsist!\n");
					this->setPlainText(fileName);
					return;
				}
				if( !file.open( QIODevice::ReadOnly | QIODevice::Text ) )
				{
					fileName.append("File:" + url.toLocalFile() + " can't be opened!\n");
					this->setPlainText(fileName);
					return;
				}

				QTextStream stream( &file );
				fileName.append(stream.readAll());
			}
		}
	}

	this->setPlainText(fileName);
}

void QMyPlainTextEdit::dragEnterEvent ( QDragEnterEvent * event )
{
	event->accept();
}

void QMyPlainTextEdit::dragMoveEvent ( QDragMoveEvent * event )
{
	event->setDropAction(Qt::CopyAction);
}

void QMyPlainTextEdit::mousePressEvent (QMouseEvent *e)
{
    if (m_controlKeyPressed)
    {
        QPlainTextEdit::mousePressEvent(e);
        QTextCursor textCursor = this->textCursor();
        textCursor.select(QTextCursor::LineUnderCursor);
        this->setTextCursor(textCursor);
        QString selectedTXT = textCursor.selectedText();

        if (m_WKeyPressed)
        {
            QThread *thread = new QThread;
            BrowserLauncer *worker = new BrowserLauncer;
            worker->SetUrl(selectedTXT);
            worker->moveToThread(thread);
            thread->start();
            QMetaObject::invokeMethod(worker, "doWork", Qt::QueuedConnection);
        }
    }
    else
    {
        QPlainTextEdit::mousePressEvent(e);
    }
}

void QMyPlainTextEdit::keyPressEvent (QKeyEvent *e)
{
    if (e->key() == Qt::Key_Control)
    {
        m_controlKeyPressed = true;
    }
    if (e->key() == Qt::Key_W)
    {
        m_WKeyPressed = true;
    }
    QPlainTextEdit::keyPressEvent(e);
}

void QMyPlainTextEdit::keyReleaseEvent (QKeyEvent *e)
{
    if (e->key() == Qt::Key_Control)
    {
        m_controlKeyPressed = false;
    }
    if (e->key() == Qt::Key_W)
    {
        m_WKeyPressed = false;
    }
    QPlainTextEdit::keyReleaseEvent(e);
}
