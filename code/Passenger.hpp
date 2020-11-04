#ifndef PASSENGER_H
#define PASSENGER_H

#include "Solution.hpp"

class Solution::Passenger
{
    private:
        Solution* sol;
        int time;
        int id;
        int start_station_id;
        int end_station_type;
        Train* train;
        Station* station;
        Line* line;
    public:
        Passenger(Solution* sol, int id, int time, int start_station_id, int end_station_type);
        int check_exit(Station* stat, bool change);
        bool check_boarding(Train* train);
        int get_id(){return this->id;}
        void reset(Solution* sol);  
};

#endif