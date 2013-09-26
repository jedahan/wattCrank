#include "run.h"

bool run::sort(run * a, run * b){
    return a->score > b->score;
}

run::run(string name){
    this->name = name;
    this->score = 0;
}

run::run(string name, int score){
    this->name = name;
    this->score = score;
}