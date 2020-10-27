#ifndef SOLUTION_H
#define SOLUTION_H

#include <vector>
#include <queue>
#include <iostream>
#include "Parameters.hpp"

using std::vector;

auto compare = [](auto lhs, auto rhs)
{
    if(lhs->get_time() == rhs->get_time()) return lhs->get_prio() > rhs->get_prio();
    else return lhs->get_time() > rhs->get_time();
};

class Solution
{   
    public:
        class Input;
        int val;
        Parameters* neighbor();
        Input* get_input(){return this->input;}
        Parameters* get_pars(){return this->para;}
        void set_pars(Parameters* para){this->para = para;}
        Solution(Input* input, Parameters* para);
        Solution();
        void print();
        bool operator<(Solution const& s) {return this->val < s.val;}
        int last_action;
    private:
        class Station;
        class Passenger;
        class Line;
        class Train;
        
        class Event;
        class TrainArrival;
        class NewStation;
        class NewPassenger;
        class NewLine;
        class NewTrain;
        class TrainDeparture;

        class OutEvent;

        int pass_finished = 0;
        int steps = 0;
        bool overfilled;
        Input* input;
        Line* last_line;
        Parameters* para;
        vector<Solution::Station*> new_stations;
        vector<Solution::Train*> new_trains;
        std::priority_queue<Solution::Event*, vector<Solution::Event*>, decltype(compare)> events;
        vector<Solution::OutEvent*> out_events;
        vector<Solution::Station*> stations;
        vector<Solution::Station*> all_stations;
};

#endif 