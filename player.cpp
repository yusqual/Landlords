#include "player.h"

Player::Player(QObject* parent) : QObject{parent}, m_score(0), m_isWin(0) {}

Player::Player(QString name, QObject* parent) : Player(parent) { m_name = name; }

void Player::prepareCallLord() {}

void Player::preparePlayHand() {}
