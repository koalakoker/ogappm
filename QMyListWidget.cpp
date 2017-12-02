#include "QMyListWidget.h"

#include <QKeyEvent>

QMyListWidget::QMyListWidget(QWidget *parent) : QListWidget(parent)
{
}

void QMyListWidget::keyPressEvent(QKeyEvent* event)
{
    emit keyPressed(event->key());
    QListWidget::keyPressEvent(event);
}
