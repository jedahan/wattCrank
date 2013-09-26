#pragma once

#include "ofMain.h"

class run {
    public:
        int score;
        string name;

        run(string name);
        run(string name, int score);

        static bool sort(run * a, run * b);
};