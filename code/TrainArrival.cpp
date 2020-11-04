#include "TrainArrival.hpp"

Solution::TrainArrival::TrainArrival(Solution* sol, int time, Train* train, Station* station)
{
    this->sol = sol;
    this->time = time;
    this->train = train;
    this->station = station;
    this->priority = 0;
}

bool Solution::TrainArrival::run()
{
    this->train->set_station(this->station);
    return this->train->unboard(this->station, this->time);
}
