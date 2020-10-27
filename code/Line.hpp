#ifndef LINE_H
#define LINE_H

#include <vector>
#include <queue>
#include "Solution.hpp"

using std::vector;
using std::priority_queue;

class Solution::Line
{
    private:
        Solution* sol;
        int id;
        int time;
        vector<Station*> stations;
        vector<Train*> trains;
        int dist(Station* s1, Station* s2);        
    public:
        Line(Solution* sol, int id, int time);
        std::pair<int, Station*> next_station(Station* s0);
        void set_stations(vector<Station*>, int time);
        void add_trains(int time);
        int get_id(){return this->id;}
        Station* get_start(){return this->stations[0];}
        bool contains(int station_type);
        int dif(int x1, int y1, int x2, int y2, int x3, int y3, int x4, int y4);
        void best_swap(vector<Station*>& line);
        vector<Station*> find_order(vector<Station*> stations);
        void reset(Solution* sol);
};
#endif