#include "TrainDeparture.hpp"
#include "Train.hpp"
#include "Station.hpp"
#include "Solution.hpp"
#include "TrainArrival.hpp"

Solution::TrainDeparture::TrainDeparture(Solution* sol, int time, bool newtrain, Train* train, Station* station)
{
    this->sol = sol;
    this->time = time;
    this->train = train;
    this->station = station;
    this->priority = 5;
    this->newtrain = newtrain;
}

bool Solution::TrainDeparture::run()
{
    if(!this->newtrain) this->station->board(this->train, this->time);

    std::pair<int, Station*> next_info = this->train->find_next_station();
    
    this->sol->events.push(new Solution::TrainArrival(this->sol, next_info.first + 1 + this->time, train, next_info.second));
    this->sol->events.push(new Solution::TrainDeparture(this->sol, next_info.first + 1 + this->time, false, train, next_info.second));
    return false;
}
