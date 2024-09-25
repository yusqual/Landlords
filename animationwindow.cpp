#include "animationwindow.h"

#include <QPainter>
#include <QTimer>

AnimationWindow::AnimationWindow(QWidget* parent) : QWidget{parent} {}

void AnimationWindow::showBetScore(int bet) {
    switch (bet) {
    case 1:
        m_image.load(":/images/score1.png");
        break;
    case 2:
        m_image.load(":/images/score2.png");
        break;
    case 3:
        m_image.load(":/images/score3.png");
        break;
    default:
        break;
    }
    update();
    QTimer::singleShot(2000, this, &AnimationWindow::hide);
}

void AnimationWindow::paintEvent(QPaintEvent* event) {
    QPainter p(this);
    p.drawPixmap(0, 0, m_image.width(), m_image.height(), m_image);
}
