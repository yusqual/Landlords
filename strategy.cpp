#include "strategy.h"

Strategy::Strategy(Player* player, const Cards& cards) : m_player(player), m_cards(cards) {}

Cards Strategy::findSamePointCards(Card::CardPoint point, int count) {
    Cards cards;
    if ((count < 1) || (count > 4))
        return cards;
    // 大小王
    if (point == Card::Card_SJ || point == Card::Card_BJ) {
        if (count > 1)
            return Cards();
        Card card(point, Card::Suit_Begin);
        if (m_cards.contains(card)) {
            cards.add(card);
        }
        return cards;
    }

    int findCount = 0;
    for (int suit = Card::Suit_Begin + 1; suit < Card::Suit_End; ++suit) {
        Card card(point, suit);
        if (m_cards.contains(card)) {
            cards.add(card);
            ++findCount;
            if (findCount == count)
                return cards;
        }
    }
    return Cards();
}

QVector<Cards> Strategy::findCardsByCount(int count) {
    QVector<Cards> cardsArray;
    if ((count < 1) || (count > 4))
        return cardsArray;
    for (auto point = Card::Card_3; point < Card::Card_End; point = (Card::CardPoint)(point + 1)) {
        if (count == m_cards.pointCount(point)) {
            Cards cs;
            cs << findSamePointCards(point, count);
            cardsArray << cs;
        }
    }
    return cardsArray;
}

Cards Strategy::getRangeCards(Card::CardPoint begin, Card::CardPoint end) {
    Cards rangeCards;
    for (auto point = begin; point < end; point = (Card::CardPoint)(point + 1)) {
        int count = m_cards.pointCount(point);
        Cards cs = findSamePointCards(point, count);
        rangeCards << cs;
    }
    return rangeCards;
}

QVector<Cards> Strategy::findCardType(PlayHand hand, bool beat) {
    PlayHand::HandType type = hand.getHandType();
    Card::CardPoint point = hand.getCardPoint();
    int extra = hand.getExtra();

    Card::CardPoint beginPoint = beat ? Card::CardPoint(point + 1) : Card::Card_3;

    switch (type) {
    case PlayHand::Hand_Single:
        return getCards(beginPoint, 1);
    case PlayHand::Hand_Pair:
        return getCards(beginPoint, 2);
    case PlayHand::Hand_Triple:
        return getCards(beginPoint, 3);
    case PlayHand::Hand_Triple_Single:
        return getTripleSingleOrPair(beginPoint, PlayHand::Hand_Single);
    case PlayHand::Hand_Triple_Pair:
        return getTripleSingleOrPair(beginPoint, PlayHand::Hand_Pair);
    case PlayHand::Hand_Plane:
        return getPlane(beginPoint);
    case PlayHand::Hand_Plane_Two_Single:
        return getPlane2SingleOr2Pair(beginPoint, PlayHand::Hand_Single);
    case PlayHand::Hand_Plane_Two_Pair:
        return getPlane2SingleOr2Pair(beginPoint, PlayHand::Hand_Pair);
    case PlayHand::Hand_Seq_Pair: {
        CardInfo info;
        info.begin = beginPoint;
        info.end = Card::Card_Q;
        info.number = 2;
        info.base = 3;
        info.extra = extra;
        info.beat = beat;
        info.getSeq = &Strategy::getBaseSeqPair;
        return getSepPairOrSeqSingle(info);
    }
    case PlayHand::Hand_Seq_Single: {
        CardInfo info;
        info.begin = beginPoint;
        info.end = Card::Card_10;
        info.number = 1;
        info.base = 5;
        info.extra = extra;
        info.beat = beat;
        info.getSeq = &Strategy::getBaseSeqSingle;
        return getSepPairOrSeqSingle(info);
    }
    case PlayHand::Hand_Bomb:
        return getBomb(beginPoint);
    default:
        return QVector<Cards>();
    }
}

QVector<Cards> Strategy::getCards(Card::CardPoint point, int number) {
    QVector<Cards> findCardsArray;
    for (auto pt = point; pt < Card::Card_End; pt = (Card::CardPoint)(pt + 1)) {
        Cards cs = findSamePointCards(pt, number);
        if (!cs.isEmpty())
            findCardsArray << cs;
    }
    return findCardsArray;
}

QVector<Cards> Strategy::getTripleSingleOrPair(Card::CardPoint begin, PlayHand::HandType type) {
    // 找到点数相同的三张牌
    QVector<Cards> findCardArray = getCards(begin, 3);
    if (!findCardArray.isEmpty()) {
        // 将找到的牌从用户手中删除
        Cards remainCards = m_cards;
        remainCards.remove(findCardArray);
        // 搜索牌型
        // 搜索单牌或双牌
        Strategy st(m_player, remainCards);
        QVector<Cards> cardsArray = st.findCardType(PlayHand(type, Card::Card_Begin, 0), false);
        if (!cardsArray.isEmpty()) {
            // 组合
            for (int i = 0; i < findCardArray.size(); ++i) {
                findCardArray[i].add(cardsArray.at(i));
            }
        } else {
            findCardArray.clear();
        }
    }
    return findCardArray;
}
