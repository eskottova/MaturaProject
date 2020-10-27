#ifndef NEWLINE_H
#define NEWLINE_H

#include "Event.hpp"
#include "Line.hpp"
#include "Solution.hpp"

class Solution::NewLine : public Event
{
    private:
        Line* line;
    public:
        NewLine(Solution* sol, Line* line, int time);
        virtual bool run();
};

#endif