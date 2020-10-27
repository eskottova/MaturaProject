#include <iostream>
#include <vector>
#include "Solution.hpp"
#include "Event.hpp"
#include "Input.hpp"
#include "OutEvent.hpp"
#include "Station.hpp"
#include "random_generator.hpp"

Solution::Solution() : events(compare)
{
    this->para = new Parameters();
    this->para->pars = {-1, -1};
    this->input = new Input(this);
}

Solution::Solution(Input* input, Parameters* para) : events(compare)
{
    std::vector<Solution::Station*> new_stations;
    std::vector<Solution::Station*> stations;
    this->para = para;
    this->input = input;
    this->input->set_sol(this);
    for(auto e : this->input->input_events) this->events.push(e);
    this->last_action = 0;
    this->overfilled = false;
    this->steps = 0;
    while(!this->overfilled && !this->events.empty() && this->steps < this->last_action + 2000)
    {
        Solution::Event* e = this->events.top();
        this->events.pop();
        this->steps = e->get_time();
        this->overfilled = e->run();
    }

    this->pass_finished = 0;
    for(auto oe : this->out_events) if(oe->get_type() == 'a') this->pass_finished ++;

    this->val = this->pass_finished * 10000 - this->steps;
}

Parameters* Solution::neighbor()
{
    Parameters* neighbor = new Parameters;
    neighbor->pars = this->para->pars;
    int changep = randint(0, neighbor->pars.size());
    neighbor->random_par(changep);
    return neighbor;
}

void Solution::print()
{
    for(auto oe : this->out_events)
    {
        oe->print();
    }
}