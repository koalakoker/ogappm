/*
 * QMyTabWidget.cpp
 *
 *  Created on: 03/gen/2010
 *      Author: koala
 */

#include <QFile>
#include <QTextStream>
#include <QMimeData>
#include <QUrl>
#include <QDropEvent>
#include <QGridLayout>
#include "QMyTabWidget.h"
#include "QMyPlainTextEdit.h"

QMyTabWidget::QMyTabWidget(QWidget *parent)
: QTabWidget(parent){
	// TODO Auto-generated constructor stub

}

QMyTabWidget::~QMyTabWidget() {
	// TODO Auto-generated destructor stub
}

void QMyTabWidget::dropEvent(QDropEvent * event)
{
	const QMimeData* mData = event->mimeData();

	QStringList fileNameList;
	fileNameList = mData->formats();

	int i;
	for (i=0;i<fileNameList.count();i++)
	{
		QByteArray bString = mData->data(fileNameList.at(i));

		// Debug
		//fileName.append(fileNameList.at(i) + "\n");

		if (fileNameList.at(i)=="text/uri-list")
		{
			QList<QUrl> urlList = mData->urls();
			foreach (QUrl url,urlList)
			{
				QString fileName = "";

				//fileName.append("\n\n" + QString(80,'*') + "\nRipped from: " +
				//				url.toLocalFile() + "\n" + QString(80,'*') + "\n\n");
				QFile file(url.toLocalFile());
				if( !file.exists() )
				{

					return;
				}
				if( !file.open( QIODevice::ReadOnly | QIODevice::Text ) )
				{
					return;
				}

				QTextStream stream( &file );

				int nextPage = this->NextNewPag();

				QWidget *newPag = new QWidget();
				newPag->setObjectName(QString::fromUtf8("newPag"));
				QGridLayout *gridLayout = new QGridLayout(newPag);
				gridLayout->setSpacing(6);
				gridLayout->setContentsMargins(11, 11, 11, 11);
				gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
				QMyPlainTextEdit *plainTextEdit = new QMyPlainTextEdit(newPag);
				plainTextEdit->setObjectName(QString::fromUtf8("plainTextEdit"));

				gridLayout->addWidget((QPlainTextEdit*)plainTextEdit, 0, 0, 1, 1);

				QString out;
				out.sprintf("Pag%d",nextPage);

				this->addTab(newPag, out);
				this->setCurrentIndex(this->count()-1); // Zero base index

				fileName.append(stream.readAll());
				plainTextEdit->setPlainText(fileName);
			}
		}
	}
}

void QMyTabWidget::dragEnterEvent ( QDragEnterEvent * event )
{
	event->accept();
}

void QMyTabWidget::dragMoveEvent ( QDragMoveEvent * event )
{
	event->setDropAction(Qt::CopyAction);
}

int QMyTabWidget::NextNewPag(void)
{
	int next = 0,i;
	for (i = 0; i < this->count(); i++)
	{
		QString title = this->tabText(i);

		title = title.remove("Pag");
		int pagNum = title.toInt();
		if (next < pagNum)
		{
			next = pagNum;
		}
	}
	return next+1;
}
