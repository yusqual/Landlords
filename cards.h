#ifndef CARDS_H
#define CARDS_H

#include <QSet>

#include "card.h"

class Cards {
public:
    enum SortType { Asc, Desc, NoSort };
    Cards();
    Cards(const Card& card);

    // 添加扑克牌
    inline void add(const Card& card) { m_cards.insert(card); }
    inline void add(const Cards& cards) { m_cards.unite(cards.m_cards); }
    // 删除扑克牌
    inline void remove(Card& card) { m_cards.remove(card); }
    inline void remove(Cards& cards) { m_cards.subtract(cards.m_cards); }
    void remove(QVector<Cards>& cards);

    // 一次性插入多个数据 (操作符重载<<)
    Cards& operator<<(const Card& card);
    Cards& operator<<(const Cards& cards);

    // 扑克牌的数量
    inline int cardCount() { return m_cards.size(); }
    // 是否为空
    inline bool isEmpty() { return m_cards.isEmpty(); }
    // 清空扑克牌
    inline void clear() { m_cards.clear(); }

    // 最大点数
    Card::CardPoint maxPoint();
    // 最小点数
    Card::CardPoint minPoint();
    // 指定点数的牌的数量
    int pointCount(Card::CardPoint point);
    // 某张牌是否在集合中
    inline bool contains(const Card& card) { return m_cards.contains(card); }
    inline bool contains(const Cards& cards) { return m_cards.contains(cards.m_cards); }

    // 随机取出一张牌
    Card takeRandomCard();

    // QSet -> QVector
    CardList toCardList(SortType type = Desc);

private:
    QSet<Card> m_cards;
};

#endif // CARDS_H
