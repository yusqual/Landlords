#ifndef GAMEPANEL_H
#define GAMEPANEL_H

#include "card.h"
#include "cardpanel.h"
#include "gamecontrol.h"
#include <QLabel>
#include <QMainWindow>
#include <QMap>
QT_BEGIN_NAMESPACE
namespace Ui {
class GamePanel;
}
QT_END_NAMESPACE

class GamePanel : public QMainWindow {
    Q_OBJECT

public:
    GamePanel(QWidget* parent = nullptr);
    ~GamePanel();
    void gameControlInit();                              // 初始化游戏控制类信息
    void updatePlayerScore();                            // 更新分数面板的分数
    void initCardMap();                                  // 初始化卡牌Map容器
    void cropImage(QPixmap& pix, int x, int y, Card& c); // 裁剪图片
    void initButtonGroup();                              // 初始化游戏按钮组
    void initPlayerContext();

private:
    enum CardAlign { Horizontal, Vertical };
    struct PlayerContext {
        // 1. 玩家扑克牌显示的区域
        QRect cardRect;
        // 2. 出牌的区域
        QRect playHandRect;
        // 3. 扑克牌的对齐方式
        CardAlign align;
        // 4. 扑克牌显示正面还是反面
        bool isFrontSide;
        // 5. 游戏过程中的提示信息, 比如: 不出
        QLabel* info;
        // 6. 玩家的头像
        QLabel* roleImg;
        // 7. 玩家刚打出的牌
        Cards lastCards;
    };
    Ui::GamePanel* ui;
    QPixmap m_bkImage;
    GameControl* m_gameCtl;
    QVector<Player*> m_playerList;
    QMap<Card, CardPanel*> m_cardMap;
    QSize m_cardSize;
    QPixmap m_cardBackImg;
    QMap<Player*, PlayerContext> m_contextMap;
    // QWidget interface
protected:
    void paintEvent(QPaintEvent* event);
};
#endif // GAMEPANEL_H
