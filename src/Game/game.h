#ifndef GAME_H
#define GAME_H

#include <QDialog>
#include "gamethread.h"
#include "artificialintelligence.h"

namespace Ui {
class Game;
}

class Game : public QDialog
{
    Q_OBJECT

public:
    explicit Game(QWidget *parent = 0);
    ~Game();
    GameThread *gThread;
    ArtificialIntelligence *AI;
    int list[5];
    int counter;
    int player_score, computer_score;
    int counter_valid_move;
    void updatePlayer(QImage img);
    void updateList(int i);
    void updateResult(int);
public slots:
    void onFieldChanged(int);
    QImage chooseSign(int);
void on_Start_clicked();
private slots:
    void send_to_robot(int reg, int value);
    void send_to_arduino(int s);
    void music(int t);
    void on_Stop_clicked();

private:
    Ui::Game *ui;
    QImage img3, img2, img1, rock, paper, scissors, victory, lose, draw;
    int size, com_sign;
    
    void updateScore(int);
    int checkPlayer();
    int max_c(int a, int b, int c, int d);
};

#endif // GAME_H
