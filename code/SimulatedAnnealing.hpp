#ifndef SIMULATEDANNEALING_H
#define SIMULATEDANNEALING_H

#include "Solution.hpp"
#include "Input.hpp"

class SimulatedAnnealing
{
    private:
        Solution* bests;
        int init_temp = 100;
        int it_per_temp = 20;
        int alpha = 10;
        Solution::Input* input;
    public:
        void optimize_solution(Solution::Input* input);
        Solution* get_best() {return bests;}
};


#endif