/*
 * GAPP_Data.cpp
 *
 *  Created on: Jan 4, 2010
 *      Author: gianluigi forte
 */

#include "GAPP_Data.h"
#include "qpassworddiag.h"
#include "GCrypt.h"
#include "QMyFileFunctions.h"

#include <QFile>
#include <QTextStream>
#include <QMessageBox>
#include <QDebug>

GAPP_Data::GAPP_Data(QObject *parent)
: QObject(parent)
{
	// TODO Auto-generated constructor stub

}

GAPP_Data::~GAPP_Data() {
	// TODO Auto-generated destructor stub
}

void GAPP_Data::setPass(QString str)
{
	m_pass = str;
}

QString GAPP_Data::Pass(void)
{
	return m_pass;
}

int GAPP_Data::notesCount()
{
	return m_notes.count();
}

QString GAPP_Data::notesAt(int i)
{
	return m_notes.at(i);
}

void GAPP_Data::notesRemoveAll()
{
	int i, c = m_notes.count();
	for (i = 0; i < c; i++)
	{
		m_notes.removeAt(0);
	}
}

void GAPP_Data::notesAdd(QString str)
{
	m_notes.append(str);
}

void GAPP_Data::AfxMessageBox(QString txt)
{
	QMessageBox* errMsg = new QMessageBox(QMessageBox::Critical,QString("Error"),txt);
	errMsg->exec();
}

void GAPP_Data::GCriptAppunti(const char* command)
{
	int i;
	for (i=0;i<m_notes.count();i++)
	{
		QString app=m_notes.at(i);
		unsigned int n = app.length();
		unsigned char* buffer = (unsigned char*)malloc(n);
		unsigned int j;
		for (j = 0; j < n; j++)
		{
			buffer[j]=app.at(j).toAscii();
		}
		GCrypt(command,buffer,n);
		for (j = 0; j < n; j++)
		{
			app[j] = buffer[j];
		}
		m_notes[i]=app;
	}
}

QString txt;

bool GAPP_Data::LoadData(QString fileName,int* retVal)
{
	m_fileName = fileName;
	m_notes.clear();
	QFile file(fileName);
	if (!file.exists())
	{
		// File will be created on save
		m_pass = "";
		*retVal = NO_UPDATE_ON_LOAD;
		return true;
	}
	*retVal = UPDATE_ON_LOAD;
	if( !file.open( QIODevice::ReadOnly ) )
	{
		AfxMessageBox("Failed to open file for reading.");
		return false;
	}

	char buff[4];
	file.read(buff,3);
	buff[3] = 0;
	QString type(buff);
	if (type != "OGP")
	{
		AfxMessageBox("Is not valid ogp file format");
		return false;
	}

	bool ok;
	int ver = readInt(&file,&ok);
	if (!ok)
	{
		AfxMessageBox("Is not valid ogp file format");
		return false;
	}

	switch (ver)
	{
		case 10:

			unsigned int H,L;
			H = readInt(&file,&ok);
			if (!ok)
			{
				AfxMessageBox("Is not valid ogp file format");
				return false;
			}
			L = readInt(&file,&ok);
			if (!ok)
			{
				AfxMessageBox("Is not valid ogp file format");
				return false;
			}

			if (!readQStringList(&file,&m_notes))
			{
				AfxMessageBox("Is not valid ogp file format");
				return false;
			}

			if ((H!=0) || (L!=0))
			{
				QPasswordDiag diag;
				if (diag.exec() == QDialog::Accepted)
				{
					unsigned int H1,L1;
					m_pass=diag.m_pass();
					GHashPass((const unsigned char*)m_pass.toAscii().constData(),m_pass.length(),&H1,&L1);
					if ((H1==H) && (L1==L))
					{
						GCrypt_Initialize(m_pass.toAscii().constData(),m_pass.length()); // To initialize GCrypt
						GCriptAppunti("d");
					}
					else
					{
						AfxMessageBox("Password Errata!");
						return false;
					}
				}
				else
				{
					AfxMessageBox("Password Richiesta!");
					return false;
				}
			}
		break;
	}

	file.close();
	return true;
}

bool GAPP_Data::saveData(void)
{
	QFile file( m_fileName );
	if( !file.open( QIODevice::WriteOnly ) )
	{
		AfxMessageBox("Failed to open file for writing.");
		return false;
	}

	file.write("OGP");
	int ver = 10; // 1.0
	writeInt(&file,ver);

	if (m_pass!="")
	{
		unsigned int H,L;
		GHashPass((const unsigned char*)m_pass.toAscii().constData(),m_pass.length(),&H,&L);
		GCrypt_Initialize(m_pass.toAscii().constData(),m_pass.length()); // To initialize GCrypt
		GCriptAppunti("c");

		writeInt(&file,H);
		writeInt(&file,L);
	}
	else
	{
		writeInt(&file,0);
		writeInt(&file,0);
	}

	writeQStringList(&file,m_notes);

	file.close();
	return true;
}
