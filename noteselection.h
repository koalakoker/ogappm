#ifndef NOTESELECTION_H
#define NOTESELECTION_H

#include <QDialog>

namespace Ui {
class noteSelection;
}

class noteSelection : public QDialog
{
    Q_OBJECT
    
public:
    explicit noteSelection(QWidget *parent = 0);
    ~noteSelection();

    void AddItem(QString txt,bool Checkable,bool Checked);
    void SetMatchList(bool* matchList);
    void GetSelectedList(bool* selectedList);
    
private:
    Ui::noteSelection *ui;
    bool* m_mtchList;

private slots:
    void CheckAll(void);
    void DecheckAll(void);
    void CheckAllDifferent(void);
};

#endif // NOTESELECTION_H
