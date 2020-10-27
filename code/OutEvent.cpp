#include "OutEvent.hpp"

#include <iostream>

using std::cout;

Solution::OutEvent::OutEvent(int t, int id, char type, vector<int> other)
{
    this->t = t;
    this->id = id;
    this->type = type;
    this->other = other;
}

void Solution::OutEvent::print()
{
    cout << this->t << " " << this->type << " " << this->id;
    for(int el : this->other)
    {
        cout << " " << el;
    }
    cout << '\n';
}
