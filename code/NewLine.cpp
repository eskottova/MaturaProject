#include "NewLine.hpp"
#include "Solution.hpp"
#include "Line.hpp"

Solution::NewLine::NewLine(Solution* sol, Line* line, int time)
{
    this->sol = sol;
    this->line = line;
    this->time = time;
    this->priority = 3;
}

bool Solution::NewLine::run()
{
    this->line->reset(this->sol);
    this->sol->last_action = this->time;
    vector<Station*> lstations;
    if(this->sol->new_stations.size() == this->sol->stations.size()) lstations = this->sol->new_stations;
    else
    {
        for(int i = 0; i < this->sol->new_stations.size(); i ++) // adds the new stations as well as the same number of already added stations
        {
            lstations.push_back(this->sol->new_stations[i]);
            int ind = (int)this->sol->stations.size() - 2 * (int)this->sol->new_stations.size() + i;
            if(ind >= 0 && ind < this->sol->stations.size()) lstations.push_back(this->sol->stations[this->sol->stations.size() - 2 * this->sol->new_stations.size() + i]); 
        }
    }
    
    if(lstations.size() > 2)
    {
        this->line->set_stations(lstations, this->time);
        this->sol->new_stations.clear();

        if(this->sol->last_line == nullptr) 
        {
            this->line->add_trains(this->time);
            this->sol->new_trains.clear();
        }
        this->sol->last_line = this->line;
    }
    else
    {
        this->sol->events.push(new NewLine(this->sol, this->line, this->time + 40));
    }

    return false;
}
