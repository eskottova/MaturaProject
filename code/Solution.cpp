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
    this->input = new Input(this);
}

Solution::Solution(Input* input, Parameters* para) : events(compare)
{
    std::cerr << "new solution\n";
    this->para = para;
    this->input = input;
    this->input->set_sol(this);
    for(auto e : this->input->input_events) this->events.push(e);

    while(!this->overfilled && !this->events.empty() && this->time < this->last_pass + 1000)
    {
        Solution::Event* e = this->events.top();
        this->events.pop();
        this->time = e->get_time();
        this->overfilled = e->run();
        if(e->get_prio() == 5 || e->get_prio() == 0) delete e;
    }
    this->val = this->pass_finished * 1000 - this->last_pass;
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

void Solution::print_result()
{
    cout << this->pass_finished << " " << this->last_pass << '\n';
}

void Solution::print_pars()
{
    for(int p : this->para->pars) cout << p << " ";
    cout << "\n";
}
