#include "NewStation.hpp"
#include "Station.hpp"
#include "Parameters.hpp"
#include "Solution.hpp"

Solution::NewStation::NewStation(Solution* sol, Station* station, int time)
{
    this->sol = sol;
    this->time = time;
    this->station = station;
    this->priority = 1;
}

bool Solution::NewStation::run()
{
    this->station->reset(this->sol);
    this->sol->new_stations.push_back(station);
    this->sol->active_stations.push_back(station);
    return false;
}
