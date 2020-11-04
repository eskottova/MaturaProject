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
    vector<Station*> lstations;
    if(this->sol->new_stations.size() == this->sol->active_stations.size()) lstations = this->sol->new_stations;
    else
    {
        for(int i = 0; i < this->sol->new_stations.size(); i ++) // adds the new stations as well as the same number of already added stations
        {
            lstations.push_back(this->sol->new_stations[i]);
            int ind = (int)this->sol->active_stations.size() - 2 * (int)this->sol->new_stations.size() + i;
            if(ind >= 0 && ind < this->sol->active_stations.size()) lstations.push_back(this->sol->active_stations[this->sol->active_stations.size() - 2 * this->sol->new_stations.size() + i]); 
        }
    }
    
    if(lstations.size() > 2)
    {
        this->line->set_stations(lstations, this->time);
        this->sol->new_stations.clear();
        this->sol->active_lines.push_back(this->line);

    }
    else
    {
        this->sol->events.push(new NewLine(this->sol, this->line, this->time + 40));
    }

    return false;
}
