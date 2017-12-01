#include "QMyLineEdit.h"
#include <QKeyEvent>

QMyLineEdit::QMyLineEdit(QWidget *parent) : QLineEdit(parent)
{
}

void QMyLineEdit::keyPressEvent(QKeyEvent* event)
{
    emit keyPressed(event->key());
    QLineEdit::keyPressEvent(event);
}
