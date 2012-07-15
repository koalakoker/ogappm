#ifndef CHANGEPASSWORDDIAG_H
#define CHANGEPASSWORDDIAG_H

#include <QtGui/QDialog>
#include "ui_ChangePasswordDiag.h"

class ChangePasswordDiag : public QDialog
{
    Q_OBJECT

public:
    ChangePasswordDiag(QWidget *parent = 0);
    ~ChangePasswordDiag();

    void setOldPassword(QString str);
    QString newPassword();

private:
    Ui::ChangePasswordDiagClass ui;

    QString m_oldPass;

    void AfxMessageBox(QString txt);

private slots:
	void checkPass(void);
	void clickButton(QAbstractButton* button);
};

#endif // CHANGEPASSWORDDIAG_H
