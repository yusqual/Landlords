#ifndef GAMEPANEL_H
#define GAMEPANEL_H

#include "animationwindow.h"
#include "card.h"
#include "cardpanel.h"
#include "gamecontrol.h"
#include <QLabel>
#include <QMainWindow>
#include <QMap>
#include <QTimer>
QT_BEGIN_NAMESPACE
namespace Ui {
class GamePanel;
}
QT_END_NAMESPACE

class GamePanel : public QMainWindow {
    Q_OBJECT

public:
    enum AnimationType { ShunZi, LianDui, Plane, JokerBomb, Bomb, Bet };
    GamePanel(QWidget* parent = nullptr);
    ~GamePanel();
    void gameControlInit();                              // 初始化游戏控制类信息
    void updatePlayerScore();                            // 更新分数面板的分数
    void initCardMap();                                  // 初始化卡牌Map容器
    void cropImage(QPixmap& pix, int x, int y, Card& c); // 裁剪图片
    void initButtonGroup();                              // 初始化游戏按钮组
    void initPlayerContext();
    void initGameScene();
    void gameStatusProcess(GameControl::GameStatus status);                              // 修改游戏状态
    void startDispatchCard();                                                            // 发牌
    void cardMoveStep(Player* player, int curPos);                                       // 移动扑克牌
    void disposCard(Player* player, Cards& cards);                                       // 处理分发得到的扑克牌
    void updatePlayerCards(Player* player);                                              // 更新扑克牌在窗口中的显示
    QPixmap loadRoleImage(Player::Sex sex, Player::Direction direct, Player::Role role); // 加载玩家头像

    void onDispatchCard();                                                        // 定时器的处理动作
    void onPlayerStatusChanged(Player* player, GameControl::PlayerStatus status); // 处理玩家状态的变化
    void onGrabLordBet(Player* player, int bet, bool isFirst);                    // 处理玩家抢地主
    void onDisPosePlayHand(Player* player, Cards& cards);                         // 处理玩家的出牌

    void showAnimation(AnimationType type, int bet = 0); // 显示特效动画
    void hidePlayerDropCards(Player* player);

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
    CardPanel* m_baseCard;
    CardPanel* m_moveCard;
    QVector<CardPanel*> m_last3Card;
    QPoint m_baseCardPos;
    GameControl::GameStatus m_gameStatus;
    QTimer* m_timer;
    AnimationWindow* m_animation;

    // QWidget interface
protected:
    void paintEvent(QPaintEvent* event);
};
#endif // GAMEPANEL_H
