#include "Line.hpp"
#include "Event.hpp"
#include "Solution.hpp"
#include "Station.hpp"
#include <cmath>
using std::pair;

Solution::Line::Line(Solution* sol, int id, int time)
{
    this->sol = sol;
    this->id = id;
    this->time = time;
}

void Solution::Line::reset(Solution* sol)
{
    this->sol = sol;
    this->stations.clear();
    this->trains.clear();
}

pair<int, Solution::Station*> Solution::Line::next_station(Station* s0)
{
    int i = 0;
    while(stations[i] != s0)
    {
        i ++;
    }
    Station* next_s = stations[(i + 1) % stations.size()];
    return  {dist(s0, next_s), next_s};
}

int Solution::Line::dist(Station* s1, Station* s2)
{
    pair<int, int> c1 = s1->get_coordinates();
    pair<int, int> c2 = s2->get_coordinates();
    return ceil(sqrtf(float((c2.first - c1.first) * (c2.first - c1.first) + (c2.second - c1.second) * (c2.second - c1.second))) / 12);
}

// dif, best_swap and find_order were mostly copied from the old program

int Solution::Line::dif(int x1, int y1, int x2, int y2, int x3, int y3, int x4, int y4)
{
    int dist12 = sqrt((x1 - x2) * (x1 - x2) + (y1 - y2) * (y1 - y2));
    int dist34 = sqrt((x3 - x4) * (x3 - x4) + (y3 - y4) * (y3 - y4));
    int dist13 = sqrt((x1 - x3) * (x1 - x3) + (y1 - y3) * (y1 - y3));
    int dist24 = sqrt((x2 - x4) * (x2 - x4) + (y2 - y4) * (y2 - y4));
    return dist13 + dist24 - dist12 - dist34;
}

void Solution::Line::best_swap(vector<Station*>& line)
{
    int ls = line.size();
    int besti = -1, bestj = -1, bestdif = 0;
    for(int i = 0; i < ls; i ++)
    {
        for(int j = 0; j < i; j ++)
        { // 1-2 3-4 -> 1-3 2-4
            pair<int, int> p1 = line[(j - 1 + ls) % ls]->get_coordinates();
            pair<int, int> p2 = line[j]->get_coordinates();
            pair<int, int> p3 = line[i]->get_coordinates();
            pair<int, int> p4 = line[(i + 1) % ls]->get_coordinates();
            if(j == 0 && i == ls - 1)
            {
                continue;
            }
            int curdif = dif(p1.first, p1.second, p2.first, p2.second, p3.first, p3.second, p4.first, p4.second);
            if(curdif < bestdif)
            {
                bestdif = curdif;
                besti = i;
                bestj = j;
            }
        }
    }
    if(besti >= 0)
    {
        vector<Station*> line2 = line;
        for(int i = bestj; i <= besti; i ++)
        {
            line[i] = line2[besti - i + bestj];
        }
    }
}

vector<Solution::Station*> Solution::Line::find_order(vector<Station*> stations)
{
    vector<Station*> ordered = stations;
    for(int i = 0; i < 300; i ++)
    {
        best_swap(ordered);
    }
    return ordered;
}

void Solution::Line::set_stations(vector<Station*> stations, int time)
{
    stations = find_order(stations);
    this->stations = stations;
    vector<int> other;
    other.push_back(stations.size());
    for(Station* station : stations)
    {
        other.push_back(station->add_line(this));
    }
    this->sol->out_events.push_back(new Solution::OutEvent(time, this->id, 'r', other));
}

void Solution::Line::add_trains(int time)
{
    this->trains = this->sol->new_trains;
    for(auto train : this->sol->new_trains) 
    {
        train->set_line(this, this->stations[0], time);
    }
    this->sol->new_trains.clear();
}

bool Solution::Line::contains(int station_type)
{
    bool poss = false;
    for(Station* s : this->stations) poss |= s->type == station_type;
    return poss;
}
