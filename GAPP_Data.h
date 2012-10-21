/*
 * GAPP_Data.h
 *
 *  Created on: Jan 4, 2010
 *      Author: gianluigi forte
 */

#ifndef GAPP_DATA_H_
#define GAPP_DATA_H_

#include <QObject>
#include <QStringList>

#define FILE_EXISTING_NO_CRYPTED    0
#define DATA_CRYPTED_PASSWORD_MATCH 1
#define NEW_FILE_TO_BE_CREATED      2
#define OPEN_FILE                   3
#define NEW_FILE                    4
#define ERROR_READING_FILE          5
#define ERROR_FILE_NOT_VALID        6
#define ERROR_NOT_HANDLED           7
#define ERROR_PASSWORD_ERROR        8


class GAPP_Data: public QObject {
	Q_OBJECT
public:
	GAPP_Data(QObject *parent = 0);
	virtual ~GAPP_Data();

	bool LoadData(QString fileName,int* retVal);
    bool LoadDataOffline(QString fileName,int* retVal,QString userPass);
	bool saveData(void);

    void NotePreview(QStringList* previewList);

	int notesCount();
	QString notesAt(int i);
	void notesRemoveAll();
	void notesAdd(QString str);

	void setPass(QString str);
	QString Pass(void);

    QString fileName() {return m_fileName;}
    void setFileName(QString fileName){m_fileName = fileName;}

    bool HasModified(void);
    bool IsCrypted(void);

	// Debug
	void AfxMessageBox(QString txt);

private:
	QStringList m_notes;
    QStringList m_notesCripted;

	QString m_pass;
	QString m_fileName;

    bool m_hasModified;

	void GCriptAppunti(const char* command);
};

#endif /* GAPP_DATA_H_ */
