#include "TipsDialog.h"
#include "ui_TipsDialog.h"
#include <QMessageBox>

TipsDialog::TipsDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::TipsDialog)
{
    ui->setupUi(this);
    m_settings = NULL;
}

TipsDialog::~TipsDialog()
{
    delete ui;
}

void TipsDialog::on_disableTips_clicked()
{
    if (ui->disableTips->checkState())
    {
        QMessageBox::information( this, "Information message", "You can re-enable the tips in the preference window", QMessageBox::Ok, 0 );
        if (m_settings)
        {
            m_settings->SetValue(GSETTING_SHOWTIPS_CFGSTR,false);
        }
    }
    else
    {
        if (m_settings)
        {
            m_settings->SetValue(GSETTING_SHOWTIPS_CFGSTR,true);
        }
    }
}
