#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTcpServer>
#include <QTcpSocket>
#include "exceptions.h"
#include "gameentry.h"
#include <list>
#include <QSqlDatabase>
#include <QSqlQuery>
using namespace std;

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    void GetData(const QString &, GameEntry &);
    void DBNewPlayer(const GameEntry &);
    void UpdateTable();
    void SendRanking(QTcpSocket *, SortCriteria::criteria = SortCriteria::score);
    ~MainWindow();

public slots:
    void ServerNewConnection();
    void ServerReadyRead();
    void ServerDisconnect();

private:
    Ui::MainWindow *ui;
    QList<QTcpSocket *> clientList;
    QTcpServer *server;
    list<GameEntry> rak;
    QSqlDatabase GameEntryDB;
};

#endif // MAINWINDOW_H
