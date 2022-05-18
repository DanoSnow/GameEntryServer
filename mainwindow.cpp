#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QLabel>
#include <QPointer>
#include <algorithm>
#include <QTextStream>
#include <QMessageBox>

static GameEntry newPlayer, currentPlayer;
enum receive{newplayer, login};
static receive state;
static list<GameEntry>::iterator it;
static int ID;

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow){
    ui->setupUi(this);
    server = new QTcpServer(this);
    connect(server,SIGNAL(newConnection()),this,SLOT(ServerNewConnection()));
    server->listen(QHostAddress("127.0.0.1"),60);
    GameEntryDB = QSqlDatabase::addDatabase("QSQLITE");
    GameEntryDB.setDatabaseName("GameEntryDB.db");
    GameEntryDB.open();
    if(GameEntryDB.isOpen()){
        QSqlQuery qry;
        int i=0;
        QList<QPointer<QLabel>> nameList, scoreList, matchList;
        QPointer<QLabel> nameLabel = new QLabel(this), scoreLabel = new QLabel(this),
                matchLabel = new QLabel(this);
        qry.exec("select * from GameEntryDB");
        while(qry.next()){
            ui->tableWidget_2->insertRow(ui->tableWidget_2->rowCount());
            nameList.append(nameLabel);
            nameList[i] = new QLabel(this);
            nameList[i]->setText(qry.value(1).toString());
            currentPlayer.SetName(nameList[i]->text());
            ui->tableWidget_2->setCellWidget(i,0,nameList[i]);
            scoreList.append(scoreLabel);
            scoreList[i] = new QLabel(this);
            scoreList[i]->setText(qry.value(2).toString());
            currentPlayer.SetScore(scoreList[i]->text().toInt());
            ui->tableWidget_2->setCellWidget(i,1,scoreList[i]);
            matchList.append(matchLabel);
            matchList[i] = new QLabel(this);
            matchList[i]->setText(qry.value(3).toString());
            currentPlayer.SetTimesPlayed(matchList[i]->text().toInt());
            ui->tableWidget_2->setCellWidget(i,2,matchList[i]);
            currentPlayer.SetPassword(qry.value(4).toString());
            rak.push_back(currentPlayer);
            i++;
        }
        GameEntryDB.close();
    }else{
        QMessageBox::warning(this,"Warning","Couldn't open the data base");
    }
}

void MainWindow::GetData(const QString &D, GameEntry &G){
    QString str;
    int i=1;
    switch(state){
    case newplayer:
        while(D[i]!="!"){
            str+=D[i];
            i++;
        }
        G.SetName(str);
        str.clear();
        i++;
        while(D[i]!="!"){
            str+=D[i];
            i++;
        }
        G.SetScore(str.toInt());
        str.clear();
        i++;
        while(D[i]!="!"){
            str+=D[i];
            i++;
        }
        G.SetTimesPlayed(str.toInt());
        str.clear();
        i++;
        while(i<D.length()){
            str+=D[i];
            i++;
        }
        G.SetPassword(str);
        break;
    case login:
        while(D[i]!="!"){
            str+=D[i];
            i++;
        }
        G.SetName(str);
        str.clear();
        i++;
        while(i<D.length()){
            str+=D[i];
            i++;
        }
        G.SetPassword(str);
        break;
    }
}

void MainWindow::DBNewPlayer(const GameEntry &G){
    QString statement;
    QTextStream textstream(&statement);
    GameEntryDB.open();
    if(GameEntryDB.isOpen()){
        QSqlQuery qry;
        qry.exec("create table if not exists GameEntryDB (id integer primary key autoincrement,"
                                "name text,score integer,match integer,password text)");
        textstream <<"insert into GameEntryDB (name,score,match,password) values (\""
                   <<G.GetName()<<"\",\""<<QString::number(G.GetScore())<<"\",\""
                   <<QString::number(G.GetTimesPlayed())<<"\",\""<<G.GetPassword()<<"\");";
        if(!qry.exec(statement)){
            QMessageBox::warning(this,"Warning","Couldn't insert into data base");
        }
        GameEntryDB.close();
    }else{
        QMessageBox::warning(this,"Warning","Couldn't open the data base");
    }
}

void MainWindow::UpdateTable(){
    ui->tableWidget_2->clear();
    int count = 0;
    QList<QPointer<QLabel>> nameList, scoreList, matchList;
    QPointer<QLabel> nameLabel = new QLabel(this), scoreLabel = new QLabel(this),
            matchLabel = new QLabel(this);
    GameEntryDB.open();
    if(GameEntryDB.isOpen()){
        QSqlQuery qry;
        for(auto &elem:rak){
            QString statement, statement2;
            QTextStream textstream(&statement), textstream2(&statement2);
            nameList.append(nameLabel);
            nameList[count] = new QLabel(this);
            nameList[count]->setText(elem.GetName());
            ui->tableWidget_2->setCellWidget(count,0,nameList[count]);
            scoreList.append(scoreLabel);
            scoreList[count] = new QLabel(this);
            scoreList[count]->setText(QString::number(elem.GetScore()));
            ui->tableWidget_2->setCellWidget(count,1,scoreList[count]);
            textstream<<"update GameEntryDB set score = ""\""<<QString::number(elem.GetScore())
                      <<"\""" where id = \""<<QString::number(count+1)<<"\";";
            qry.exec(statement);
            matchList.append(matchLabel);
            matchList[count] = new QLabel(this);
            matchList[count]->setText(QString::number(elem.GetTimesPlayed()));
            ui->tableWidget_2->setCellWidget(count,2,matchList[count]);
            textstream2<<"update GameEntryDB set match = ""\""<<QString::number(elem.GetTimesPlayed())
                       <<"\""" where id = \""<<QString::number(count+1)<<"\";";
            qry.exec(statement2);
            count++;
        }
        GameEntryDB.close();
    }else{
        QMessageBox::warning(this,"Warning","Couldn't open the data base");
    }
}

void MainWindow::SendRanking(QTcpSocket *s, SortCriteria::criteria crit){
    if(!rak.empty()){
        multisetSort rank(crit);
        int count = 0;
        list<GameEntry>::iterator it = rak.begin();
        while(it!=rak.end() && count<5){
            rank.insert(*it);
            it++;
            count++;
        }
        QString str = "R";
        for(auto &elem:rank){
            str+=elem.GetName()+"!"+QString::number(elem.GetScore())+"!"+QString::number(elem.GetTimesPlayed())+"!";
        }
        QByteArray bytes = str.toLatin1();
        s->write(bytes);
    }else{
        QString err = "1";
        QByteArray byte = err.toLatin1();
        s->write(byte);
    }
}

MainWindow::~MainWindow(){
    delete ui;
}

void MainWindow::ServerNewConnection(){
    QTcpSocket *socket = server->nextPendingConnection();
    connect(socket,SIGNAL(readyRead()),this,SLOT(ServerReadyRead()));
    connect(socket,SIGNAL(disconnect()),this,SLOT(ServerDisconnect()));
    clientList.append(socket);
}

void MainWindow::ServerReadyRead(){
    QTcpSocket* socket = (QTcpSocket*)sender();
    qint64 bytesCount = socket->bytesAvailable();
    QByteArray bytes = socket->read(bytesCount);
    QString data = QString::fromLatin1(bytes);
    //-------------------------------------------------------------------
    //-------------------Check what data I'm asking for------------------
    //-------------------------------------------------------------------
    if(data[0]=="L"){
        if(rak.empty()){
            QString err = "1";
            QByteArray byte = err.toLatin1();
            socket->write(byte);
        }else{
            state = login;
            GetData(data,currentPlayer);
            it = find_if(rak.begin(),rak.end(),[=](const GameEntry &G){
                return G.GetName()==currentPlayer.GetName() && G.GetPassword()==currentPlayer.GetPassword();
            });
            if(it==rak.end()){
                QString err = "2";
                QByteArray byte = err.toLatin1();
                socket->write(byte);
            }else{
                QString A = "A";
                QByteArray byte = A.toLatin1();
                socket->write(byte);
            }
        }
    }
    if(data[0]=="n"){
        state = newplayer;
        GetData(data,newPlayer);
        rak.push_back(newPlayer);
        ui->tableWidget_2->insertRow(ui->tableWidget_2->rowCount());
        DBNewPlayer(newPlayer);
        UpdateTable();
    }
    if(data[0]=="S"){
        QString str;
        for(int i=1;i<data.length();i++){
            str+=data[i];
        }
        it->Play(str.toInt());
        UpdateTable();
    }
    if(data[0]=="R"){
        SendRanking(socket);
    }
    if(data[0]=="r"){
        SendRanking(socket,SortCriteria::match);
    }
}

void MainWindow::ServerDisconnect(){
    QTcpSocket *socket = (QTcpSocket*)sender();
    socket->close();
    socket->deleteLater();
    clientList.removeOne(socket);
}
