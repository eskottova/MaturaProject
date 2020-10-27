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
    this->sol->last_action = this->time;
    if(this->sol->last_line == nullptr) this->sol->new_trains.push_back(this->train);
    else train->set_line(this->sol->last_line, this->sol->last_line->get_start(), this->time);
    return false;
}
