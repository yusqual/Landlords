#include "player.h"

Player::Player(QObject* parent) : QObject{parent}, m_score(0), m_isWin(0) {}

Player::Player(QString name, QObject* parent) : Player(parent) { m_name = name; }

void Player::grabLordBet(int point) { emit notifyGrabLordBet(this, point); }

void Player::storePendingInfo(Player* player, Cards& cards) { setPendingInfo(player, cards); }

void Player::prepareCallLord() {}

void Player::preparePlayHand() {}

void Player::thinkCallLord() {}

void Player::thinkPlayHand() {}
