/*
 * QMyPlainTextEdit.h
 *
 *  Created on: 31/dic/2009
 *      Author: koala
 */

#include <QPlainTextEdit>

#ifndef QMYPLAINTEXTEDIT_H_
#define QMYPLAINTEXTEDIT_H_

class QMyPlainTextEdit :  public QPlainTextEdit {

public:
	explicit QMyPlainTextEdit(QWidget *parent = 0);
	virtual ~QMyPlainTextEdit();
protected:
    void dropEvent ( QDropEvent * event );
    void dragEnterEvent ( QDragEnterEvent * event );
    void dragMoveEvent ( QDragMoveEvent * event );
    void mousePressEvent (QMouseEvent *e);
    void keyPressEvent (QKeyEvent *e);
    void keyReleaseEvent (QKeyEvent *e);
private:
    bool m_controlKeyPressed;
    bool m_WKeyPressed;
};

#endif /* QMYPLAINTEXTEDIT_H_ */
