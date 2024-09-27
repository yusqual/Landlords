#include "gamecontrol.h"
#include "playhand.h"
#include <QRandomGenerator>
#include <QTimer>

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

    // 处理玩家发射的信号
    connect(m_user, &UserPlayer::notifyGrabLordBet, this, &GameControl::onGrabBet);
    connect(m_robotLeft, &UserPlayer::notifyGrabLordBet, this, &GameControl::onGrabBet);
    connect(m_robotRight, &UserPlayer::notifyGrabLordBet, this, &GameControl::onGrabBet);

    // 传递出牌玩家
    connect(this, &GameControl::pendingInfo, m_robotLeft, &Robot::storePendingInfo);
    connect(this, &GameControl::pendingInfo, m_robotRight, &Robot::storePendingInfo);
    connect(this, &GameControl::pendingInfo, m_user, &Robot::storePendingInfo);

    // 处理玩家出牌
    connect(m_robotLeft, &Robot::notifyPlayHand, this, &GameControl::onPlayHand);
    connect(m_robotRight, &Robot::notifyPlayHand, this, &GameControl::onPlayHand);
    connect(m_user, &Robot::notifyPlayHand, this, &GameControl::onPlayHand);
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

void GameControl::startLordCard() {
    m_currPlayer->prepareCallLord();
    emit playerStatusChanged(m_currPlayer, ThinkingForCallLord);
}

void GameControl::becomeLord(Player* player, int bet) {
    m_curBet = bet;
    player->setRole(Player::Lord);
    player->getPrevPlayer()->setRole(Player::Farmer);
    player->getNextPlayer()->setRole(Player::Farmer);
    m_currPlayer = player;
    player->storeDispatchCard(m_allCards); // 地主获得最后三张牌
    QTimer::singleShot(1000, this, [=]() {
        emit gameStatusChanged(PlayingHand);
        emit playerStatusChanged(player, ThinkingForPlayHand);
        m_currPlayer->preparePlayHand(); // 轮到地主准备出牌
    });
}

void GameControl::clearPlayerScore() {
    m_robotLeft->setScore(0);
    m_robotRight->setScore(0);
    m_user->setScore(0);
}

void GameControl::onGrabBet(Player* player, int bet) {
    // 1. 通知主界面玩家叫地主了(更新信息提示)
    if (bet == 0 || m_betRecord.bet >= bet) {
        emit notifyGrabLordBet(player, 0, false); // 机器人叫地主分数低于可选分数, 直接不抢
    } else if (m_betRecord.bet == 0 && bet > 0) {
        // 第一个抢地主的玩家
        emit notifyGrabLordBet(player, bet, true);
    } else {
        emit notifyGrabLordBet(player, bet, false);
    }
    // 2. 判断玩家下注是不是3分, 如果是则抢地主结束
    if (bet == 3) {
        // 玩家成为地主
        becomeLord(player, bet);
        m_betRecord.reset();
        return;
    }
    // 3.下注不够3分, 比较分数，高的是地主
    if (m_betRecord.bet < bet) {
        m_betRecord.bet = bet;
        m_betRecord.player = player;
    }
    ++m_betRecord.times;
    if (m_betRecord.times == 3) {
        if (m_betRecord.bet == 0) {
            emit gameStatusChanged(DispatchCard);
        } else {
            becomeLord(m_betRecord.player, m_betRecord.bet);
        }
        m_betRecord.reset();
        return;
    }
    // 4. 切换玩家, 通知下一个玩家继续抢地主
    m_currPlayer = player->getNextPlayer();
    // 发送信号给主界面, 告知当前状态仍为抢地主
    emit playerStatusChanged(m_currPlayer, ThinkingForCallLord);
    m_currPlayer->prepareCallLord();
}

void GameControl::onPlayHand(Player* player, Cards& card) {
    // 1. 将玩家出牌的信号转发给主界面
    emit notifyPlayHand(player, card);
    // 2. 判断是否是空牌，给其他玩家发送信号
    if (!card.isEmpty()) {
        m_pendCards = card;
        m_pendPlayer = player;
        emit pendingInfo(player, card);
    }
    // 若有炸弹则底分翻倍
    PlayHand::HandType type = PlayHand(card).getHandType();
    if (type == PlayHand::Hand_Bomb || type == PlayHand::Hand_Bomb_Jokers) {
        m_curBet = m_curBet * 2;
    }
    // 3. 判断玩家牌是否出完, 是则计算总分
    if (player->getCards().isEmpty()) {
        Player* prev = player->getPrevPlayer();
        Player* next = player->getNextPlayer();
        if (player->getRole() == Player::Lord) {
            player->setScore(player->getScore() + 2 * m_curBet);
            prev->setScore(prev->getScore() - m_curBet);
            next->setScore(next->getScore() - m_curBet);
            player->setWin(true);
            prev->setWin(false);
            next->setWin(false);
        } else {
            player->setWin(true);
            player->setScore(player->getScore() + m_curBet);
            if (prev->getRole() == Player::Lord) {
                prev->setScore(prev->getScore() - 2 * m_curBet);
                next->setScore(next->getScore() + m_curBet);
                prev->setWin(false);
                next->setWin(true);
            } else {
                next->setScore(next->getScore() - 2 * m_curBet);
                prev->setScore(prev->getScore() + m_curBet);
                next->setWin(false);
                prev->setWin(true);
            }
        }
        emit playerStatusChanged(player, GameControl::Winning);
        return;
    }
    // 4. 牌未出完则继续
    m_currPlayer = player->getNextPlayer();
    m_currPlayer->preparePlayHand();
    emit playerStatusChanged(m_currPlayer, GameControl::ThinkingForPlayHand);
}
