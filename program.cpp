#include <iostream>
#include <vector>
#define int int64_t
using namespace std;

class Station
{
    private:
        int id;
        int time;
        int type;
        int x, y;
    public:
        Station(int id, int time, int type, int x, int y)
        {
            this->id = id;
            this->time = time;
            this->type = type;
            this->x = x;
            this->y = y;
        }
};

class Passenger
{
    private:
        int time;
        int id;
        int start_station_id;
        int end_station_type;
    public:
        Passenger(int id, int time, int start_station_id, int end_station_type)
        {
            this->id = id;
            this->time = time;
            this->start_station_id = start_station_id;
            this->end_station_type = end_station_type;
        }
};

class Train
{
    private:
        int id;
        int time;
    public:
        Train(int id, int time)
        {
            this->id = id;
            this->time = time;
        }
};

class Line
{
    private:
        int id;
        int time;
    public:
        Line(int id, int time)
        {
            this->id = id;
            this->time = time;
        }
};

int event_n, change_time, train_passengers, station_passengers, station_n, passenger_n, line_n, train_n;

vector<Station> stations;
vector<Passenger> passengers;
vector<Line> lines;
vector<Train> trains;

void readInput()
{
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
                Station s = *new Station(id, time, station_type, x, y);
                stations.push_back(s);
                break;
            }
            case 'p': // new passenger
            {
                int start_station, end_station_type;
                cin >> start_station >> end_station_type;
                Passenger p = *new Passenger(id, time, start_station, end_station_type);
                passengers.push_back(p);
                break;
            }
            case 'l': // new line
            {
                Line l = *new Line(id, time);
                lines.push_back(l);
                break;
            }
            default: // new train
            {
                Train t = *new Train(id, time);
                trains.push_back(t);
                break;
            }
        }
    }
}

signed main()
{
    readInput();
}
