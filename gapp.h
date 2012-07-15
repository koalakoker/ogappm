#ifndef GAPP_H
#define GAPP_H

#include <QtGui/QDialog>
#include "ui_gapp.h"

class GAPP : public QDialog
{
    Q_OBJECT

public:
    GAPP(QWidget *parent = 0);
    ~GAPP();

private:
    Ui::GAPPClass ui;
};

#endif // GAPP_H
