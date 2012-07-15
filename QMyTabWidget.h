/*
 * QMyTabWidget.h
 *
 *  Created on: 03/gen/2010
 *      Author: koala
 */

#include <QTabWidget>

#ifndef QMYTABWIDGET_H_
#define QMYTABWIDGET_H_

class QMyTabWidget :  public QTabWidget {
public:
	QMyTabWidget(QWidget *parent = 0);
	virtual ~QMyTabWidget();

	int NextNewPag(void);

protected:
    void dropEvent ( QDropEvent * event );
    void dragEnterEvent ( QDragEnterEvent * event );
    void dragMoveEvent ( QDragMoveEvent * event );
};

#endif /* QMYTABWIDGET_H_ */
