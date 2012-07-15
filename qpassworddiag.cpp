#include "qpassworddiag.h"

QPasswordDiag::QPasswordDiag(QWidget *parent)
    : QDialog(parent)
{
	ui.setupUi(this);
}

QPasswordDiag::~QPasswordDiag()
{

}

QString QPasswordDiag::m_pass(void)
{
	return ui.m_pass->text();
}
