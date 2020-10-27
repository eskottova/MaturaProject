#ifndef NEWTRAIN_H
#define NEWTRAIN_H

#include "Event.hpp"
#include "Solution.hpp"
#include "Train.hpp"

class Solution::NewTrain : public Event
{
    private:
        Train* train;
    public:
        NewTrain(Solution* sol, Train* train, int time);
        virtual bool run();
};

#endif