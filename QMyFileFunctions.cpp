/*
 * QMyFileFunctions.cpp
 *
 *  Created on: Jan 5, 2010
 *      Author: gianluigi forte
 */

#include <stdlib.h>
#include "QMyFileFunctions.h"

//#include <QString>
//#include <QMessageBox>
//void AfxMessageBox(QString txt);
//
//void AfxMessageBox(QString txt)
//{
//	QMessageBox* errMsg = new QMessageBox(QMessageBox::Critical,QString("Error"),txt);
//	errMsg->exec();
//}

bool writeInt(QFile* file, int val)
{
	if (file->write((char*)&val,sizeof(val)) < sizeof(val))
		return false;
	else
		return true;
}

int readInt(QFile* file, bool* retVal)
{
	int val = 0xFF;
	if (file->read((char*)&val,sizeof(val)) < sizeof(val))
	{
		*retVal = false;
		return 0;
	}
	else
	{
		*retVal = true;
		return val;
	}
}

bool writeQString(QFile* file, QString str)
{
	int size = str.length();
//	QString out;
//	out.sprintf("Write Size:%d",size);
//	AfxMessageBox(out);
	if (!writeInt(file, size))
		return false;
	if (file->write(str.toAscii(),size) < size)
		return false;
	else
		return true;
}

QString readQString(QFile* file,bool* retVal)
{
	bool ok;
	int size = readInt(file,&ok);
	if (!ok)
	{
		*retVal = false;
		return QString("");
	}
//	QString show;
//	show.sprintf("Read Size:%d",size);
//	AfxMessageBox(show);
	char* buff = (char*)malloc(size + 1);
	if (file->read(buff,size) < size)
	{
		retVal = false;
		return QString("");
	}
	buff[size]=0;
	QString out;
    out = QString(size,' ');
    int i;
    for (i = 0; i < size; i++)
    {
    	out[i] = buff[i];
    }
//	show.sprintf("QString size:%d",out.length());
//	AfxMessageBox(show);
	free (buff);
	return out;
}

bool writeQStringList(QFile* file, QStringList strList)
{
	int i , c = strList.count();
	if (!writeInt(file,c))
		return false;
	for (i = 0; i < c; i++)
	{
		if (!writeQString(file,strList.at(i)))
			return false;
	}
	return true;
}

bool readQStringList(QFile* file, QStringList* strList)
{
	bool ok;
	int i, c = readInt(file,&ok);
	if (!ok)
	{
		return false;
	}

	for (i = 0; i < c; i++)
	{
		QString note = readQString(file,&ok);
		if (!ok)
		{
			return false;
		}
		strList->append(note);
	}
	return true;
}

