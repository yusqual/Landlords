#include "cardpanel.h"

#include <QPainter>

CardPanel::CardPanel(QWidget* parent)
    : QWidget{parent}, m_isFront(true) {
}

void CardPanel::setImage(QPixmap& front, QPixmap& back) {
    m_front = front;
    m_back = back;
    setFixedSize(m_front.size());
    update();
}

void CardPanel::paintEvent(QPaintEvent* event) {
    QPainter p(this);
    if (m_isFront)
        p.drawPixmap(rect(), m_front);
    else
        p.drawPixmap(rect(), m_back);
}

void CardPanel::mousePressEvent(QMouseEvent* event) {
}
