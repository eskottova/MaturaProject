#include "Passenger.hpp"
#include "Station.hpp"
#include "Parameters.hpp"
#include "Solution.hpp"
#include "Input.hpp"
#include <algorithm>

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


int Solution::Passenger::check_exit(Station* stat, bool change)
{
    vector<int>& par = this->sol->para->pars;
    int val = par[4] * stat->get_cap() + par[5] * this->train->get_cap() + this->train->line_contains(this->end_station_type) * par[6] + (stat->num_lines_type(this->end_station_type) > this->train->line_contains(this->end_station_type)) * par[7];
    if(stat->type == end_station_type || ((val > 0) && stat->get_cap() > 0)) 
    {
        train = nullptr;
        station = stat;
        return 1 + (stat->type == this->end_station_type);
    }
    return 0;
}

bool Solution::Passenger::check_boarding(Train* tra)
{
    vector<int>& par = this->sol->para->pars;
    int val = par[0] * this->station->get_cap() + par[1] * tra->get_cap() + tra->line_contains(this->end_station_type) * par[2]+ (this->station->num_lines_type(this->end_station_type) > tra->line_contains(this->end_station_type)) * par[3];
    if(tra->get_cap() > 0 && val > 0)
    {
        this->train = tra;
        this->station = nullptr;
        return true;
    }
    else return false;
}