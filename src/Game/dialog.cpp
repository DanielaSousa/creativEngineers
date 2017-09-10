#include "dialog.h"
#include "ui_dialog.h"
#include <QtGui>
#include <QGraphicsPixmapItem>
#include <QGraphicsView>

#include <stdio.h>
#include <unistd.h>

#include <string.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <sys/socket.h>

Dialog::Dialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog)
{
    ui->setupUi(this);
    game = false;
    found_start = 0;
    mThread = new MyThread(this);
    connect(mThread, SIGNAL(ImageChanged(int, QImage)), this,SLOT(onImageChanged(int, QImage)));
}

Dialog::~Dialog()
{
    delete ui;
}


void Dialog::onImageChanged(int Number, QImage img)
{
    if(!game)
    {
        if(Number == 1) ui->label->setText("Rock");
        else if (Number == 2){
            ui->label->setText("Paper");

            found_start++;
            printf("[DEBUG]found_start %d\n",found_start );
            if(found_start >= 7){
                //start game
                //update image to something like rules
                //alter r2 on robot --> Robot::posJ()
                //
                printf("\nRobot::posJ()\nR10 = 10\n" );
                send_to_robot(10, 10);

                music('s');
                //wait until ack
                usleep(9000000);
                printf("ACK received-> continue!\n");





                on_pushButton_3_clicked();

            }
        }
        else if (Number == 3) ui->label->setText("Scissors");
        else ui->label->setText("Not detected");
        updateGraphicsView(img);
    } else {
        game_window.updateResult(Number);
        game_window.updatePlayer(img);
        game_window.updateList(Number);
    }
}

void Dialog::on_pushButton_clicked()
{
    //Started
    mThread->Stop = false;
    mThread->start();
}

void Dialog::on_pushButton_2_clicked()
{
    //Stoped
    mThread->Stop = true;
}

void Dialog::on_pushButton_3_clicked()
{
    on_pushButton_clicked();
    game = true;
    mThread->game_window = true;

    game_window.show();
    game_window.on_Start_clicked();
}


void Dialog::updateGraphicsView(QImage img)
{
    //QImage img2 = this->getQImageFromIplImage(img1);
    //QImage img2("/home/ania/projekty/RockPaperScissor/image.png");

    QGraphicsScene* scene = new QGraphicsScene;
    ui->graphicsView->setScene(scene);
    scene->setBackgroundBrush(Qt::black);
    scene->addPixmap(QPixmap::fromImage(img));

    ui->graphicsView->show();
}

void Dialog::send_to_robot(int reg, int value){

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

    snprintf(buffer, sizeof(buffer), "http://192.168.0.1/karel/ComSet?sFc=2&sIndx=%d&sRealFlag=-1&sValue=%d", reg, value);

    n = sendto(sockfd,buffer,strlen(buffer) , 0 , (struct sockaddr *) &serv_addr, sizeof(serv_addr));
    if (n < 0)
         printf("ERROR writing to socket");

    shutdown(sockfd, SHUT_RDWR);
}

void Dialog::music(char t){
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

    buffer[0] = t;



    n = sendto(sockfd,buffer,strlen(buffer) , 0 , (struct sockaddr *) &serv_addr, sizeof(serv_addr));
    if (n < 0)
         printf("ERROR writing to socket");

    shutdown(sockfd, SHUT_RDWR);
}
