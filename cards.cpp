#include "cards.h"

#include <QRandomGenerator>

Cards::Cards() {}

Cards::Cards(const Card& card) { add(card); }

Cards& Cards::operator<<(const Card& card) {
    add(card);
    return *this;
}

Card::CardPoint Cards::maxPoint() {
    Card::CardPoint max = Card::Card_Begin;
    if (!m_cards.isEmpty()) {
        for (auto it = m_cards.begin(); it != m_cards.end(); ++it) {
            if (it->point() > max)
                max = it->point();
        }
    }
    return max;
}

Card::CardPoint Cards::minPoint() {
    Card::CardPoint min = Card::Card_End;
    if (!m_cards.isEmpty()) {
        for (auto it = m_cards.begin(); it != m_cards.end(); ++it) {
            if (it->point() < min)
                min = it->point();
        }
    }
    return min;
}

int Cards::pointCount(Card::CardPoint point) {
    int count = 0;
    if (!m_cards.isEmpty()) {
        for (auto it = m_cards.begin(); it != m_cards.end(); ++it) {
            if (it->point() == point)
                ++count;
        }
    }
    return count;
}

Card Cards::takeRandomCard() {
    // 生成一个随机数
    int num = QRandomGenerator::global()->bounded(m_cards.size());
    auto it = m_cards.constBegin();
    for (int i = 0; i < num; ++i, ++it)
        ;
    Card card = *it;
    m_cards.erase(it);
    return card;
}

CardList Cards::toCardList(SortType type) {
    CardList list;
    for (auto it = m_cards.begin(); it != m_cards.end(); ++it) {
        list << *it;
    }
    if (type == Asc) {
        std::sort(list.begin(), list.end(),
                  [&](const Card& c1, const Card& c2) { return (c1.point() < c2.point()) || ((c1.point() == c2.point()) && c1.suit() < c2.suit()); });
    } else if (type == Desc) {
        std::sort(list.begin(), list.end(),
                  [&](const Card& c1, const Card& c2) { return (c1.point() > c2.point()) || ((c1.point() == c2.point()) && c1.suit() > c2.suit()); });
    }
    return list;
}

Cards& Cards::operator<<(const Cards& cards) {
    add(cards);
    return *this;
}
