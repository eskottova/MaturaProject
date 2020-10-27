#ifndef OUTEVENT_H
#define OUTEVENT_H

#include <iostream>
#include "Solution.hpp"
using std::cout;

class Solution::OutEvent
{
    private:
        Solution* sol;
        int t, id;
        char type;
        vector<int> other;
    public:
        OutEvent(int t, int id, char type, vector<int> other);
        char get_type(){return this->type;}
        void print();
};

#endif