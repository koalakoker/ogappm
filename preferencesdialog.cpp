#include "preferencesdialog.h"
#include "ui_preferencesdialog.h"

#include "qmyfiledialog.h"

PreferencesDIalog::PreferencesDIalog(GSettings* pSettings, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::PreferencesDIalog)
{
    ui->setupUi(this);

    this->m_settings = pSettings;
    UpdateGUI();
}

PreferencesDIalog::~PreferencesDIalog()
{
    delete ui;
}

void PreferencesDIalog::on_RestoreDefault_clicked()
{
    m_settings->RestoreDefault();
    UpdateGUI();
}

void PreferencesDIalog::UpdateGUI(void)
{
    ui->DefaultNoteFile->setText(m_settings->Get(GSETTING_DEFNOTEFILE_CFGSTR)->Value().toString());
    ui->Autosave->setChecked(m_settings->Get(GSETTING_AUTOSAVE_CFGSTR)->Value().toBool());
    ui->ShowTips->setChecked(m_settings->Get(GSETTING_SHOWTIPS_CFGSTR)->Value().toBool());
    ui->SaveWinState->setChecked(m_settings->Get(GSETTING_SAVEWINSTATE_CFGSTR)->Value().toBool());
}

void PreferencesDIalog::UpdateSettings(void)
{
    m_settings->SetValue(GSETTING_DEFNOTEFILE_CFGSTR,QVariant(ui->DefaultNoteFile->text()));
    m_settings->SetValue(GSETTING_AUTOSAVE_CFGSTR,QVariant(ui->Autosave->checkState()));
    m_settings->SetValue(GSETTING_SHOWTIPS_CFGSTR,QVariant(ui->ShowTips->checkState()));
    m_settings->SetValue(GSETTING_SAVEWINSTATE_CFGSTR,QVariant(ui->SaveWinState->checkState()));
}

void PreferencesDIalog::on_SelectFile_clicked()
{
    QMyFileDialog* fileDiag = new QMyFileDialog(this,"Open .ogp file","","*.ogp");
    if (fileDiag->exec())
    {
        ui->DefaultNoteFile->setText(fileDiag->selectedFiles()[0]);
    }
}

void PreferencesDIalog::on_buttonBox_accepted()
{
    UpdateSettings();
}
