#include "Station.hpp"
#include "Parameters.hpp"
#include "Solution.hpp"
#include "Input.hpp"

Solution::Station::Station(Solution* sol, int id, int time, int type, int x, int y)
{
    this->sol = sol;
    this->id = id;
    this->time = time;
    this->type = type;
    this->x = x;
    this->y = y;
}

void Solution::Station::reset(Solution* sol)
{
    this->sol = sol;
    this->lines.clear();
    this->passengers.clear();
}
        
void Solution::Station::board(Train* train, int time)
{
    for(auto it = this->passengers.begin(); it != this->passengers.end(); it ++)
    {
        if((*it)->check_boarding(train))
        {
            train->add_pass(*it);
            this->sol->out_events.push_back(new Solution::OutEvent(time, (*it)->get_id(), 'e', {train->get_id()}));
            it = passengers.erase(it) - 1;
            this->sol->last_action = time;
        }
    }
}

bool Solution::Station::add_passenger(Passenger* passenger)
{
    this->passengers.push_back(passenger);
    return this->passengers.size() > this->sol->input->station_passengers;
}

int Solution::Station::add_line(Line* line)
{
    this->lines.push_back(line);
    return this->id;
}

int Solution::Station::get_cap()
{
    return this->sol->input->station_passengers - this->passengers.size();
}