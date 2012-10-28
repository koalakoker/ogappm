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
    m_hasModified = false;
    m_notes.clear();
    m_notesCripted.clear();
    m_pass = "";
    m_fileName = "";
}

GAPP_Data::~GAPP_Data()
{
}

void GAPP_Data::setPass(QString str)
{
	m_pass = str;
    m_hasModified = true;
}

QString GAPP_Data::Pass(void)
{
	return m_pass;
}

int GAPP_Data::notesCount()
{
    int noteCount = m_notes.count();
    int noteCryptCount = m_notesCripted.count();
    int retVal;
    if (noteCount > noteCryptCount)
    {
        retVal = noteCount;
    }
    else
    {
        retVal = noteCryptCount;
    }
    return retVal;
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
    m_hasModified = true;
}

void GAPP_Data::notesAdd(QString str)
{
	m_notes.append(str);
    m_hasModified = true;
}

void GAPP_Data::AfxMessageBox(QString txt)
{
    QMessageBox* errMsg = new QMessageBox(QMessageBox::Critical,QString(tr("Error")),txt);
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
        m_hasModified = false;
        *retVal = NEW_FILE_TO_BE_CREATED;
	}
    else
    {
        if( !file.open( QIODevice::ReadOnly ) )
        {
            AfxMessageBox(tr("Failed to open file for reading."));
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
                AfxMessageBox(tr("Is not valid ogp file format"));
                *retVal = ERROR_FILE_NOT_VALID;
                returnVal = false;
            }
            else
            {
                bool ok;
                int ver = readInt(&file,&ok);
                if (!ok)
                {
                    AfxMessageBox(tr("Is not valid ogp file format"));
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
                                AfxMessageBox(tr("Is not valid ogp file format"));
                                *retVal = ERROR_FILE_NOT_VALID;
                                returnVal = false;
                            }
                            else
                            {
                                L = readInt(&file,&ok);
                                if (!ok)
                                {
                                    AfxMessageBox(tr("Is not valid ogp file format"));
                                    *retVal = ERROR_FILE_NOT_VALID;
                                    returnVal = false;
                                }
                                else
                                {
                                    if (!readQStringList(&file,&m_notes))
                                    {
                                        AfxMessageBox(tr("Is not valid ogp file format"));
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
                                                    m_hasModified = false;
                                                    *retVal = DATA_CRYPTED_PASSWORD_MATCH;
                                                }
                                                else
                                                {
                                                    AfxMessageBox(tr("Wrong password!"));
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
                                            m_hasModified = false;
                                            *retVal = FILE_EXISTING_NO_CRYPTED;
                                        }
                                    }
                                }
                            }
                        }
                        break;
                    default:
                        {
                            AfxMessageBox(tr("File is not supported"));
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

bool GAPP_Data::LoadDataOffline(QString fileName,int* retVal,QString userPass)
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
        m_hasModified = false;
        *retVal = NEW_FILE_TO_BE_CREATED;
    }
    else
    {
        if( !file.open( QIODevice::ReadOnly ) )
        {
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
                *retVal = ERROR_FILE_NOT_VALID;
                returnVal = false;
            }
            else
            {
                bool ok;
                int ver = readInt(&file,&ok);
                if (!ok)
                {
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
                                *retVal = ERROR_FILE_NOT_VALID;
                                returnVal = false;
                            }
                            else
                            {
                                L = readInt(&file,&ok);
                                if (!ok)
                                {
                                    *retVal = ERROR_FILE_NOT_VALID;
                                    returnVal = false;
                                }
                                else
                                {
                                    if (!readQStringList(&file,&m_notes))
                                    {
                                        *retVal = ERROR_FILE_NOT_VALID;
                                        returnVal = false;
                                    }
                                    else
                                    {
                                        if ((H!=0) || (L!=0))
                                        {
                                            m_notesCripted = QStringList(m_notes);
                                            m_notes.clear();

                                            if (userPass != "")
                                            {
                                                unsigned int H1,L1;
                                                m_pass=userPass;
                                                GHashPass((const unsigned char*)m_pass.toAscii().constData(),m_pass.length(),&H1,&L1);
                                                if ((H1==H) && (L1==L))
                                                {
                                                    GCrypt_Initialize(m_pass.toAscii().constData(),m_pass.length()); // To initialize GCrypt
                                                    GCriptAppunti("d");
                                                    m_fileName = fileName;
                                                    m_hasModified = false;
                                                    *retVal = DATA_CRYPTED_PASSWORD_MATCH;
                                                }
                                                else
                                                {
                                                    *retVal = ERROR_PASSWORD_ERROR;
                                                    returnVal = false;
                                                }
                                            }
                                            else
                                            {
                                                *retVal = ERROR_PASSWORD_ERROR;
                                                returnVal = false;
                                            }
                                        }
                                        else
                                        {
                                            m_fileName = fileName;
                                            m_pass = "";
                                            m_hasModified = false;
                                            *retVal = FILE_EXISTING_NO_CRYPTED;
                                        }
                                    }
                                }
                            }
                        }
                        break;
                    default:
                        {
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
        AfxMessageBox(tr("Failed to open file for writing."));
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
        m_hasModified = false;
	}
	else
	{
		writeInt(&file,0);
		writeInt(&file,0);
        writeQStringList(&file,m_notes);
        m_hasModified = false;
	}

	file.close();
	return true;
}

bool GAPP_Data::HasModified(void)
{
    return m_hasModified;
}

bool GAPP_Data::IsCrypted(void)
{
    bool retVal = false;
    if (m_pass != "")
    {
        retVal = true;
    }
    return retVal;
}

void GAPP_Data::NotePreview(QStringList* previewList)
{
    previewList->clear();
    int i;
    for (i = 0; i < m_notes.count(); i++)
    {
        QString out;
        out.sprintf("Pag%d - ",i+1);
        QString previewStr = m_notes.at(i);
        QStringList previewStrList = previewStr.split('\n');
        out.append(previewStrList.at(0));
        previewList->append(out);
    }
}

void GAPP_Data::NoteMatchList(bool* matchList,GAPP_Data* noteList)
{
    int i;
    for (i = 0; i <  noteList->notesCount(); i++)
    {
        QString noteToBeFound = noteList->notesAt(i);
        int j;
        matchList[i] = false;
        for (j = 0; j < this->notesCount(); j++)
        {
            if (noteToBeFound == this->notesAt(j))
            {
                matchList[i] = true;
                continue;
            }
        }
    }
}

void GAPP_Data::FindInNotes(QStringList* resultStrList, QList<int>* noteSelectedList, QList<int>* numFindRequiredList, const QString& strToBefind)
{
    resultStrList->clear();
    noteSelectedList->clear();
    numFindRequiredList->clear();
    int i;
    for (i = 0; i < m_notes.count(); i++)
    {
        QString note = m_notes.at(i);
        QStringList noteRows = note.split("\n");
        int j;
        int numFindRequired = 1;
        for (j = 0; j < noteRows.count(); j++)
        {
            QString row = noteRows.at(j);
            if (row.contains(strToBefind))
            {
                QString resultStr;
                resultStr.sprintf("Pag%d - ",i);
                resultStr.append(row);
                resultStrList->append(resultStr);
                noteSelectedList->append(i);
                numFindRequiredList->append(numFindRequired);
                QString tstStr = "@#GAP_BEGIN#@";
                tstStr.append(row);
                tstStr.append("@#GAP_END#@");
                numFindRequired += tstStr.split(strToBefind).count()-1;
            }
        }
    }
}
