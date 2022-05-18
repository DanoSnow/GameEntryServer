#ifndef GAMEENTRY_H
#define GAMEENTRY_H

#include <QObject>
#include <set>
using namespace std;

class GameEntry{
private:
    QString name;        //Player's name
    int score;          //Player's Score
    int timesPlayed;    //Number of times that the player played
    QString password;    //Player's password
public:
    GameEntry(QString="", int=0, QString="");
    GameEntry(const GameEntry &);
    GameEntry &operator=(const GameEntry &);
    bool operator<(const GameEntry &)const;
    bool operator>(const GameEntry &)const;
    bool operator==(const GameEntry &)const;
    QString GetName() const {return name;}
    void SetName(QString n) {name = n;}
    int GetScore() const {return score;}
    void SetScore(int s) {score = s;}
    int GetTimesPlayed() const {return timesPlayed;}
    void SetTimesPlayed(int t) {timesPlayed = t;}
    QString GetPassword() const {return password;}
    void SetPassword(QString p) {password = p;}
    void Print() const;
    void Play(int);
};

class SortCriteria{
public:
    enum criteria {score, match};
private:
    criteria crit;
public:
    SortCriteria(criteria c = score) : crit(c){}
    bool operator() (const GameEntry &G1, const GameEntry &G2)const{
        if(crit == score){
            return G1.GetScore()>G2.GetScore();
        }else{
            return G1.GetTimesPlayed()>G2.GetTimesPlayed();
        }
    }
};
typedef multiset<GameEntry,SortCriteria> multisetSort;

#endif // GAMEENTRY_H
