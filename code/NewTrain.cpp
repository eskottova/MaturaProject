#include "NewTrain.hpp"
#include "Solution.hpp"
#include "Line.hpp"

Solution::NewTrain::NewTrain(Solution* sol, Train* train, int time)
{
    this->sol = sol;
    this->train = train;
    this->time = time;
    this->priority = 4;
}

bool Solution::NewTrain::run()
{
    this->train->reset(this->sol);
    this->train->find_line(this->time);
    return false;
}
