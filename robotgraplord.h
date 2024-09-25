#ifndef ROBOTGRAPLORD_H
#define ROBOTGRAPLORD_H

#include "player.h"

#include <QThread>

class RobotGrapLord : public QThread {
    Q_OBJECT
public:
    explicit RobotGrapLord(Player* player, QObject* parent = nullptr);

signals:

    // QThread interface
protected:
    void run() override;

private:
    Player* m_player;
};

#endif // ROBOTGRAPLORD_H
