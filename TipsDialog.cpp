#include "TipsDialog.h"
#include "ui_TipsDialog.h"
#include <QMessageBox>

TipsDialog::TipsDialog(Tips* pTips, int selected, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::TipsDialog)
{
    ui->setupUi(this);
    m_settings = NULL;

    m_tips = pTips;
    m_selected = selected;

    ShowSelectedTips();
}

TipsDialog::~TipsDialog()
{
    delete ui;
}

void TipsDialog::on_disableTips_clicked()
{
    if (ui->disableTips->checkState())
    {
        QMessageBox::information( this, tr("Information message"), tr("You can re-enable the tips in the preference window"), QMessageBox::Ok, 0 );
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

void TipsDialog::ShowSelectedTips(void)
{
    if ((m_selected >= 0) && (m_selected < m_tips->count()))
    {
        ui->infoText->setText(m_tips->at(m_selected));
    }
}

void TipsDialog::on_NextTip_clicked()
{
    m_selected++;
    if (m_selected >= m_tips->count())
    {
        m_selected = 0;
    }
    ShowSelectedTips();
}

void TipsDialog::on_PreviousTip_clicked()
{
    m_selected--;
    if (m_selected < 0)
    {
        m_selected = m_tips->count() - 1;
    }
    ShowSelectedTips();
}
