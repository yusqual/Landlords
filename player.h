#ifndef PLAYER_H
#define PLAYER_H

#include "cards.h"
#include <QObject>

class Player : public QObject {
    Q_OBJECT
public:
    enum Role { Lord, Farmer };        // 角色
    enum Sex { Man, Woman };           // 性别
    enum Direction { Letf, Right };    // 头像显示方位
    enum Type { Robot, User, Unknow }; // 玩家类型

    explicit Player(QObject* parent = nullptr);
    explicit Player(QString name, QObject* parent = nullptr);

    // 名字
    inline void setName(QString name) { m_name = name; }
    inline QString getName() { return m_name; }
    // 角色
    inline void setRole(Role role) { m_role = role; }
    inline Role gerRole() { return m_role; }
    // 性别
    inline void setSex(Sex sex) { m_sex = sex; }
    inline Sex getSex() { return m_sex; }
    // 头像方位
    inline void setDirection(Direction dir) { m_direction = dir; }
    inline Direction getDirection() { return m_direction; }
    // 玩家类型
    inline void setType(Type type) { m_type = type; }
    inline Type getType() { return m_type; }
    // 分数
    inline void setScore(int score) { m_score = score; }
    inline int getScore() { return m_score; }
    // 游戏结果
    inline void setWin(bool flag) { m_isWin = flag; }
    inline bool isWin() { return m_isWin; }
    // 提供当前对象的上家/下家
    inline void setPrevPlayer(Player* player) { m_prev = player; }
    inline void setNextPlayer(Player* player) { m_next = player; }
    inline Player* getPrevPlayer() { return m_prev; }
    inline Player* getNextPlayer() { return m_next; }

    // 叫地主/抢地主
    void grabLordBet(int point);

    // 存储扑克牌
    inline void storeDispatchCard(Card& card) { m_cards.add(card); }
    inline void storeDispatchCard(Cards& cards) { m_cards.add(cards); }
    // 查看拥有的牌
    inline Cards getCards() { return m_cards; }
    // 清空玩家手中所有的牌
    inline clearCards() { m_cards.clear(); }
    // 出牌
    inline void playHand(Cards& cards) { m_cards.remove(cards); }
    // 设置出牌的玩家以及待处理的牌
    inline void setPendingInfo(Player* player, Cards& cards) {
        m_pendPlayer = player;
        m_pendCards = cards;
    }
    inline Player* getPendPlayer() { return m_pendPlayer; }
    inline Cards getPendCards() { return m_pendCards; }

    virtual void prepareCallLord(); // 准备叫地主
    virtual void preparePlayHand(); // 准备抢地主

signals:

protected:
    QString m_name;
    Role m_role;
    Sex m_sex;
    Direction m_direction;
    Type m_type;
    int m_score;
    bool m_isWin;
    Player* m_prev;
    Player* m_next;
    Cards m_cards;
    Cards m_pendCards;    // 需要比较的牌
    Player* m_pendPlayer; // 打出需要比较的牌的玩家
};

#endif // PLAYER_H
