#ifndef TIPSDIALOG_H
#define TIPSDIALOG_H

#include <QDialog>
#include "GSettings.h"
#include "Tips.h"

namespace Ui {
class TipsDialog;
}

class TipsDialog : public QDialog
{
    Q_OBJECT
    
public:
    explicit TipsDialog(Tips* pTips, int selected = 0, QWidget *parent = 0);
    ~TipsDialog();
    void SetSettings(GSettings* settings){m_settings = settings;};
    
private slots:
    void on_disableTips_clicked();

    void on_NextTip_clicked();

    void on_PreviousTip_clicked();

private:
    Ui::TipsDialog *ui;
    GSettings* m_settings;
    Tips* m_tips;
    int m_selected;

    void ShowSelectedTips(void);
};

#endif // TIPSDIALOG_H
