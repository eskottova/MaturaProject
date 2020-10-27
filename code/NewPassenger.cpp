#include "NewPassenger.hpp"
#include "Event.hpp"
#include "Solution.hpp"
#include "Passenger.hpp"
#include "Station.hpp"

Solution::NewPassenger::NewPassenger(Solution* sol, Passenger* passenger, Station* station, int time)
{
    this->sol = sol;
    this->passenger = passenger;
    this->station = station;
    this->time = time;
    this->priority = 2;
}
 
bool Solution::NewPassenger::run()
{
    this->passenger->reset(this->sol);
    this->sol->last_action = this->time;
    return this->station->add_passenger(this->passenger);
}
