#ifndef NEWPASSENGER_H
#define NEWPASSENGER_H

#include "Solution.hpp"
#include "Event.hpp"

class Solution::NewPassenger : public Event
{
    private:
        Passenger* passenger;
        Station* station;
    public:
        NewPassenger(Solution* sol, Passenger* passenger, Station* station, int time);
        virtual bool run();
};

#endif