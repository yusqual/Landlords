#ifndef CARDPANEL_H
#define CARDPANEL_H

#include <QWidget>

#include "card.h"
#include "player.h"

class CardPanel : public QWidget {
    Q_OBJECT
public:
    explicit CardPanel(QWidget* parent = nullptr);

    // 设置图片
    void setImage(QPixmap& front, QPixmap& back);
    inline QPixmap getImage() { return m_front; }

    // 设置扑克牌显示哪一面
    inline void setFrontSide(bool flag) { m_isFront = flag; }
    inline bool isFront() { return m_isFront; }

    // 记录窗口是否被选中了
    inline void setSelected(bool flag) { m_isSelected = flag; }
    inline bool isSelected() { return m_isSelected; }

    // 扑克牌的花色以及点数
    inline void setCard(Card& card) { m_card = card; }
    inline Card getCard() { return m_card; }

    // 扑克牌的所有者
    inline void setOwner(Player* player) {}
    inline Player* getOwner() { return m_owner; }

protected:
    void paintEvent(QPaintEvent* event);
    void mousePressEvent(QMouseEvent* event);

signals:

private:
    QPixmap m_front;
    QPixmap m_back;
    bool m_isFront;
    bool m_isSelected;
    Card m_card;
    Player* m_owner;
};

#endif // CARDPANEL_H
