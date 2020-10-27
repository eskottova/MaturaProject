#ifndef NEWSTATION_H
#define NEWSTATION_H

#include "Event.hpp"
#include "Solution.hpp"

class Solution::NewStation : public Event
{
    private:
        Station* station;
    public:
        NewStation(Solution* sol, Station* station, int time);
        virtual bool run();
};


#endif