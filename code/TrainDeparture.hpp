#ifndef TRAINDEPARTURE_H
#define TRAINDEPARTURE_H

#include "Event.hpp"
#include "Train.hpp"
#include "Station.hpp"

class Solution::TrainDeparture : public Event
{
    private:
        Train* train;
        Station* station;
        bool newtrain;
    public:
        TrainDeparture(Solution* sol, int time, bool newtrain, Train* train, Station* station);
        virtual bool run();
};


#endif