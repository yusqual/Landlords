#include "robot.h"
#include "robotgraplord.h"
#include "robotplayhand.h"
#include "strategy.h"

Robot::Robot(QObject* parent) : Player{parent} { m_type = Player::Type::Robot; }

void Robot::prepareCallLord() {
    RobotGrapLord* subThread = new RobotGrapLord(this);
    subThread->start();
}

void Robot::preparePlayHand() {
    RobotPlayHand* subThread = new RobotPlayHand(this);
    subThread->start();
}

void Robot::thinkCallLord() {
    // 基于权重和判断是否叫地主
    int weigth = 0;
    Strategy st(this, m_cards);
    weigth += st.getRangeCards(Card::Card_SJ, Card::Card_BJ).cardCount() * 6;
    auto optSeq = st.pickOptimalSeqSingles();
    weigth += optSeq.size() * 5;
    auto bombs = st.findCardsByCount(4);
    weigth += bombs.size() * 5;
    weigth += m_cards.pointCount(Card::Card_2) * 3;
    // 剔除已经计算过的牌
    Cards tmp = m_cards;
    tmp.remove(optSeq);
    tmp.remove(bombs);
    Cards card2 = st.getRangeCards(Card::Card_2, Card::Card_2);
    tmp.remove(card2);
    auto triples = Strategy(this, tmp).findCardsByCount(3);
    weigth += triples.size() * 4;
    tmp.remove(triples);
    auto pairs = Strategy(this, tmp).findCardsByCount(2);
    weigth += pairs.size() * 1;

    if (weigth >= 22) {
        grabLordBet(3);
    } else if (weigth >= 18) {
        grabLordBet(2);
    } else if (weigth >= 10) {
        grabLordBet(1);
    } else {
        grabLordBet(0);
    }
}

void Robot::thinkPlayHand() {
    Strategy st(this, m_cards);
    Cards cs = st.makeStrategy();
    playHand(cs);
}
