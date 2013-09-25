#include "run.h"

bool run::sort(run a, run b){
    return a.score > b.score;
}

void run::setup(string name){
    this->name = name;
    this->score = 0;
}

void run::setup(string name, int score){
    this->name = name;
    this->score = score;
}

int run::setScore(int score) {
    this->score = score;
}