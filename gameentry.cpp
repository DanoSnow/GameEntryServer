#include "gameentry.h"

GameEntry::GameEntry(QString n, int s, QString p) : name(n), score(s), password(p){timesPlayed = 0;}

GameEntry::GameEntry(const GameEntry &G){
    name = G.GetName();
    score = G.GetScore();
    timesPlayed = G.GetTimesPlayed();
    password = G.GetPassword();
}

GameEntry &GameEntry::operator=(const GameEntry &G){
    name = G.GetName();
    score = G.GetScore();
    timesPlayed = G.GetTimesPlayed();
    password = G.GetPassword();
    return *this;
}

bool GameEntry::operator<(const GameEntry &G)const{
    if(score<G.score){
        return true;
    }else{
        return false;
    }
}

bool GameEntry::operator>(const GameEntry &G)const{
    if(score>G.score){
        return true;
    }else{
        return false;
    }
}

bool GameEntry::operator==(const GameEntry &G) const
{
    if(name==G.name && password==G.password){
        return true;
    }else{
        return false;
    }
}

void GameEntry::Play(int s){
    if(s>score){
        score = s;
    }
    timesPlayed++;
}
