#include "playhand.h"

PlayHand::PlayHand() {}

PlayHand::PlayHand(Cards& cards) {
    classify(cards);
    judgeCardType();
}

PlayHand::PlayHand(HandType type, Card::CardPoint pt, int extra) : m_type(type), m_pt(pt), m_extra(extra) {}

bool PlayHand::canBeat(const PlayHand& other) {
    if (m_type == Hand_Unknown)
        return false;
    // 对方放弃出牌
    if (other.m_type == Hand_Pass)
        return true;
    if (m_type == Hand_Bomb_Jokers)
        return true;
    if (m_type == Hand_Bomb && other.m_type >= Hand_Single && other.m_type <= Hand_Seq_Single)
        return true;
    // 双方牌型一致
    if (m_type == other.m_type) {
        if (m_type == Hand_Seq_Pair || m_type == Hand_Seq_Single) {
            return m_pt > other.m_pt && m_extra == other.m_extra;
        } else {
            return m_pt > other.m_pt;
        }
    }
    return false;
}

void PlayHand::classify(Cards& cards) {
    CardList list = cards.toCardList();
    int cardRecord[Card::Card_End];
    memset(cardRecord, 0, sizeof(int) * Card::Card_End);
    for (int i = 0; i < list.size(); ++i) {
        Card c = list.at(i);
        ++cardRecord[c.point()];
    }

    m_oneCard.clear();
    m_twoCard.clear();
    m_threeCard.clear();
    m_fourCard.clear();

    for (int i = 0; i < Card::Card_End; ++i) {
        switch (cardRecord[i]) {
        case 1:
            m_oneCard.push_back((Card::CardPoint)i);
            break;
        case 2:
            m_twoCard.push_back((Card::CardPoint)i);
            break;
        case 3:
            m_threeCard.push_back((Card::CardPoint)i);
            break;
        case 4:
            m_fourCard.push_back((Card::CardPoint)i);
            break;
        default:
            break;
        }
    }
}

void PlayHand::judgeCardType() {
    m_type = Hand_Unknown;
    m_pt = Card::Card_Begin;
    m_extra = 0;
    if (isPass()) {
        m_type = Hand_Pass;
        return;
    }
    if (isSingle()) {
        m_type = Hand_Single;
        m_pt = m_oneCard[0];
    } else if (isPair()) {
        m_type = Hand_Pair;
        m_pt = m_twoCard[0];
    } else if (isTriple()) {
        m_type = Hand_Triple;
        m_pt = m_threeCard[0];
    } else if (isTripleSingle()) {
        m_type = Hand_Triple_Single;
        m_pt = m_threeCard[0];
    } else if (isTriplePair()) {
        m_type = Hand_Triple_Pair;
        m_pt = m_threeCard[0];
    } else if (isPlane()) {
        m_type = Hand_Plane;
        // 记录点数最小的牌
        m_pt = m_threeCard[0];
    } else if (isPlaneTwoSingle()) {
        m_type = Hand_Plane_Two_Single;
        // 记录点数最小的牌
        m_pt = m_threeCard[0];
    } else if (isPlaneTwoPair()) {
        m_type = Hand_Plane_Two_Pair;
        // 记录点数最小的牌
        m_pt = m_threeCard[0];
    } else if (isSeqPair()) {
        m_type = Hand_Seq_Pair;
        // 记录点数最小的牌
        m_pt = m_twoCard[0];
        m_extra = m_twoCard.size();
    } else if (isSeqSingle()) {
        m_type = Hand_Seq_Single;
        // 记录点数最小的牌
        m_pt = m_oneCard[0];
        m_extra = m_oneCard.size();
    } else if (isBomb()) {
        m_type = Hand_Bomb;
        // 记录点数最小的牌
        m_pt = m_fourCard[0];
    } else if (isBombSingle()) {
        m_type = Hand_Bomb_Single;
        // 记录点数最小的牌
        m_pt = m_fourCard[0];
    } else if (isBombPair()) {
        m_type = Hand_Bomb_Pair;
        // 记录点数最小的牌
        m_pt = m_fourCard[0];
    } else if (isBombTwoSingle()) {
        m_type = Hand_Bomb_Two_Single;
        // 记录点数最小的牌
        m_pt = m_fourCard[0];
    } else if (isBombJokers()) {
        m_type = Hand_Bomb_Jokers;
    } else if (isBombJokersSingle()) {
        m_type = Hand_Bomb_Jokers_Single;
    } else if (isBombJokersPair()) {
        m_type = Hand_Bomb_Jokers_Pair;
    } else if (isBombJokersTwoSingle()) {
        m_type = Hand_Bomb_Jokers_Two_Single;
    }
}

bool PlayHand::isPass() { return m_oneCard.isEmpty() && m_twoCard.isEmpty() && m_threeCard.isEmpty() && m_fourCard.isEmpty(); }

bool PlayHand::isSingle() { return m_oneCard.size() == 1 && m_twoCard.isEmpty() && m_threeCard.isEmpty() && m_fourCard.isEmpty(); }

bool PlayHand::isPair() { return m_oneCard.isEmpty() && m_twoCard.size() == 1 && m_threeCard.isEmpty() && m_fourCard.isEmpty(); }

bool PlayHand::isTriple() { return m_oneCard.isEmpty() && m_twoCard.isEmpty() && m_threeCard.size() == 1 && m_fourCard.isEmpty(); }

bool PlayHand::isTripleSingle() { return m_oneCard.size() == 1 && m_twoCard.isEmpty() && m_threeCard.size() == 1 && m_fourCard.isEmpty(); }

bool PlayHand::isTriplePair() { return m_oneCard.isEmpty() && m_twoCard.size() == 1 && m_threeCard.size() == 1 && m_fourCard.isEmpty(); }

bool PlayHand::isPlane() {
    if (m_oneCard.isEmpty() && m_twoCard.isEmpty() && m_threeCard.size() == 2 && m_fourCard.isEmpty()) {
        std::sort(m_threeCard.begin(), m_threeCard.end());
        if (m_threeCard[1] - m_threeCard[0] == 1 && m_threeCard[1] < Card::Card_2) { // 两种牌相邻且不是222AAA
            return true;
        }
    }
    return false;
}

bool PlayHand::isPlaneTwoSingle() {
    if (m_oneCard.size() == 2 && m_twoCard.isEmpty() && m_threeCard.size() == 2 && m_fourCard.isEmpty()) {
        std::sort(m_threeCard.begin(), m_threeCard.end());
        std::sort(m_oneCard.begin(), m_oneCard.end());
        if (m_threeCard[1] - m_threeCard[0] == 1 && m_threeCard[1] < Card::Card_2 && m_oneCard[0] != Card::Card_SJ && m_oneCard[1] != Card::Card_BJ) {
            return true;
        }
    }
    return false;
}

bool PlayHand::isPlaneTwoPair() {
    if (m_oneCard.isEmpty() && m_twoCard.size() == 2 && m_threeCard.size() == 2 && m_fourCard.isEmpty()) {
        std::sort(m_threeCard.begin(), m_threeCard.end());
        if (m_threeCard[1] - m_threeCard[0] == 1 && m_threeCard[1] < Card::Card_2) {
            return true;
        }
    }
    return false;
}

bool PlayHand::isSeqPair() {
    if (m_oneCard.isEmpty() && m_twoCard.size() >= 3 && m_threeCard.isEmpty() && m_fourCard.isEmpty()) {
        std::sort(m_twoCard.begin(), m_twoCard.end());
        if (m_twoCard.last() - m_twoCard.first() == (m_twoCard.size() - 1) && m_twoCard.first() >= Card::Card_3 && m_twoCard.last() < Card::Card_2) {
            return true;
        }
    }
    return false;
}

bool PlayHand::isSeqSingle() {
    if (m_oneCard.size() >= 5 && m_twoCard.isEmpty() && m_threeCard.isEmpty() && m_fourCard.isEmpty()) {
        std::sort(m_oneCard.begin(), m_oneCard.end());
        if (m_oneCard.last() - m_oneCard.first() == (m_oneCard.size() - 1) && m_oneCard.first() >= Card::Card_3 && m_oneCard.last() < Card::Card_2) {
            return true;
        }
    }
    return false;
}

bool PlayHand::isBomb() { return m_oneCard.isEmpty() && m_twoCard.isEmpty() && m_threeCard.isEmpty() && m_fourCard.size() == 1; }

bool PlayHand::isBombSingle() { return m_oneCard.size() == 1 && m_twoCard.isEmpty() && m_threeCard.isEmpty() && m_fourCard.size() == 1; }

bool PlayHand::isBombPair() { return m_oneCard.isEmpty() && m_twoCard.size() == 1 && m_threeCard.isEmpty() && m_fourCard.size() == 1; }

bool PlayHand::isBombTwoSingle() {
    if (m_oneCard.size() == 2 && m_twoCard.isEmpty() && m_threeCard.isEmpty() && m_fourCard.size() == 1) {
        std::sort(m_oneCard.begin(), m_oneCard.end());
        if (m_oneCard.first() != Card::Card_SJ && m_oneCard.last() != Card::Card_BJ) {
            return true;
        }
    }
    return false;
}

bool PlayHand::isBombJokers() {
    if (m_oneCard.size() == 2 && m_twoCard.isEmpty() && m_threeCard.isEmpty() && m_fourCard.isEmpty()) {
        std::sort(m_oneCard.begin(), m_oneCard.end());
        if (m_oneCard.first() == Card::Card_SJ && m_oneCard.last() == Card::Card_BJ) {
            return true;
        }
    }
    return false;
}

bool PlayHand::isBombJokersSingle() {
    if (m_oneCard.size() == 3 && m_twoCard.isEmpty() && m_threeCard.isEmpty() && m_fourCard.isEmpty()) {
        std::sort(m_oneCard.begin(), m_oneCard.end());
        if (m_oneCard[1] == Card::Card_SJ && m_oneCard[2] == Card::Card_BJ) {
            return true;
        }
    }
    return false;
}

bool PlayHand::isBombJokersPair() {
    if (m_oneCard.size() == 2 && m_twoCard.size() == 1 && m_threeCard.isEmpty() && m_fourCard.isEmpty()) {
        std::sort(m_oneCard.begin(), m_oneCard.end());
        if (m_oneCard[0] == Card::Card_SJ && m_oneCard[1] == Card::Card_BJ) {
            return true;
        }
    }
    return false;
}

bool PlayHand::isBombJokersTwoSingle() {
    if (m_oneCard.size() == 4 && m_twoCard.isEmpty() && m_threeCard.isEmpty() && m_fourCard.isEmpty()) {
        std::sort(m_oneCard.begin(), m_oneCard.end());
        if (m_oneCard[2] == Card::Card_SJ && m_oneCard[3] == Card::Card_BJ) {
            return true;
        }
    }
    return false;
}
