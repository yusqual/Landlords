#ifndef ROBOT_H
#define ROBOT_H

#include "player.h"
#include <QObject>

class Robot : public Player {
    Q_OBJECT
public:
    using Player::Player; // 使用继承构造函数
    explicit Robot(QObject* parent = nullptr);

    // Player interface
public:
    void prepareCallLord() override;
    void preparePlayHand() override;
};

#endif // ROBOT_H
