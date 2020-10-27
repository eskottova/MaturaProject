#include "Passenger.hpp"
#include "Station.hpp"
#include "Parameters.hpp"
#include "Solution.hpp"
#include "Input.hpp"

Solution::Passenger::Passenger(Solution* sol, int id, int time, int start_station_id, int end_station_type)
{
    this->sol = sol;
    this->id = id;
    this->time = time;
    this->start_station_id = start_station_id;
    this->end_station_type = end_station_type;
    this->station = this->sol->all_stations[start_station_id];
}

void Solution::Passenger::reset(Solution* sol)
{
    this->sol = sol;
    this->station = this->sol->input->all_stations[this->start_station_id];
    this->train = nullptr;
    this->line = nullptr;
}


bool Solution::Passenger::check_exit(Station* stat)
{
    if(stat->type == end_station_type) 
    {
        train = nullptr;
        station = stat;
        return true;
    }
    return false;
}

bool Solution::Passenger::check_boarding(Train* tra)
{
    if((tra->line_contains(this->end_station_type) && tra->get_cap() > 0) && this->sol->para->pars[0] * this->station->get_cap() < this->sol->para->pars[1] * tra->get_cap())
    {
        this->train = tra;
        this->station = nullptr;
        return true;
    }
    else return false;
}