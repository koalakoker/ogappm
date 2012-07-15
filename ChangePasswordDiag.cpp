#include "ChangePasswordDiag.h"
#include <QMessageBox>

ChangePasswordDiag::ChangePasswordDiag(QWidget *parent)
    : QDialog(parent)
{
	ui.setupUi(this);

	connect(ui.buttonBox, SIGNAL(accepted()), this, SLOT(checkPass()));
	connect(ui.buttonBox, SIGNAL(clicked(QAbstractButton*)), this, SLOT(clickButton(QAbstractButton*)));
}

ChangePasswordDiag::~ChangePasswordDiag()
{

}

void ChangePasswordDiag::setOldPassword(QString str)
{
	m_oldPass = str;
}

QString ChangePasswordDiag::newPassword()
{
	return ui.newPass->text();
}

void ChangePasswordDiag::checkPass(void)
{
	if (ui.oldPass->text() != m_oldPass)
	{
		AfxMessageBox("Old password is not correct!");
		return;
	}
	if (ui.newPass->text() != ui.confirmPass->text())
	{
		AfxMessageBox("New password and Confirm password are different!");
		return;
	}
	if (ui.newPass->text() == ui.oldPass->text())
	{
		if (ui.newPass->text()!="")
		{
			AfxMessageBox("New password and new password are the same!");
			return;
		}
	}
	emit (accept());
}

void ChangePasswordDiag::clickButton(QAbstractButton* button)
{
	if (ui.buttonBox->standardButton(button) == QDialogButtonBox::Reset)
	{
		if (ui.newPass->isHidden())
		{
			ui.newPass->show();
			ui.newPassLabel->show();
			ui.confirmPass->show();
			ui.confirmPassLabel->show();
		}
		else
		{
			ui.newPass->setText("");
			ui.newPass->hide();
			ui.newPassLabel->hide();
			ui.confirmPass->setText("");
			ui.confirmPass->hide();
			ui.confirmPassLabel->hide();
		}
	}
}

void ChangePasswordDiag::AfxMessageBox(QString txt)
{
	QMessageBox* errMsg = new QMessageBox(QMessageBox::Critical,QString("Error"),txt);
	errMsg->exec();
}
