#include "player.h"

Player::Player(QObject* parent) : QObject{parent} {}

Player::Player(QString name, QObject* parent) : QObject(parent), m_name(name) {}

void Player::prepareCallLord() {}

void Player::preparePlayHand() {}
