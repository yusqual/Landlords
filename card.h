#ifndef CARD_H
#define CARD_H

#include <QVector>

class Card {
public:
    // 花色
    enum CardSuit {
        Suit_Begin,
        Diamond, // 方块
        Club,    // 梅花
        Heart,   // 红桃
        Spade,   // 黑桃
        Suit_End
    };
    // 点数
    enum CardPoint {
        Card_Begin,
        Card_3,
        Card_4,
        Card_5,
        Card_6,
        Card_7,
        Card_8,
        Card_9,
        Card_10,
        Card_J,
        Card_Q,
        Card_K,
        Card_A,
        Card_2,
        Card_SJ, // 小王
        Card_BJ, // 大王
        Card_End
    };

    Card();
    Card(CardPoint point, CardSuit suit);

    // Getter Setter
    inline void setPoint(CardPoint point) { m_point = point; }
    inline void setSuit(CardSuit suit) { m_suit = suit; }
    inline CardPoint point() const { return m_point; }
    inline CardSuit suit() const { return m_suit; }

private:
    CardPoint m_point;
    CardSuit m_suit;
};

// 定义类型的别名
using CardList = QVector<Card>;

// 操作符重载==
bool operator==(const Card& left, const Card& right);
bool operator<(const Card& c1, const Card& c2);
// 重写全局函数 qHash
uint qHash(const Card& card);

#endif // CARD_H
