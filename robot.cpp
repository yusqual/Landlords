#include "robot.h"

Robot::Robot(QObject* parent) : Player{parent} { m_type = Player::Type::Robot; }

void Robot::prepareCallLord() {}

void Robot::preparePlayHand() {}
