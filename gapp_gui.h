#ifndef GAPP_GUI_H
#define GAPP_GUI_H

#include <QtGui/QDialog>
#include "ui_gapp_gui.h"
#include "GAPP_Data.h"

class GAPP_GUI : public QDialog
{
    Q_OBJECT

public:
    GAPP_GUI(QWidget *parent = 0);
    ~GAPP_GUI();

    void addData(GAPP_Data* pData);
    void closeEvent(QCloseEvent* event);

    void updateGUI();
    void updateData();

public slots:
	void nextPage();
	void previousPage();
	void deleteNote(int index);
	void noteIndex();
	void noteIndexHasChanged(int sel);

	void changePassword();
	void openProperties();
    void saveAs();

private slots:
    void on_toolButton_clicked();

private:
    Ui::GAPP_GUIClass ui;

    GAPP_Data* p_data;

    void AfxInfoBox(QString txt);
};

#endif // GAPP_GUI_H
