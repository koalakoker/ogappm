#ifndef QPASSWORDDIAG_H
#define QPASSWORDDIAG_H

#include <QDialog>
#include "ui_qpassworddiag.h"

#define QPASSDLG_NEWFILE 2
#define QPASSDLG_OPEN    3

class QPasswordDiag : public QDialog
{
    Q_OBJECT

public:
    QPasswordDiag(QWidget *parent = 0);
    ~QPasswordDiag();

    QString m_pass(void);

private slots:
    void on_cancel_clicked();

    void on_ok_clicked();

    void on_newfile_clicked();

    void on_open_clicked();

private:
    Ui::QPasswordDiagClass ui;
};

#endif // QPASSWORDDIAG_H
