#ifndef TRAIN_H
#define TRAIN_H

#include <vector>
#include "Passenger.hpp"
#include "Line.hpp"
#include "OutEvent.hpp"
#include "Station.hpp"
#include "Solution.hpp"

class Solution::Train
{
    private:
        Solution* sol;
        int id;
        int time;
        vector<Solution::Passenger*> passengers;
        Solution::Line* line;
        Solution::Station* station;
    public:
        Train(Solution* sol, int id, int time);
        void unboard(Station* station, int time);
        int get_cap();
        void set_line(Line* line, Station* station, int time);
        void set_station(Station* station);
        std::pair<int, Station*> find_next_station();
        int get_id(){return this->id;}
        void add_pass(Passenger* p);
        bool line_contains(int station_type);
        void reset(Solution* sol);
};
#endif