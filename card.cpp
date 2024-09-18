#include "card.h"

Card::Card() {}

Card::Card(CardPoint point, CardSuit suit) : m_point(point), m_suit(suit) {}

bool operator==(const Card& left, const Card& right) { return (left.point() == right.point() && left.suit() == right.suit()); }

uint qHash(const Card& card) { return card.point() * 100 + card.suit(); }

bool operator<(const Card& c1, const Card& c2) { return (c1.point() < c2.point()) || ((c1.point() == c2.point() && c1.suit() < c2.suit())); }
