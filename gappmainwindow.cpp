#include "gappmainwindow.h"
#include "ui_gappmainwindow.h"

#include "QMyPlainTextEdit.h"
#include "cnoteindex.h"
#include "ChangePasswordDiag.h"
#include "aboutdialog.h"
#include "defines.h"

#include <QMessageBox>
#include <QFileDialog>

GappMainWindow::GappMainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::GappMainWindow)
{
    ui->setupUi(this);

    this->setWindowFlags(Qt::WindowSystemMenuHint);
    ui->testo1->setFocus();

    // Connection
    connect(ui->noteTab, SIGNAL(tabCloseRequested(int)), this, SLOT(deleteNote(int)));

}

GappMainWindow::~GappMainWindow()
{
    delete ui;
}

void GappMainWindow::addData(GAPP_Data* pData)
{
    p_data = pData;
}

void GappMainWindow::closeEvent(QCloseEvent* event)
{
    updateData();
    p_data->saveData();
    event->accept();
}

void GappMainWindow::updateGUI()
{
    // Clear all tabs
    int i, c = ui->noteTab->count();
    for (i = 0; i < c; i++)
    {
        ui->noteTab->removeTab(0);
    }

    // Update Tabs
    for (i = 0; i < p_data->notesCount(); i++)
    {
        //p_data->AfxMessageBox(p_data->notesAt(i));

        QWidget *newPag = new QWidget();
        //newPag->setObjectName(QString::fromUtf8("newPag"));
        QGridLayout *gridLayout = new QGridLayout(newPag);
        gridLayout->setSpacing(6);
        gridLayout->setContentsMargins(11, 11, 11, 11);
        //gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        QMyPlainTextEdit *plainTextEdit = new QMyPlainTextEdit(newPag);
        plainTextEdit->setPlainText(p_data->notesAt(i));
        //plainTextEdit->setObjectName(QString::fromUtf8("plainTextEdit"));

        gridLayout->addWidget((QPlainTextEdit*)plainTextEdit, 0, 0, 1, 1);

        int nextPage = ui->noteTab->NextNewPag();

        QString out;
        out.sprintf("Pag%d",nextPage);

        ui->noteTab->addTab(newPag, out);
    }
    ui->noteTab->setCurrentIndex(0);

    // Update window title
    QString title("Open GAPP version ");
    title.append(PRG_VERSION);
    title.append (" - NOTES - ");
    QFileInfo fileInfo(p_data->fileName());
    title.append(fileInfo.fileName());
    this->setWindowTitle(title);
}

void GappMainWindow::updateData()
{
    // Clear all data
    p_data->notesRemoveAll();

    int i, c = ui->noteTab->count();
    for (i = 0; i < c; i++)
    {
        QWidget* pag = ui->noteTab->widget(i);
        QGridLayout* lay = (QGridLayout*)pag->layout();
        QLayoutItem* layItem = lay->itemAtPosition(0,0);
        QMyPlainTextEdit* txtNote = (QMyPlainTextEdit*)layItem->widget();
        QString txt = txtNote->toPlainText();
        p_data->notesAdd(txt);
    }
}

void GappMainWindow::deleteNote(int index)
{
    ui->noteTab->removeTab(index);
}

void GappMainWindow::noteIndexHasChanged(int sel)
{
    ui->noteTab->setCurrentIndex(sel);
}

void GappMainWindow::AfxInfoBox(QString txt)
{
    QMessageBox* errMsg = new QMessageBox(QMessageBox::Information,QString("Info"),txt);
    errMsg->exec();
}

void GappMainWindow::on_action_Open_activated()
{
    QFileDialog* fileDiag = new QFileDialog(this,"Open .ogp file","","*.ogp");
    if (fileDiag->exec())
    {
        QStringList fileName = fileDiag->selectedFiles();
        int update;
        // Fist save old datas
        updateData();
        p_data->saveData();
        // The open new one
        if (p_data->LoadData(fileName[0],&update))
        {
            if (update == UPDATE_ON_LOAD)
            {
                this->updateGUI();
            }
        }
    }
}

void GappMainWindow::on_action_Save_activated()
{
    QFileDialog* fileDiag = new QFileDialog(this,"SaveAs .ogp file","","*.ogp");
    fileDiag->setAcceptMode(QFileDialog::AcceptSave);
    fileDiag->setDefaultSuffix("ogp");
    if (fileDiag->exec())
    {
        QStringList fileName = fileDiag->selectedFiles();
        updateData();
        p_data->setFileName(fileName[0]);
        p_data->saveData();
        // Data are now cripted so is necessary to re-load to decript
        int update;
        if (p_data->LoadData(fileName[0],&update))
        {
            if (update == UPDATE_ON_LOAD)
            {
                this->updateGUI();
            }
        }
    }
}

void GappMainWindow::on_action_About_activated()
{
    AboutDialog aboutDlg;
    QString title("Open GAPP version ");
    title.append(PRG_VERSION);
    title.append (" - NOTES - ");
    aboutDlg.SetAboutTxt(title);
    aboutDlg.exec();
}

void GappMainWindow::on_actionGo_Next_or_create_new_activated()
{
    int nextPage = ui->noteTab->currentIndex() + 1;
    if (nextPage < ui->noteTab->count())
    {
        ui->noteTab->setCurrentIndex(nextPage);
    }
    else
    {
        QWidget *newPag = new QWidget();
        newPag->setObjectName(QString::fromUtf8("newPag"));
        QGridLayout *gridLayout = new QGridLayout(newPag);
        gridLayout->setSpacing(6);
        gridLayout->setContentsMargins(11, 11, 11, 11);
        gridLayout->setObjectName(QString::fromUtf8("gridLayout"));
        QMyPlainTextEdit *plainTextEdit = new QMyPlainTextEdit(newPag);
        plainTextEdit->setObjectName(QString::fromUtf8("plainTextEdit"));

        gridLayout->addWidget((QPlainTextEdit*)plainTextEdit, 0, 0, 1, 1);

        nextPage = ui->noteTab->NextNewPag();

        QString out;
        out.sprintf("Pag%d",nextPage);

        ui->noteTab->addTab(newPag, out);
        ui->noteTab->setCurrentIndex(ui->noteTab->currentIndex()+1);
    }
}

void GappMainWindow::on_actionGo_Previous_activated()
{
    ui->noteTab->setCurrentIndex(ui->noteTab->currentIndex()-1);
}

void GappMainWindow::on_action_Set_or_change_password_activated()
{
    ChangePasswordDiag diag;
    diag.setOldPassword(p_data->Pass());
    if (diag.exec() == QDialog::Accepted)
    {
        p_data->setPass(diag.newPassword());
        AfxInfoBox("Password has been changed.");
    }
}

void GappMainWindow::on_actionShow_index_activated()
{
    CNoteIndex diag;
    int i;
    for (i = 0; i < ui->noteTab->count(); i++)
    {
        QWidget* page = ui->noteTab->widget(i);
        QGridLayout* layout = (QGridLayout*)(page->layout());
        QMyPlainTextEdit* note = (QMyPlainTextEdit*)(layout->itemAtPosition(0,0)->widget());
        QString summary = note->toPlainText().left(110);
        QStringList summary_list = summary.split('\n');
        diag.noteIndexAdd(ui->noteTab->tabText(i) + " - " + summary_list.at(0));
    }
    diag.setCurSelected(ui->noteTab->currentIndex());
    connect(&diag, SIGNAL(noteIndexHasChanged(int)),this,SLOT(noteIndexHasChanged(int)));
    diag.exec();
}
