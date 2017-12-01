#ifndef FINDDIALOG_H
#define FINDDIALOG_H

#include <QDialog>
#include "GAPP_Data.h"

namespace Ui {
class FindDialog;
}

class FindDialog : public QDialog
{
    Q_OBJECT
    
public:
    explicit FindDialog(GAPP_Data* data, QString strToBeFind, QWidget *parent = 0);
    ~FindDialog();

    int noteSelected(){return m_noteSelected;}
    int numFindRequired(){return m_numFindRequired;}
    QString strToBeFind(){return m_strToBeFind;}

private slots:
    void on_stringToBeFound_textChanged(const QString &arg1);
    void on_searchResults_currentRowChanged(int currentRow);

private:
    Ui::FindDialog *ui;
    GAPP_Data* m_data;

    QStringList m_resultStrList;
    QList<int> m_noteSelectedList;
    QList<int> m_numFindRequiredList;

    QString m_strToBeFind;
    int m_noteSelected;
    int m_numFindRequired;
};

#endif // FINDDIALOG_H
