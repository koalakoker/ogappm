#ifndef CNOTEINDEX_H
#define CNOTEINDEX_H

#include <QtGui/QDialog>
#include "ui_cnoteindex.h"

class CNoteIndex : public QDialog
{
    Q_OBJECT

public:
    CNoteIndex(QWidget *parent = 0);
    ~CNoteIndex();

    void noteIndexAdd(QString note);
    void setCurSelected(int sel);

private slots:
    void noteIndexChange();

private:
    Ui::CNoteIndexClass ui;

signals:
	void noteIndexHasChanged(int sel);

};

#endif // CNOTEINDEX_H
