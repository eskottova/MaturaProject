#include "Input.hpp"

#include "Station.hpp"
#include "Passenger.hpp"
#include "Line.hpp"
#include "Train.hpp"

#include "NewStation.hpp"
#include "NewPassenger.hpp"
#include "NewLine.hpp"
#include "NewTrain.hpp"


#include <iostream>

using std::cin;

Solution::Input::Input(Solution* sol)
{
    this->sol = sol;
    cin >> event_n >> change_time >> train_passengers >> station_passengers >> station_n >> passenger_n >> line_n >> train_n;
    for(int i = 0; i < event_n; i ++)
    {
        int time, id;
        char type;
        cin >> time >> type >> id;
        switch (type)
        {
            case 's': // new station
            {
                int station_type, x, y;
                cin >> x >> y >> station_type;
                Solution::Station* s = new Station(this->sol, id, time, station_type, x, y);
                this->all_stations.push_back(s);
                this->sol->all_stations.push_back(s);
                this->input_events.push_back(new NewStation(this->sol, s, time));
                break;
            }
            case 'p': // new passenger
            {
                int start_station_id, end_station_type;
                cin >> start_station_id >> end_station_type;
                Solution::Passenger* p = new Passenger(this->sol, id, time, start_station_id, end_station_type);
                this->all_passengers.push_back(p);
                this->input_events.push_back(new NewPassenger(this->sol, p, this->sol->all_stations[start_station_id], time));
                break;
            }
            case 'l': // new line
            {
                Solution::Line* l = new Line(this->sol, id, time);
                this->all_lines.push_back(l);
                this->input_events.push_back(new NewLine(this->sol, l, time));
                break;
            }
            default: // new train
            {
                Solution::Train* t = new Train(this->sol, id, time);
                this->all_trains.push_back(t);
                this->input_events.push_back(new NewTrain(this->sol, t, time));
                break;
            }
        }
    }
}

void Solution::Input::set_sol(Solution* sol)
{
    this->sol = sol;
    for(Event* e : this->input_events) e->set_sol(sol);
}