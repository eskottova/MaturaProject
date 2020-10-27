#include "Train.hpp"
#include "Parameters.hpp"
#include "OutEvent.hpp"
#include "Station.hpp"
#include "TrainDeparture.hpp"
#include "Solution.hpp"
#include "Input.hpp"

Solution::Train::Train(Solution* sol, int id, int time)
{
    this->sol = sol;
    this->id = id;
    this->time = time;
}

void Solution::Train::reset(Solution* sol)
{
    this->sol = sol;
    this->passengers.clear();
    this->line = nullptr;
    this->station = nullptr;
}

void Solution::Train::unboard(Solution::Station* station, int time)
{
    for(auto it = passengers.begin(); it != passengers.end(); it ++)
    {
        if((*it)->check_exit(station))
        {
            this->sol->out_events.push_back(new Solution::OutEvent(time, (*it)->get_id(), 'a', {station->get_id()}));
            it = passengers.erase(it) - 1;
            this->sol->last_action = time;
        }
    }
}

void Solution::Train::add_pass(Passenger* p)
{
    this->passengers.push_back(p);
}

int Solution::Train::get_cap()
{
    return this->sol->input->train_passengers - this->passengers.size();
}

void Solution::Train::set_line(Line* line, Station* station, int time)
{
    this->line = line;
    this->station = station;
    this->sol->events.push(new Solution::TrainDeparture(this->sol, time, true, this, station));
    this->sol->out_events.push_back(new Solution::OutEvent(time, this->id, 'z', {this->line->get_id(), 0}));
}

void Solution::Train::set_station(Station* station)
{
    this->station = station;
}

std::pair<int, Solution::Station*> Solution::Train::find_next_station()
{
    return this->line->next_station(this->station);
}

bool Solution::Train::line_contains(int station_type)
{
    return this->line->contains(station_type);
}