#ifndef GAMECONTROL_H
#define GAMECONTROL_H

#include "robot.h"
#include "userplayer.h"
#include <QObject>

class GameControl : public QObject {
    Q_OBJECT
public:
    // 游戏状态
    enum GameStatus {
        DispatchCard, // 发牌
        CallingLord,  // 叫/抢地主
        PlayingHand   // 出牌
    };
    // 玩家状态
    enum PlayerStatus {
        ThinkingForCallLord, // 考虑叫/抢地主
        ThinkingForPlayHand, // 考虑出牌
        Winning              // 获胜
    };

    explicit GameControl(QObject* parent = nullptr);

    // 初始化玩家
    void playerInit();

    // 得到玩家实例对象
    inline Robot* getLeftRobot() { return m_robotLeft; }
    inline Robot* getRightRobot() { return m_robotRight; }
    inline UserPlayer* getUserPlayer() { return m_user; }
    inline void setCurrPlayer(Player* player) { m_currPlayer = player; }
    inline Player* getCurrPlayer() { return m_currPlayer; }

    // 得到出牌玩家和打出的牌
    inline Player* getPendPlayer() { return m_pendPlayer; }
    inline Cards getPendCards() { return m_pendCards; }

    // 初始化扑克牌
    void initAllCards();
    // 每次发一张牌
    inline Card takeOneCard() { return m_allCards.takeRandomCard(); }
    // 发最后三张牌
    inline Cards getSurplusCards() { return m_allCards; }
    // 重置卡牌数据
    void resetCardData();

    // 准备叫地主
    void startLordCard();
    // 成为地主
    void becomeLord(Player* player);

    // 清空所有玩家的得分
    void clearPlayerScore();

    // 处理叫地主

    // 处理出牌

signals:

private:
    Robot* m_robotLeft;
    Robot* m_robotRight;
    UserPlayer* m_user;
    Player* m_currPlayer;
    Player* m_pendPlayer;
    Cards m_pendCards;
    Cards m_allCards;
};

#endif // GAMECONTROL_H
