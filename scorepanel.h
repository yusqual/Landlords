#ifndef SCOREPANEL_H
#define SCOREPANEL_H

#include <QWidget>

namespace Ui {
class ScorePanel;
}

class ScorePanel : public QWidget {
    Q_OBJECT

public:
    explicit ScorePanel(QWidget* parent = nullptr);
    ~ScorePanel();

    void setScores(int left, int right, int user);

private:
    Ui::ScorePanel* ui;
};

#endif // SCOREPANEL_H
