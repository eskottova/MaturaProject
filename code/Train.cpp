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
    this->change = false;
}

bool Solution::Train::unboard(Solution::Station* station, int time)
{
    for(auto it = passengers.begin(); it != passengers.end(); it ++)
    {
        int exit_state = (*it)->check_exit(station, this->change);
        if(exit_state > 0)
        {
            this->sol->out_events.push_back(new Solution::OutEvent(time, (*it)->get_id(), 'a', {station->get_id()}));
            if(exit_state == 1) station->add_passenger((*it));
            else
            {
                this->sol->pass_finished ++;
                this->sol->last_pass = time;
            }
            it = passengers.erase(it) - 1;
        }
    }
    if(change && this->passengers.size() == 0)
    {
        this->find_line(time);
        this->change = false;
    }
    return (station->get_cap() < 0);
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

void Solution::Train::find_line(int time)
{
    Line* ans_line;
    int ans_val = 0;
    for(auto line : this->sol->active_lines)
    {
        if(line->get_val() > ans_val)
        {
            ans_val = line->get_val();
            ans_line = line;
        }
    }
    this->set_line(ans_line, ans_line->get_start(), time);
    ans_line->add_train(this);
}

/* void Solution::Train::check_change()
{
    this->change = this->line->get_change();
}*/