#include "buttongroup.h"
#include "ui_buttongroup.h"

ButtonGroup::ButtonGroup(QWidget* parent) : QWidget(parent), ui(new Ui::ButtonGroup) { ui->setupUi(this); }

ButtonGroup::~ButtonGroup() { delete ui; }

void ButtonGroup::initButtons() {
    ui->startBtn->setImage(":/images/start-1.png", ":/images/start-3.png", ":/images/start-2.png");
    ui->playCardBtn->setImage(":/images/chupai_btn-1.png", ":/images/chupai_btn-3.png", ":/images/chupai_btn-2.png");
    ui->playCardBtn1->setImage(":/images/chupai_btn-1.png", ":/images/chupai_btn-3.png", ":/images/chupai_btn-2.png");
    ui->passBtn->setImage(":/images/pass_btn-1.png", ":/images/pass_btn-3.png", ":/images/pass_btn-2.png");
    ui->giveupBtn->setImage(":/images/buqiang-1.png", ":/images/buqiang-3.png", ":/images/buqiang-2.png");
    ui->oneScoreBtn->setImage(":/images/1fen-1.png", ":/images/1fen-3.png", ":/images/1fen-2.png");
    ui->twoScoreBtn->setImage(":/images/2fen-1.png", ":/images/2fen-3.png", ":/images/2fen-2.png");
    ui->threeScoreBtn->setImage(":/images/3fen-1.png", ":/images/3fen-3.png", ":/images/3fen-2.png");
    // 设置按钮大小
    QVector<MyButton*> btns;
    btns << ui->startBtn << ui->playCardBtn << ui->playCardBtn1 << ui->passBtn << ui->giveupBtn << ui->oneScoreBtn << ui->twoScoreBtn << ui->threeScoreBtn;
    for (MyButton* btn : btns) {
        btn->setFixedSize(90, 45);
    }

    connect(ui->startBtn, &MyButton::clicked, this, &ButtonGroup::startGame);
    connect(ui->playCardBtn, &MyButton::clicked, this, &ButtonGroup::playHand);
    connect(ui->playCardBtn1, &MyButton::clicked, this, &ButtonGroup::playHand);
    connect(ui->passBtn, &MyButton::clicked, this, &ButtonGroup::pass);
    connect(ui->giveupBtn, &MyButton::clicked, this, [=]() { emit betPoint(0); });
    connect(ui->oneScoreBtn, &MyButton::clicked, this, [=]() { emit betPoint(1); });
    connect(ui->twoScoreBtn, &MyButton::clicked, this, [=]() { emit betPoint(2); });
    connect(ui->threeScoreBtn, &MyButton::clicked, this, [=]() { emit betPoint(3); });
}

void ButtonGroup::selectPage(Panel type, int bet) {
    ui->stackedWidget->setCurrentIndex(type);
    if (type != CallLord)
        return;
    // 叫地主玩家可选分数按钮
    if (bet == 0) {
        ui->oneScoreBtn->setVisible(true);
        ui->twoScoreBtn->setVisible(true);
        ui->threeScoreBtn->setVisible(true);
    } else if (bet == 1) {
        ui->twoScoreBtn->setVisible(true);
        ui->threeScoreBtn->setVisible(true);
    } else if (bet == 2) {
        ui->threeScoreBtn->setVisible(true);
    }
}
