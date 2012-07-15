#ifndef QPASSWORDDIAG_H
#define QPASSWORDDIAG_H

#include <QtGui/QDialog>
#include "ui_qpassworddiag.h"

class QPasswordDiag : public QDialog
{
    Q_OBJECT

public:
    QPasswordDiag(QWidget *parent = 0);
    ~QPasswordDiag();

    QString m_pass(void);

private:
    Ui::QPasswordDiagClass ui;
};

#endif // QPASSWORDDIAG_H
