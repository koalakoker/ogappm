#include "qpassworddiag.h"

QPasswordDiag::QPasswordDiag(QWidget *parent)
    : QDialog(parent)
{
	ui.setupUi(this);

    ui.m_pass->setFocus();
}

QPasswordDiag::~QPasswordDiag()
{

}

QString QPasswordDiag::m_pass(void)
{
	return ui.m_pass->text();
}

void QPasswordDiag::on_cancel_clicked()
{
    this->done(QDialog::Rejected);
}

void QPasswordDiag::on_ok_clicked()
{
    this->done(QDialog::Accepted);
}

void QPasswordDiag::on_newfile_clicked()
{
    this->done(QPASSDLG_NEWFILE);
}

void QPasswordDiag::on_open_clicked()
{
    this->done(QPASSDLG_OPEN);
}
