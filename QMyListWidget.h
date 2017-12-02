#ifndef QMYLISTWIDGET_H
#define QMYLISTWIDGET_H

#include <QListWidget>

class QMyListWidget : public QListWidget
{
    Q_OBJECT

public:
    explicit QMyListWidget(QWidget *parent = nullptr);

protected:
    void keyPressEvent(QKeyEvent* event);

signals:
    void keyPressed(int key);

};

#endif // QMYLISTWIDGET_H
