#ifndef INPUT_H
#define INPUT_H

#include <queue>
#include <vector>
#include "Solution.hpp"
#include "Event.hpp"

class Solution::Input
{
    private:
        Solution* sol;
    public:
        int event_n, change_time, train_passengers, station_passengers, station_n, passenger_n, line_n, train_n;
        
        std::vector<Solution::Station*> all_stations;
        std::vector<Solution::Passenger*> all_passengers;
        std::vector<Solution::Line*> all_lines;
        std::vector<Solution::Train*> all_trains;
        
        vector<Solution::Event*> input_events;
        Input(Solution* sol);
        void set_sol(Solution* sol);
};

#endif
