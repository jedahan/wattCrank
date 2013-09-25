#pragma once

#include "ofMain.h"

class run {
    public:
        int score;
        string name;

        void setup(string name);
        void setup(string name, int score);
        int setScore(int score);

        static bool sort(run a, run b);
};