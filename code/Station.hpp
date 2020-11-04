#ifndef STATION_H
#define STATION_H


#include "Line.hpp"
#include "Passenger.hpp"
#include "Train.hpp"
#include "OutEvent.hpp"
#include "Line.hpp"
#include "Passenger.hpp"
#include "Solution.hpp"
#include <vector>

using std::vector;

class Solution::Station
{
    private:
        Solution* sol;
        int id;
        int time;
        int x, y;
        vector<Solution::Line*> lines;
        vector<Solution::Passenger*> passengers;
    public:
        int type;
        Station(Solution* sol, int id, int time, int type, int x, int y);
        bool add_passenger(Passenger* passenger);
        void board(Train* train, int time);
        int add_line(Line*);
        int get_cap();
        int get_id(){return this->id;}
        std::pair<int, int> get_coordinates(){return {this->x, this->y};}
        void reset(Solution* sol);
        int num_lines_type(int type);
        int new_line_score();
};
#endif