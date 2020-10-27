#ifndef EVENT_H
#define EVENT_H

#include "Solution.hpp"

class Solution::Event
{
    protected:
        Solution* sol;
        int time;
        int priority;
    public:
        virtual bool run();
        virtual int get_time() const {return this->time;}
        virtual int get_prio() const {return this->priority;}
        virtual ~Event(){};
        virtual void set_sol(Solution* sol);
};

#endif