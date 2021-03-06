#ifndef PREFERENCESDIALOG_H
#define PREFERENCESDIALOG_H

#include <QDialog>

#include "GSettings.h"

namespace Ui {
class PreferencesDIalog;
}

class PreferencesDIalog : public QDialog
{
    Q_OBJECT
    
public:
    explicit PreferencesDIalog(GSettings* pSettings, QWidget *parent = 0);
    ~PreferencesDIalog();
    
private slots:
    void on_RestoreDefault_clicked();
    void on_SelectFile_clicked();

    void on_buttonBox_accepted();

    void on_LanguageSel_currentIndexChanged(int index);

private:
    Ui::PreferencesDIalog *ui;
    GSettings* m_settings;

    void UpdateGUI(void);
    bool GUIIsUpdating;
    void UpdateSettings(void);
};

#endif // PREFERENCESDIALOG_H
