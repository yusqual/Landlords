#include "gamecontrol.h"
#include <QRandomGenerator>

GameControl::GameControl(QObject* parent) : QObject{parent} {}

void GameControl::playerInit() {
    // 对象实例化
    m_robotLeft = new Robot("机器人A", this);
    m_robotRight = new Robot("机器人B", this);
    m_user = new UserPlayer("我", this);
    // 设置相关属性
    // 头像显示
    m_robotLeft->setDirection(Player::Direction::Left);
    m_robotRight->setDirection(Player::Direction::Right);
    m_user->setDirection(Player::Direction::Right);
    // 性别
    Player::Sex sex = (Player::Sex)QRandomGenerator::global()->bounded(2);
    m_robotLeft->setSex(sex);
    sex = (Player::Sex)QRandomGenerator::global()->bounded(2);
    m_robotRight->setSex(sex);
    sex = (Player::Sex)QRandomGenerator::global()->bounded(2);
    m_user->setSex(sex);
    // 出牌顺序
    m_user->setPrevPlayer(m_robotLeft);
    m_user->setNextPlayer(m_robotRight);
    m_robotLeft->setPrevPlayer(m_robotRight);
    m_robotLeft->setNextPlayer(m_user);
    m_robotRight->setPrevPlayer(m_user);
    m_robotRight->setNextPlayer(m_robotLeft);
    // 指定当前玩家
    m_currPlayer = m_user;
}

void GameControl::initAllCards() {
    m_allCards.clear();
    for (int p = Card::Card_Begin + 1; p < Card::Card_SJ; ++p) {
        for (int s = Card::Suit_Begin + 1; s < Card::Suit_End; ++s) {
            m_allCards.add(Card((Card::CardPoint)p, (Card::CardSuit)s));
        }
    }
    m_allCards.add(Card(Card::Card_SJ, Card::Suit_Begin));
    m_allCards.add(Card(Card::Card_BJ, Card::Suit_Begin));
}

void GameControl::resetCardData() {
    // 洗牌
    initAllCards();
    // 清空所有玩家的牌
    m_robotLeft->clearCards();
    m_robotRight->clearCards();
    m_user->clearCards();
    // 初始化出牌玩家和出的牌
    m_pendPlayer = nullptr;
    m_pendCards.clear();
}

void GameControl::startLordCard() { m_currPlayer->prepareCallLord(); }

void GameControl::becomeLord(Player* player) {
    player->setRole(Player::Lord);
    player->getPrevPlayer()->setRole(Player::Farmer);
    player->getNextPlayer()->setRole(Player::Farmer);
    m_currPlayer = player;
    player->storeDispatchCard(m_allCards); // 地主获得最后三张牌
    m_currPlayer->preparePlayHand();       // 轮到地主准备出牌
}

void GameControl::clearPlayerScore() {
    m_robotLeft->setScore(0);
    m_robotRight->setScore(0);
    m_user->setScore(0);
}
