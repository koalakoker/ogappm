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
#include "preferencesdialog.h"
#include "Tips.h"
#include "TipsDialog.h"
#include <QMessageBox>
#include <QSettings>

GappMainWindow::GappMainWindow(GAPP_Data* pData, GSettings* pSettings, QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::GappMainWindow)
{
    this->p_data = pData;
    this->p_settings = pSettings;

    ui->setupUi(this);
    ui->testo1->setFocus();
    connect(ui->noteTab, SIGNAL(tabCloseRequested(int)), this, SLOT(deleteNote(int)));
    connect(ui->noteTab,SIGNAL(tabMoved(int,int)),this,SLOT(noteMoved(int,int)));
    m_strToBeFind.clear();

    resize(QSize(800, 600));
    move(QPoint(20, 20));
    if (p_settings->Get(GSETTING_SAVEWINSTATE_CFGSTR)->Value().toBool())
    {
        readWindowsSettings();
    }

    // Show tips
    if (p_settings->Get(GSETTING_SHOWTIPS_CFGSTR)->Value().toBool())
    {
        Tips* pTips = new Tips();
        pTips->append(QString(tr("Welcome to OGapp!\n\nOGapp is a slim sized and fast program to manage your textual notes. Notes are organized in pages and it is possible to setup a main password to get the access to the notes. When the notes are password protected the .ogp file in which the notes are stored is encrypted.\nOGapp is cross platform and is available for Windows, Linux and (maybe in the future) Mac. It is based on Qt4 and is written in C++.")));
        pTips->append(QString(tr("The \"default\" note file\n\nWhen you start OGapp the \"default\" .ogp file will be opened. If it is the first time a new one will be created. When you close the program all the modifications you did during the last session are automatically saved. In this way you can access very fast to your \"default\" notes. This behavior can be modified launching the command \"Preference\".")));
        pTips->append(QString(tr("Encrypt file\n\nIf you want to protect your notes it is possible to set a password launching \"set or change password\" command. When the password is set the file .ogp is saved encrypted. OGapp uses the password as key to crypt or decrypt the data. Just a password hash is stored into the file for a verification if the password is correct. You can disable the encryption of the .ogp file launching \"set or change password\" command and push reset button (old password is required).")));
        pTips->append(QString(tr("Drag & Drop\n\nTry to drag a textual file (like .txt or any source file .c .cpp .h) from any window and drop it inside a note page. It will be added to that note page.\n\nIf you drop it into the tab labels (for example Pag1, Pag2, ...) a new page will be created.")));
        pTips->append(QString(tr("Find\n\nIf you want to find very fast a note that contains a particular string, simply press CTRL+F and type just few character of that string. A list of pages containing that string will appear in the result list. Double click on it and the note will be opened. Push F3 to find next occurrence inside that page.")));
        pTips->append(QString(tr("Launch links in browser\n\nPress CTRL+W and click on a URL (like https://sourceforge.net/projects/ogapp/) inside a note to open the link in the default browser.")));
        pTips->append(QString(tr("Change note pages order\n\nTry to drag one note tab (Pag1, Pag2, ...) over another note tab. The order of the pages will be modified.")));
        pTips->append(QString(tr("Note listing\n\nLaunch the command \"Show index\" to generate a list of note pages including as reference the first rows of the note. Double click on a page for a quick jump.")));
        pTips->append(QString(tr("Note file info\n\nWhen you open, create, save, saveAs or import a .ogp file you can see in the dialog a preview showing a short list of the pages including the first row of each note as a reference.\nIf the file is encrypted, just the number of pages is shown.")));
        pTips->append(QString(tr("Import from a file\n\nYou can import selectively notes from an existing .ogp file using the \"Import from file\" command. A dialog will show the notes that is present in the selected file highlighting the ones that are not present in the working document (this to avoid import a duplicate). Then you can select which notes will be added even importing duplicates.")));

        int tipSel = p_settings->Get(GSETTING_SHOWTIPSSEL_CFGSTR)->Value().toInt();
        TipsDialog* tipDlg = new TipsDialog(pTips,tipSel,this);
        tipDlg->SetSettings(p_settings);
        tipDlg->show();
        tipDlg->raise();
        tipSel++;
        if (tipSel >= pTips->count())
        {
            tipSel = 0;
        }
        p_settings->SetValue(GSETTING_SHOWTIPSSEL_CFGSTR,tipSel);
    }
}

GappMainWindow::~GappMainWindow()
{
    if (p_settings->Get(GSETTING_SAVEWINSTATE_CFGSTR)->Value().toBool())
    {
        writeWindowsSettings();
    }
    delete ui;
}

void GappMainWindow::closeEvent(QCloseEvent* event)
{
    updateData();
    bool saveIt = false;
    if (!p_settings->Get(GSETTING_AUTOSAVE_CFGSTR)->Value().toBool())
    {
        QMessageBox msg;
        msg.setText(tr("Do you want to save the notes into the file?"));
        msg.setIcon(QMessageBox::Question);
        msg.setStandardButtons(QMessageBox::Yes|QMessageBox::No);
        if (msg.exec() == QMessageBox::Yes)
        {
            saveIt = true;
        }
    }
    else
    {
        saveIt = true;
    }
    if (saveIt)
    {
        p_data->saveData();
    }
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

        int index = ui->noteTab->addTab(newPag, out);
        p_data->setNotesTitle(index,out);
    }
    ui->noteTab->setCurrentIndex(0);

    updateTitle();
}

void GappMainWindow::updateTitle()
{
    // Update window title
    QString title(tr("Open GAPP version "));
    title.append(PRG_VERSION);
    title.append (tr(" - NOTES - "));
    QFileInfo fileInfo(p_data->fileName());
    title.append(fileInfo.fileName());
    if (p_data->IsCrypted())
    {
        title.append(tr(" <Encrypted>"));
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
        QString noteTitle = ui->noteTab->tabText(i);
        QWidget* pag = ui->noteTab->widget(i);
        QGridLayout* lay = (QGridLayout*)pag->layout();
        QLayoutItem* layItem = lay->itemAtPosition(0,0);
        QMyPlainTextEdit* txtNote = (QMyPlainTextEdit*)layItem->widget();
        QString txt = txtNote->toPlainText();
        p_data->notesAdd(txt,noteTitle);
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

void GappMainWindow::noteMoved(int from, int to)
{
    updateData();
}

void GappMainWindow::AfxInfoBox(QString txt)
{
    QMessageBox* errMsg = new QMessageBox(QMessageBox::Information,QString(tr("Info")),txt);
    errMsg->exec();
}

void GappMainWindow::on_action_Open_activated()
{
    QMyFileDialog* fileDiag = new QMyFileDialog(this,tr("Open .ogp file"),"","*.ogp");
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
                    QMyFileDialog* fileDiag = new QMyFileDialog(NULL,tr("Open .ogp file"),"","*.ogp");
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
                    QMyFileDialog* fileDiag = new QMyFileDialog(NULL,tr("Create new .ogp file"),"","*.ogp");
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
                p_data->notesAdd("","Pag1");
                p_data->notesAdd("","Pag2");
                p_data->notesAdd("","Pag3");
                p_data->notesAdd("","Pag4");
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
    QString title(tr("Open GAPP version "));
    title.append(PRG_VERSION);
    title.append (tr(" - NOTES - "));
    aboutDlg.SetAboutTxt(title);
    aboutDlg.SetIconAuthorTxt(tr("Icon Author: Saki (Alexandre Moore)\nHomePage: http://sa-ki.deviantart.com\nLicense: Free for non-commercial use.\n\nShimmer Icons - Free Set\nIcon Design by Creative Freedom\nhttp://www.creativefreedom.co.uk/icon-design/\nAll copyright for Shimmer Icons belongs to Creative Freedom Ltd.\nhttp://creativecommons.org/licenses/by-nd/3.0/\n\nDowloaded from http://www.veryicon.com"));
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

        int index = ui->noteTab->addTab(newPag, out);
        p_data->setNotesTitle(index,out);
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
        statusBar()->showMessage(tr("Password has been changed."));
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
    QMyFileDialog* fileDiag = new QMyFileDialog(NULL,tr("Create new .ogp file"),"","*.ogp");
    fileDiag->setAcceptMode(QFileDialog::AcceptSave);
    fileDiag->setDefaultSuffix("ogp");
    if (fileDiag->exec())
    {
        QStringList fileName = fileDiag->selectedFiles();
        p_data->notesRemoveAll();
        // Set default 4 page empty
        p_data->notesAdd("","Pag1");
        p_data->notesAdd("","Pag2");
        p_data->notesAdd("","Pag3");
        p_data->notesAdd("","Pag4");
        p_data->setFileName(fileName[0]);
        p_data->setPass("");
        updateGUI();
    }
    delete fileDiag;
}

void GappMainWindow::on_actionGet_notes_from_file_activated()
{
    QMyFileDialog* fileDiag = new QMyFileDialog(this,tr("Open .ogp file"),"","*.ogp");
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
                            p_data->notesAdd(noteToBeImported.notesAt(i),"");
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
    QMyFileDialog* fileDiag = new QMyFileDialog(this,tr("SaveAs .ogp file"),"","*.ogp");
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
        m_strToBeFind = dlg.strToBeFind();
        if ((index >= 0) && (index < ui->noteTab->count()))
        {
            ui->noteTab->setCurrentIndex(index);
            QWidget* pag = ui->noteTab->widget(index);
            QGridLayout* lay = (QGridLayout*)pag->layout();
            QLayoutItem* layItem = lay->itemAtPosition(0,0);
            QMyPlainTextEdit* txtNote = (QMyPlainTextEdit*)layItem->widget();
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
}

void GappMainWindow::on_actionF_ind_next_activated()
{
    QWidget* pag = ui->noteTab->currentWidget();
    QGridLayout* lay = (QGridLayout*)pag->layout();
    QLayoutItem* layItem = lay->itemAtPosition(0,0);
    QMyPlainTextEdit* txtNote = (QMyPlainTextEdit*)layItem->widget();
    txtNote->find(m_strToBeFind);
}

void GappMainWindow::writeWindowsSettings()
 {
     QSettings settings(p_settings->Organization(),p_settings->Application());

     settings.beginGroup("MainWindow");
     settings.setValue("maximized", isMaximized());
     settings.setValue("size", size());
     settings.setValue("pos", pos());
     settings.endGroup();
 }

 void GappMainWindow::readWindowsSettings()
 {
     QSettings settings(p_settings->Organization(),p_settings->Application());

     settings.beginGroup("MainWindow");
     if ((settings.value("maximized",false).toBool())==false)
     {
        resize(settings.value("size", QSize(800, 600)).toSize());
        move(settings.value("pos", QPoint(200, 200)).toPoint());
     }
     else
     {
         setWindowState(windowState() | Qt::WindowMaximized);
     }
     settings.endGroup();
 }

void GappMainWindow::on_action_Preference_activated()
{
    PreferencesDIalog dlg(p_settings);
    if (dlg.exec())
    {
        // Do someting
    }
}

void GappMainWindow::on_actionQuit_activated()
{
    close();
}

