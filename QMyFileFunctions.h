/*
 * QMyFileFunctions.h
 *
 *  Created on: Jan 5, 2010
 *      Author: gianluigi forte
 */

#ifndef QMYFILEFUNCTIONS_H_
#define QMYFILEFUNCTIONS_H_

#include <QFile>
#include <QString>
#include <QStringList>

bool writeInt(QFile* file, int val);
int readInt(QFile* file, bool* retVal);
bool writeQString(QFile* file, QString str);
QString readQString(QFile* file, bool* retVal);
bool writeQStringList(QFile* file, QStringList strList);
bool readQStringList(QFile* file, QStringList* strList);

#endif /* QMYFILEFUNCTIONS_H_ */
