#ifndef GAPPMAINWINDOW_H
#define GAPPMAINWINDOW_H

#include <QMainWindow>
#include "GAPP_Data.h"


namespace Ui {
class GappMainWindow;
}

class GappMainWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit GappMainWindow(QWidget *parent = 0);
    ~GappMainWindow();

    void addData(GAPP_Data* pData);
    void closeEvent(QCloseEvent* event);

    void updateGUI();
    void updateTitle();
    void updateData();

public slots:
    void deleteNote(int index);   
    void noteIndexHasChanged(int sel);

private slots:
    void on_action_Open_activated();
    void on_action_Save_activated();
    void on_action_About_activated();
    void on_actionGo_Next_or_create_new_activated();
    void on_actionGo_Previous_activated();
    void on_action_Set_or_change_password_activated();
    void on_actionShow_index_activated();

private:
    Ui::GappMainWindow *ui;

    GAPP_Data* p_data;
    void AfxInfoBox(QString txt);
};

#endif // GAPPMAINWINDOW_H
