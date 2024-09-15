#ifndef USERPLAYER_H
#define USERPLAYER_H

#include "player.h"
#include <QObject>

class UserPlayer : public Player {
    Q_OBJECT
public:
    using Player::Player; // 使用继承构造函数
    explicit UserPlayer(QObject* parent = nullptr);

    // Player interface
public:
    void prepareCallLord() override;
    void preparePlayHand() override;
};

#endif // USERPLAYER_H
