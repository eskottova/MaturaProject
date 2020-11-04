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
        void print_pars();
        void print_result();
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

        int last_change = 0;
        int pass_finished = 0;
        int time = 0;
        int last_pass = 0;
        bool overfilled = false;
        Input* input;
        Parameters* para;
        vector<Solution::Station*> new_stations;
        std::priority_queue<Solution::Event*, vector<Solution::Event*>, decltype(compare)> events;
        vector<Solution::OutEvent*> out_events;
        vector<Solution::Station*> all_stations;
        vector<Solution::Line*> active_lines;
        vector<Solution::Station*> active_stations;
};

#endif 