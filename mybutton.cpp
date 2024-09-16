#include "mybutton.h"

#include <QPainter>

MyButton::MyButton(QWidget* parent) : QPushButton{parent} {}

void MyButton::setImage(QString normal, QString hover, QString pressed) {
    m_normal = normal;
    m_hover = hover;
    m_pressed = pressed;
    m_pixmap.load(m_normal);
    update();
}

void MyButton::mousePressEvent(QMouseEvent* event) {
    if (event->button() == Qt::MouseButton::LeftButton) {
        m_pixmap.load(m_pressed);
        update();
    }
    QPushButton::mousePressEvent(event);
}

void MyButton::mouseReleaseEvent(QMouseEvent* event) {

    if (event->button() == Qt::MouseButton::LeftButton) {
        m_pixmap.load(m_normal);
        update();
    }
    QPushButton::mouseReleaseEvent(event);
}

void MyButton::enterEvent(QEvent* event) {
    m_pixmap.load(m_hover);
    update();
}

void MyButton::leaveEvent(QEvent* event) {
    m_pixmap.load(m_normal);
    update();
}

void MyButton::paintEvent(QPaintEvent* event) {
    QPainter p(this);
    p.drawPixmap(rect(), m_pixmap);
}
