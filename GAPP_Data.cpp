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
#include "defines.h"

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
    int noteNum = 0;
    if (command[0]=='c')
    {
        m_notesCripted.clear();
        noteNum = m_notes.count();
    }
    if (command[0]=='d')
    {
        m_notes.clear();
        noteNum = m_notesCripted.count();
    }
	int i;
    for (i = 0; i < noteNum; i++)
	{
        QString app;
        if (command[0]=='c')
        {
            app=m_notes.at(i);
        }
        if (command[0]=='d')
        {
            app=m_notesCripted.at(i);
        }
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
        if (command[0]=='c')
        {
            //m_notesCripted[i]=app;
            m_notesCripted.insert(i,app);
        }
        if (command[0]=='d')
        {
            //m_notes[i]=app;
            m_notes.insert(i,app);
        }
	}
}

QString txt;

bool GAPP_Data::LoadData(QString fileName,int* retVal)
{
    bool returnVal = true;
    *retVal = ERROR_NOT_HANDLED;
	QFile file(fileName);
	if (!file.exists())
	{
		// File will be created on save
        m_fileName = fileName;
        m_notes.clear();
		m_pass = "";
        *retVal = NEW_FILE_TO_BE_CREATED;
	}
    else
    {
        if( !file.open( QIODevice::ReadOnly ) )
        {
            AfxMessageBox("Failed to open file for reading.");
            *retVal = ERROR_READING_FILE;
            returnVal = false;
        }
        else
        {
            char buff[4];
            file.read(buff,3);
            buff[3] = 0;
            QString type(buff);
            if (type != "OGP")
            {
                AfxMessageBox("Is not valid ogp file format");
                *retVal = ERROR_FILE_NOT_VALID;
                returnVal = false;
            }
            else
            {
                bool ok;
                int ver = readInt(&file,&ok);
                if (!ok)
                {
                    AfxMessageBox("Is not valid ogp file format");
                    *retVal = ERROR_FILE_NOT_VALID;
                    returnVal = false;
                }
                else
                {
                    switch (ver)
                    {
                        case 10:
                        {
                            unsigned int H,L;
                            H = readInt(&file,&ok);
                            if (!ok)
                            {
                                AfxMessageBox("Is not valid ogp file format");
                                *retVal = ERROR_FILE_NOT_VALID;
                                returnVal = false;
                            }
                            else
                            {
                                L = readInt(&file,&ok);
                                if (!ok)
                                {
                                    AfxMessageBox("Is not valid ogp file format");
                                    *retVal = ERROR_FILE_NOT_VALID;
                                    returnVal = false;
                                }
                                else
                                {
                                    if (!readQStringList(&file,&m_notes))
                                    {
                                        AfxMessageBox("Is not valid ogp file format");
                                        *retVal = ERROR_FILE_NOT_VALID;
                                        returnVal = false;
                                    }
                                    else
                                    {
                                        if ((H!=0) || (L!=0))
                                        {
                                            m_notesCripted = QStringList(m_notes);
                                            m_notes.clear();
                                            QPasswordDiag diag;
                                            int retDiag = diag.exec();
                                            if (retDiag == QDialog::Accepted)
                                            {
                                                unsigned int H1,L1;
                                                m_pass=diag.m_pass();
                                                GHashPass((const unsigned char*)m_pass.toAscii().constData(),m_pass.length(),&H1,&L1);
                                                if ((H1==H) && (L1==L))
                                                {
                                                    GCrypt_Initialize(m_pass.toAscii().constData(),m_pass.length()); // To initialize GCrypt
                                                    GCriptAppunti("d");
                                                    m_fileName = fileName;
                                                    *retVal = DATA_CRYPTED_PASSWORD_MATCH;
                                                }
                                                else
                                                {
                                                    AfxMessageBox("Password Errata!");
                                                    *retVal = ERROR_PASSWORD_ERROR;
                                                    returnVal = false;
                                                }
                                            }
                                            if (retDiag == QPASSDLG_OPEN)
                                            {
                                                *retVal = OPEN_FILE;
                                            }
                                            if (retDiag == QPASSDLG_NEWFILE)
                                            {
                                                *retVal = NEW_FILE;
                                            }
                                            if (retDiag != DATA_CRYPTED_PASSWORD_MATCH)
                                            {
                                                m_notesCripted.clear();
                                            }
                                        }
                                        else
                                        {
                                            m_fileName = fileName;
                                            m_pass = "";
                                            *retVal = FILE_EXISTING_NO_CRYPTED;
                                        }
                                    }
                                }
                            }
                        }
                        break;
                    default:
                        {
                            AfxMessageBox("File is not supported");
                            *retVal = ERROR_FILE_NOT_VALID;
                            retVal = false;
                        }
                        break;
                    }
                }
            }
        }
        file.close();
    }
    return returnVal;
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
    int ver = GAP_DATA_VER;
	writeInt(&file,ver);

	if (m_pass!="")
	{
		unsigned int H,L;
		GHashPass((const unsigned char*)m_pass.toAscii().constData(),m_pass.length(),&H,&L);
		GCrypt_Initialize(m_pass.toAscii().constData(),m_pass.length()); // To initialize GCrypt
		GCriptAppunti("c");

		writeInt(&file,H);
		writeInt(&file,L);
        writeQStringList(&file,m_notesCripted);
	}
	else
	{
		writeInt(&file,0);
		writeInt(&file,0);
        writeQStringList(&file,m_notes);
	}

	file.close();
	return true;
}
