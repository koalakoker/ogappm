#ifndef QMYLINEEDIT_H
#define QMYLINEEDIT_H

#include <QLineEdit>

class QMyLineEdit : public QLineEdit
{
    Q_OBJECT
public:
    explicit QMyLineEdit(QWidget *parent = nullptr);

protected:
    void keyPressEvent(QKeyEvent* event);

signals:
    void keyPressed(int key);

};

#endif // QMYLINEEDIT_H
