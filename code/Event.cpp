#include "Event.hpp"
#include "Solution.hpp"
#include "Parameters.hpp"
#include <iostream>

bool Solution::Event::run()
{
    return false;
}

void Solution::Event::set_sol(Solution* sol)
{
    this->sol = sol;
}
