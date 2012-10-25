#include "gappmainwindow.h"
#include "ui_gappmainwindow.h"

#include "QMyPlainTextEdit.h"
#include "cnoteindex.h"
#include "ChangePasswordDiag.h"
#include "aboutdialog.h"
#include "defines.h"
#include "qmyfiledialog.h"
#include "noteselection.h"
#include "finddialog.h"
#include <QMessageBox>

GappMainWindow::GappMainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::GappMainWindow)
{
    ui->setupUi(this);
    ui->testo1->setFocus();
    connect(ui->noteTab, SIGNAL(tabCloseRequested(int)), this, SLOT(deleteNote(int)));
    m_strToBeFind.clear();;
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
        connect(plainTextEdit,SIGNAL(textChanged()),this,SLOT(noteTextChanged()));

        gridLayout->addWidget((QPlainTextEdit*)plainTextEdit, 0, 0, 1, 1);

        int nextPage = ui->noteTab->NextNewPag();

        QString out;
        out.sprintf("Pag%d",nextPage);

        ui->noteTab->addTab(newPag, out);
    }
    ui->noteTab->setCurrentIndex(0);

    updateTitle();
}

void GappMainWindow::updateTitle()
{
    // Update window title
    QString title("Open GAPP version ");
    title.append(PRG_VERSION);
    title.append (" - NOTES - ");
    QFileInfo fileInfo(p_data->fileName());
    title.append(fileInfo.fileName());
    if (p_data->IsCrypted())
    {
        title.append(" <Crypted>");
    }
    if (p_data->HasModified())
    {
        title.append("*");
        ui->action_Save->setEnabled(true);
    }
    else
    {
        ui->action_Save->setEnabled(false);
    }
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
    updateData();
    updateTitle();
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
    QMyFileDialog* fileDiag = new QMyFileDialog(this,"Open .ogp file","","*.ogp");
    if (fileDiag->exec())
    {
        QStringList fileName = fileDiag->selectedFiles();
        int update;
        // Fist save old datas
        updateData();
        p_data->saveData();
        // The open new one
        QString file = fileName[0];
        do
        {
            p_data->LoadData(file,&update);
            switch(update)
            {
            case OPEN_FILE:
                {
                    QMyFileDialog* fileDiag = new QMyFileDialog(NULL,"Open .ogp file","","*.ogp");
                    if (fileDiag->exec())
                    {
                        QStringList fileName = fileDiag->selectedFiles();
                        file = fileName[0];
                    }
                    delete fileDiag;
                }
                break;
            case NEW_FILE:
                {
                    QMyFileDialog* fileDiag = new QMyFileDialog(NULL,"Create new .ogp file","","*.ogp");
                    fileDiag->setAcceptMode(QFileDialog::AcceptSave);
                    fileDiag->setDefaultSuffix("ogp");
                    if (fileDiag->exec())
                    {
                        QStringList fileName = fileDiag->selectedFiles();
                        file = fileName[0];
                        QFile fileToBeDeleted(file);
                        if (fileToBeDeleted.exists())
                        {
                            fileToBeDeleted.remove();
                        }
                    }
                    delete fileDiag;
                }
                break;
            }
        }
        while ((update == ERROR_PASSWORD_ERROR) ||
               (update == NEW_FILE) ||
               (update == OPEN_FILE));
        switch (update)
        {
        case DATA_CRYPTED_PASSWORD_MATCH:
        case FILE_EXISTING_NO_CRYPTED:
            {
                updateGUI();
            }
            break;
        case NEW_FILE_TO_BE_CREATED:
            {
                // Set default 4 page empty
                p_data->notesRemoveAll();
                p_data->notesAdd("");
                p_data->notesAdd("");
                p_data->notesAdd("");
                p_data->notesAdd("");
                updateGUI();
            }
            break;
        default:
            {

            }
            break;
        }
    }
    delete fileDiag;
}

void GappMainWindow::on_action_About_activated()
{
    AboutDialog aboutDlg;
    QString title("Open GAPP version ");
    title.append(PRG_VERSION);
    title.append (" - NOTES - ");
    aboutDlg.SetAboutTxt(title);
    aboutDlg.SetIconAuthorTxt("Icon Author: Saki (Alexandre Moore)\nHomePage: http://sa-ki.deviantart.com\nLicense: Free for non-commercial use.\n\nShimmer Icons - Free Set\nIcon Design by Creative Freedom\nhttp://www.creativefreedom.co.uk/icon-design/\nAll copyright for Shimmer Icons belongs to Creative Freedom Ltd.\nhttp://creativecommons.org/licenses/by-nd/3.0/\n\nDowloaded from http://www.veryicon.com");
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
        connect(plainTextEdit,SIGNAL(textChanged()),this,SLOT(noteTextChanged()));

        gridLayout->addWidget((QPlainTextEdit*)plainTextEdit, 0, 0, 1, 1);

        nextPage = ui->noteTab->NextNewPag();

        QString out;
        out.sprintf("Pag%d",nextPage);

        ui->noteTab->addTab(newPag, out);
        ui->noteTab->setCurrentIndex(ui->noteTab->currentIndex()+1);

        updateData();
        updateTitle();
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
        updateTitle();
        statusBar()->showMessage("Password has been changed.");
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

void GappMainWindow::on_action_New_activated()
{
    // Autosave
    updateData();
    p_data->saveData();
    QMyFileDialog* fileDiag = new QMyFileDialog(NULL,"Create new .ogp file","","*.ogp");
    fileDiag->setAcceptMode(QFileDialog::AcceptSave);
    fileDiag->setDefaultSuffix("ogp");
    if (fileDiag->exec())
    {
        QStringList fileName = fileDiag->selectedFiles();
        p_data->notesRemoveAll();
        // Set default 4 page empty
        p_data->notesAdd("");
        p_data->notesAdd("");
        p_data->notesAdd("");
        p_data->notesAdd("");
        p_data->setFileName(fileName[0]);
        p_data->setPass("");
        updateGUI();
    }
    delete fileDiag;
}

void GappMainWindow::on_actionGet_notes_from_file_activated()
{
    QMyFileDialog* fileDiag = new QMyFileDialog(this,"Open .ogp file","","*.ogp");
    if (fileDiag->exec())
    {
        QStringList fileName = fileDiag->selectedFiles();
        // Open
        GAPP_Data noteToBeImported;
        int update;
        do
        {
            noteToBeImported.LoadData(fileName[0],&update);
        }
        while ((update == ERROR_PASSWORD_ERROR) ||
               (update == NEW_FILE) ||
               (update == OPEN_FILE));
        switch (update)
        {
        case DATA_CRYPTED_PASSWORD_MATCH:
        case FILE_EXISTING_NO_CRYPTED:
            {
                noteSelection noteSelDlg;
                // Fill
                QStringList previewList;
                noteToBeImported.NotePreview(&previewList);
                bool* noteMatchList = new bool(noteToBeImported.notesCount());
                p_data->NoteMatchList(noteMatchList,&noteToBeImported);
                noteSelDlg.SetMatchList(noteMatchList);
                int i;
                for (i = 0; i < previewList.count(); i++)
                {
                    noteSelDlg.AddItem(previewList.at(i),true,!noteMatchList[i]);
                }
                if (noteSelDlg.exec())
                {
                    // Import selected
                    noteSelDlg.GetSelectedList(noteMatchList);
                    for (i = 0; i < noteToBeImported.notesCount(); i++)
                    {
                        if (noteMatchList[i])
                        {
                            p_data->notesAdd(noteToBeImported.notesAt(i));
                        }
                    }
                    updateGUI();
                }
                delete noteMatchList;
            }
            break;
        default:
            {

            }
            break;
        }
    }
    delete fileDiag;
}

void GappMainWindow::on_action_Save_activated()
{
    updateData();
    p_data->saveData();
    updateTitle();
}

void GappMainWindow::on_actionS_ave_AS_activated()
{
    QMyFileDialog* fileDiag = new QMyFileDialog(this,"SaveAs .ogp file","","*.ogp");
    fileDiag->setAcceptMode(QFileDialog::AcceptSave);
    fileDiag->setDefaultSuffix("ogp");
    if (fileDiag->exec())
    {
        QStringList fileName = fileDiag->selectedFiles();
        updateData();
        p_data->setFileName(fileName[0]);
        p_data->saveData();
        updateTitle();
    }
    delete fileDiag;
}

void GappMainWindow::noteTextChanged()
{
    updateData();
    updateTitle();
}

void GappMainWindow::on_action_Find_activated()
{
    FindDialog dlg;
    // Init
    dlg.SetData(p_data);
    dlg.SetStrToBeFind(m_strToBeFind);
    if (dlg.exec())
    {
        // Go to finded note
        int index = dlg.noteSelected();
        ui->noteTab->setCurrentIndex(index);
        QWidget* pag = ui->noteTab->widget(index);
        QGridLayout* lay = (QGridLayout*)pag->layout();
        QLayoutItem* layItem = lay->itemAtPosition(0,0);
        QMyPlainTextEdit* txtNote = (QMyPlainTextEdit*)layItem->widget();
        m_strToBeFind = dlg.strToBeFind();
        QTextCursor cursor = txtNote->textCursor();
        cursor.setPosition(0);
        txtNote->setTextCursor(cursor);
        int i;
        for (i = 0; i < dlg.numFindRequired(); i++)
        {
            txtNote->find(m_strToBeFind);
        }
    }
}

void GappMainWindow::on_actionF_ind_next_activated()
{
    QWidget* pag = ui->noteTab->currentWidget();
    QGridLayout* lay = (QGridLayout*)pag->layout();
    QLayoutItem* layItem = lay->itemAtPosition(0,0);
    QMyPlainTextEdit* txtNote = (QMyPlainTextEdit*)layItem->widget();
    txtNote->find(m_strToBeFind);
}
