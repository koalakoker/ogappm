#include "preferencesdialog.h"
#include "ui_preferencesdialog.h"

#include "qmyfiledialog.h"
#include <QMessageBox>

PreferencesDIalog::PreferencesDIalog(GSettings* pSettings, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::PreferencesDIalog)
{
    GUIIsUpdating = false;
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
    GUIIsUpdating = true;
    ui->DefaultNoteFile->setText(m_settings->Get(GSETTING_DEFNOTEFILE_CFGSTR)->Value().toString());
    ui->Autosave->setChecked(m_settings->Get(GSETTING_AUTOSAVE_CFGSTR)->Value().toBool());
    ui->ShowTips->setChecked(m_settings->Get(GSETTING_SHOWTIPS_CFGSTR)->Value().toBool());
    ui->SaveWinState->setChecked(m_settings->Get(GSETTING_SAVEWINSTATE_CFGSTR)->Value().toBool());
    if (m_settings->Get(GSETTING_LANGUAGE_CFGSTR)->Value().toString()=="it_IT")
    {
        ui->LanguageSel->setCurrentIndex(1);
    }
    else
    {
        ui->LanguageSel->setCurrentIndex(0);
    }
    GUIIsUpdating = false;
}

void PreferencesDIalog::UpdateSettings(void)
{
    m_settings->SetValue(GSETTING_DEFNOTEFILE_CFGSTR,QVariant(ui->DefaultNoteFile->text()));
    m_settings->SetValue(GSETTING_AUTOSAVE_CFGSTR,QVariant(ui->Autosave->checkState()));
    m_settings->SetValue(GSETTING_SHOWTIPS_CFGSTR,QVariant(ui->ShowTips->checkState()));
    m_settings->SetValue(GSETTING_SAVEWINSTATE_CFGSTR,QVariant(ui->SaveWinState->checkState()));
    if (ui->LanguageSel->currentIndex() == 0)
    {
        m_settings->SetValue(GSETTING_LANGUAGE_CFGSTR,QString("en_EN"));
    }
    else
    {
        m_settings->SetValue(GSETTING_LANGUAGE_CFGSTR,QString("it_IT"));
    }
}

void PreferencesDIalog::on_SelectFile_clicked()
{
    QMyFileDialog* fileDiag = new QMyFileDialog(this,tr("Open .ogp file"),"","*.ogp");
    if (fileDiag->exec())
    {
        ui->DefaultNoteFile->setText(fileDiag->selectedFiles()[0]);
    }
}

void PreferencesDIalog::on_buttonBox_accepted()
{
    UpdateSettings();
}

void PreferencesDIalog::on_LanguageSel_currentIndexChanged(int index)
{
    if (!GUIIsUpdating)
    {
        QMessageBox::information(this,tr("Information"),tr("Restart OGapp to take effect"));
    }
}
