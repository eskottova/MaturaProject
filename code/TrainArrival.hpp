#ifndef TRAINARRIVAL_H
#define TRAINARRIVAL_H

#include "Event.hpp"
#include "Train.hpp"
#include "Station.hpp"
#include "Solution.hpp"

class Solution::TrainArrival : public Event
{
    private:
        Train* train;
        Station* station;
    public:
        TrainArrival(Solution* sol, int time, Train* train, Station* station);
        virtual bool run();
};

#endif