#include "gapp.h"
#include <QLabel>
#include <QLineEdit>
#include <QGridLayout>
#include <QIntValidator>
#include <QDoubleValidator>

GAPP::GAPP(QWidget *parent)
    : QDialog(parent)
{
	ui.setupUi(this);

	QGridLayout* gLay = new QGridLayout(this);
	QLabel* labelInt = new QLabel("&Int");
	gLay->addWidget(labelInt,0,0);
	QLineEdit* editInt = new QLineEdit();
	labelInt->setBuddy(editInt);
	gLay->addWidget(editInt,0,1);
	QLabel* labelFloat = new QLabel("&Float");
	gLay->addWidget(labelFloat,1,0);
	QLineEdit* editFloat = new QLineEdit();
	labelFloat->setBuddy(editFloat);
	gLay->addWidget(editFloat,1,1);

	QIntValidator* intValidator = new QIntValidator(10,100,this);
	editInt->setValidator(intValidator);
	QDoubleValidator* floatValidator = new QDoubleValidator(10.0,20.0,2,this);
	editFloat->setValidator(floatValidator);

}

GAPP::~GAPP()
{

}
