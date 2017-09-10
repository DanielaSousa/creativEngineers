#include "game.h"
#include "ui_game.h"
#include "artificialintelligence.h"
#include <QtGui>
#include <QGraphicsPixmapItem>
#include <QGraphicsView>

#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <sys/socket.h>
Game::Game(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Game)
{
    ui->setupUi(this);
    img3.load("Images/3.jpg");
    img2.load("Images/2.jpg");
    img1.load("Images/1.jpg");

    rock.load("Images/Rock.jpg");
    paper.load("Images/Paper.jpg");
    scissors.load("Images/Scissors.jpg");

    victory.load("Images/Ganhar.jpg");
    lose.load("Images/Perder.jpg");
    draw.load("Images/Empatar.jpg");

    counter = 0;
    size = 5;
    computer_score = 0;
    player_score = 0;
    com_sign = 0;
    counter_valid_move = 0;
    AI = new ArtificialIntelligence();
    gThread = new GameThread(this);
    connect(gThread, SIGNAL(FieldChanged(int)), this, SLOT(onFieldChanged(int)));
}

Game::~Game()
{
    delete ui;
}

void Game::onFieldChanged(int Number)
{
    QGraphicsScene* scene = new QGraphicsScene;
    ui->computer->setScene(scene);
    scene->setBackgroundBrush(Qt::black);
    int sleep = 0;
    QImage img;
    if(Number == 0)
    {
        //move
        printf("\nRobot::move()\nR10 = 20\n" );
        send_to_robot(10,20);
        printf("ACK (R10) = 0\n");

        img = img3;
        ui->result->setText("");
        
    }
    else if(Number == 1) {
        img = img2;
        
    }
    else if(Number == 2){
        img = img1;
        
    } 
    else
    {
        int num = checkPlayer();
        //send_to_robot(11, 40);
        img = chooseSign(num); //send to arduino

         //delay to do the robotic position
        usleep(1000000);          
        updateScore(num);
        updateResult(num);
    }

    scene->addPixmap(QPixmap::fromImage(img));

    ui->computer->show();

    ui->score_computer->setText(QString::number(computer_score));
    ui->score_player->setText(QString::number(player_score));
    
}

void Game::on_Start_clicked()
{
    gThread->Stop = false;
    gThread->start();
}

void Game::on_Stop_clicked()
{
    QGraphicsScene* scene = new QGraphicsScene;
    ui->computer->setScene(scene);

    gThread->Stop = true;
    //Evaluate
    if(computer_score < player_score){
        printf("YOU WON!\n");
        music(2);
        send_to_robot(10, 30);

        scene->addPixmap(QPixmap::fromImage(victory));
        ui->computer->show();

        /*
        //controlo da garra
        //wait until go to apple
        usleep(1000); //10s while pic up apple
        //close claw
        send_to_arduino(1);
        //wait until go to dropping place
        usleep(1000); //10s while pic up apple
        //open claw
        send_to_arduino(4);   */        
    }
    else if(computer_score == player_score){
        printf("draw!\n");
        send_to_robot(10, 10);

        scene->addPixmap(QPixmap::fromImage(draw));
        ui->computer->show();

        
        
    }
    else{
        printf("lose!\n");
        send_to_robot(10, 40);

        scene->addPixmap(QPixmap::fromImage(victory));
        ui->computer->show();

        music(3);
        
    }
    
}


QImage Game::chooseSign(int player_sign)
{
    QImage img;
    com_sign = AI->play(player_sign);
    if(com_sign == 1) img = rock;
    else if(com_sign == 2) img = paper;
    else img = scissors;

    //send to arduino
    send_to_arduino(com_sign);

    return img;
}

void Game::updatePlayer(QImage img)
{
    QGraphicsScene* scene = new QGraphicsScene;
    ui->player->setScene(scene);
    scene->setBackgroundBrush(Qt::black);
    scene->addPixmap(QPixmap::fromImage(img));

    ui->player->show();
}

void Game::updateResult(int Number)
{
    if(Number == 2) ui->result->setText("Paper");
    else if (Number == 3) ui->result->setText("Scissors");
    else if (Number == 1) ui->result->setText("Rock");
    else ui->result->setText("Not detected");
}

void Game::updateList(int i)
{
    list[counter] = i;
    counter = (counter + 1) % size;
}

int Game::checkPlayer()
{
    int c_paper = 0, c_scissors = 0, c_rock = 0, c_empty = 0;
    for(int i= 0; i< size; i++)
    {
        if (list[i] == 1) c_rock++;
        else if (list[i] == 2) c_paper++;
        else if (list[i] == 3) c_scissors++;
        else c_empty++;
    }
    int temp = max_c(c_paper, c_rock, c_scissors, c_empty);
    if(temp == c_rock) return 1;
    if(temp == c_paper) return 2;
    if(temp == c_scissors) return 3;
    return 0;
}

int Game::max_c(int a, int b, int c, int d)
{
    bool max_a = false, max_c = false;
    if(a > b) max_a = true;
    if(c > d) max_c = true;
    if(max_a)
    {
        if(max_c)
        {
            if(a > c) return a;
            else return c;
        }
        else
        {
            if(a > d) return a;
            else return d;
        }
    }
    else
    {
        if(max_c)
        {
            if(b > c) return b;
            else return c;
        }
        else
        {
            if(b > d) return b;
            else return d;
        }
    }
}

void Game::updateScore(int my_sign)
{
    if(com_sign == 1) //Kamień
    {
        if(my_sign == 2)  player_score++;//Papier
        else if(my_sign == 3) computer_score++; //Nożyczki
    }
    else if(com_sign == 2) //Papier
    {
        if(my_sign == 1) computer_score++; //Kamień
        else if(my_sign == 3) player_score++; //Nożyczki
    }
    else if(com_sign == 3) //Nożyczki
    {
        if(my_sign == 2) computer_score++; //Papier
        else if(my_sign == 1) player_score++; //Kamień
    }
}

void Game::send_to_robot(int reg, int value){
    
    /*std::ostringstream os;
    os << curlpp::options::Url(std::string(
        "http://192.168.0.1/karel/ComSet?sFc=2&sIndx=12&sRealFlag=-1&sValue=300"        ));
    */
    int sockfd, portno, n;
    struct sockaddr_in serv_addr;
    struct hostent *server;

    
   
    portno = 5005;
    sockfd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (sockfd < 0) 
        printf("ERROR opening socket");
    memset((char *) &serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(portno);
    if (inet_aton("127.0.0.1" , &serv_addr.sin_addr) == 0) 
    {
        fprintf(stderr, "inet_aton() failed\n");
        exit(1);
    }

    
    char buffer[512]; // "http://192.168.0.1/karel/ComSet?sFc=2&sIndx=12&sRealFlag=-1&sValue=10";
    char message[2];
    
    if(reg ==11){
        snprintf(buffer, sizeof(buffer),  "http://192.168.0.1/MD/NUMREG.VA");
    }
    else{
        snprintf(buffer, sizeof(buffer), "http://192.168.0.1/karel/ComSet?sFc=2&sIndx=%d&sRealFlag=-1&sValue=%d", reg, value);
    }
    n = sendto(sockfd,buffer,strlen(buffer) , 0 , (struct sockaddr *) &serv_addr, sizeof(serv_addr));
    if (n < 0) 
         printf("ERROR writing to socket");
   
   /*if(value==40 && reg ==11){
        socklen_t sendsize = sizeof(serv_addr);
        recvfrom(sockfd, message, sizeof(message), 0, (struct sockaddr*)&serv_addr, (&sendsize));

    }*/

    shutdown(sockfd, SHUT_RDWR);
}

void Game::send_to_arduino(int s){

    int sockfd, portno, n;
    struct sockaddr_in serv_addr;
    struct hostent *server;

    
   
    portno = 5005;
    sockfd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (sockfd < 0) 
        printf("ERROR opening socket");
    memset((char *) &serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(portno);
    if (inet_aton("127.0.0.1" , &serv_addr.sin_addr) == 0) 
    {
        fprintf(stderr, "inet_aton() failed\n");
        exit(1);
    }

    
    char buffer[512]; 
    
    if(s == 1) buffer[0] = 'r';
    else if(s== 2) buffer[0] = 'p';
    else if(s==3) buffer[0] = 's';
    else buffer[0]= 'n';
    
    
    n = sendto(sockfd,buffer,strlen(buffer) , 0 , (struct sockaddr *) &serv_addr, sizeof(serv_addr));
    if (n < 0) 
         printf("ERROR writing to socket");

    shutdown(sockfd, SHUT_RDWR);
}

void Game::music(int t){
    //send to music udp server
    int sockfd, portno, n;
    struct sockaddr_in serv_addr;
    struct hostent *server;

    
   
    portno = 5009;
    sockfd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (sockfd < 0) 
        printf("ERROR opening socket");
    memset((char *) &serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(portno);
    if (inet_aton("127.0.0.1" , &serv_addr.sin_addr) == 0) 
    {
        fprintf(stderr, "inet_aton() failed\n");
        exit(1);
    }

    
      char buffer[1]; 
    
    if(t== 1) buffer[0] = 's';
    if (t==2) buffer[0] = 'v';
    else buffer[0] = 'l';
    
    
    
    n = sendto(sockfd,buffer,strlen(buffer) , 0 , (struct sockaddr *) &serv_addr, sizeof(serv_addr));
    if (n < 0) 
         printf("ERROR writing to socket");

    shutdown(sockfd, SHUT_RDWR);
}

