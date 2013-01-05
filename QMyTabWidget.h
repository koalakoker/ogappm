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
    Q_OBJECT
public:
    explicit QMyTabWidget(QWidget *parent = 0);
    ~QMyTabWidget();

	int NextNewPag(void);

signals:
    void tabMoved(int from, int to);

protected:
    void dropEvent ( QDropEvent * event );
    void dragEnterEvent ( QDragEnterEvent * event );
    void dragMoveEvent ( QDragMoveEvent * event );

private slots:
    void moveTab(int from, int to);
};

#endif /* QMYTABWIDGET_H_ */
