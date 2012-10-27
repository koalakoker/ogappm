#ifndef TIPSDIALOG_H
#define TIPSDIALOG_H

#include <QDialog>
#include "GSettings.h"

namespace Ui {
class TipsDialog;
}

class TipsDialog : public QDialog
{
    Q_OBJECT
    
public:
    explicit TipsDialog(QWidget *parent = 0);
    ~TipsDialog();
    void SetSettings(GSettings* settings){m_settings = settings;};
    
private slots:
    void on_disableTips_clicked();

private:
    Ui::TipsDialog *ui;
    GSettings* m_settings;
};

#endif // TIPSDIALOG_H
