#include <iostream>
#include <vector>
#include <queue>
#include <cmath>
#define int int64_t
using namespace std;

class Station;
class Train;
class Passenger;
class Line;

class Event;
class TrainArrival;
class NewStation;
class NewPassenger;
class NewLine;
class NewTrain;
class TrainDeparture;

class Solution;
class Parameters;

class OutEvent;

auto compare = [](auto const& lhs, auto const& rhs)
{
    if(lhs->get_time() == rhs->get_time()) return lhs->get_prio() > rhs->get_prio();
    else return lhs->get_time() > rhs->get_time();
};

int event_n, change_time, train_passengers, station_passengers, station_n, passenger_n, line_n, train_n;
Line* last_line = nullptr;
priority_queue<Event*, vector<Event*>, decltype(compare)> input_events(compare);
int last_action = 0;

vector<Station*> stations;
vector<Station*> all_stations;
vector<Station*> new_stations;
vector<Line*> lines;
vector<Train*> trains;
vector<Train*> new_trains;


class Station
{
    private:
        int id;
        int time;
        vector<Line*> lines;
        vector<Passenger*> passengers;
    public:
        int x, y;
        int type;
        Station(int id, int time, int type, int x, int y);
        bool add_passenger(Passenger* passenger);
        void board(Train* train, int time, vector<OutEvent*>& out_events);
        int add_line(Line*);
        int get_id(){return this->id;}
};

class Passenger
{
    private:
        int time;
        int id;
        int start_station_id;
        int end_station_type;
        Train* train;
        Station* station;
        Line* line;
    public:
        Passenger(int id, int time, int start_station_id, int end_station_type);
        bool check_exit(Station* stat);
        bool check_boarding(Train* train);
        int get_id(){return this->id;}
};

class Train
{
    private:
        int id;
        int time;
        vector<Passenger*> passengers;
        Line* line;
        Station* station;
    public:
        Train(int id, int time);
        void unboard(Station* station, int time, vector<OutEvent*>& out_events);
        int get_cap();
        void set_line(Line* line, Station* station, int time, priority_queue<Event*, vector<Event*>, decltype(compare)>& events, vector<OutEvent*>& out_events);
        void set_station(Station* station);
        pair<int, Station*> find_next_station();
        int get_id(){return this->id;}
        void add_pass(Passenger* p);
};

class Line
{
    private:
        int id;
        int time;
        vector<Station*> stations;
        vector<Train*> trains;
        int dist(Station* s1, Station* s2);        
    public:
        Line(int id, int time);
        pair<int, Station*> next_station(Station* s0);
        void set_stations(vector<Station*>, int time, vector<OutEvent*>& out_events);
        void add_trains(int time, priority_queue<Event*, vector<Event*>, decltype(compare)>& events, vector<OutEvent*>& out_events);
        int get_id(){return this->id;}
        Station* get_start(){return this->stations[0];}
};

class Event
{
    protected:
        int time;
        int priority;
    public:
        virtual bool run(priority_queue<Event*, vector<Event*>, decltype(compare)>& events, vector<OutEvent*>& out_events);
        virtual int get_time(){return this->time;}
        virtual int get_prio(){return this->priority;}
        virtual ~Event(){};
};

class TrainArrival : public Event
{
    private:
        Train* train;
        Station* station;
    public:
        TrainArrival(int time, Train* train, Station* station);
        virtual bool run(priority_queue<Event*, vector<Event*>, decltype(compare)>& events, vector<OutEvent*>& out_events);
};

class NewStation : public Event
{
    private:
        Station* station;
    public:
        NewStation(Station* station, int time);
        virtual bool run(priority_queue<Event*, vector<Event*>, decltype(compare)>& events, vector<OutEvent*>& out_events);
};

class NewPassenger : public Event
{
    private:
        Passenger* passenger;
        Station* station;
    public:
        NewPassenger(Passenger* passenger, Station* station, int time);
        virtual bool run(priority_queue<Event*, vector<Event*>, decltype(compare)>& events, vector<OutEvent*>& out_events);
};

class NewLine : public Event
{
    private:
        Line* line;
    public:
        NewLine(Line* line, int time);
        virtual bool run(priority_queue<Event*, vector<Event*>, decltype(compare)>& events, vector<OutEvent*>& out_events);
};

class NewTrain : public Event
{
    private:
        Train* train;
    public:
        NewTrain(Train* train, int time);
        virtual bool run(priority_queue<Event*, vector<Event*>, decltype(compare)>& events, vector<OutEvent*>& out_events);
};

class TrainDeparture : public Event
{
    private:
        Train* train;
        Station* station;
        bool newtrain;
    public:
        TrainDeparture(int time, bool newtrain, Train* train, Station* station);
        virtual bool run(priority_queue<Event*, vector<Event*>, decltype(compare)>& events, vector<OutEvent*>& out_events);
};

class Solution
{
    private:
        Parameters* p;
        bool finished = false;
        int steps = 0;
        
        bool operator>(Solution& s)
        {
            if(this->finished == s.finished)
            {
                if(this->finished == true)
                {
                    return this->steps < s.steps;
                }
                else
                {
                    return this->steps > s.steps;
                }
                
            }
            else
            {
                return this->finished == true;
            }
        }
    public:
        vector<OutEvent*> out_events;
        void print();
};

class Parameters
{
    public:
        // boarding passenger
        int pass_at_station;
        int pass_on_train;

};

class OutEvent
{
    private:
        int t, id;
        char type;
        vector<int> other;
    public:
        OutEvent(int t, int id, char type, vector<int> other);
        void print()
        {
            cout << this->t << " " << this->type << " " << this->id;
            for(int el : this->other)
            {
                cout << " " << el;
            }
            cout << '\n';
        }
};

Station::Station(int id, int time, int type, int x, int y)
{
    this->id = id;
    this->time = time;
    this->type = type;
    this->x = x;
    this->y = y;

}
        
void Station::board(Train* train, int time, vector<OutEvent*>& out_events)
{
    for(auto it = this->passengers.begin(); it != this->passengers.end(); it ++)
    {
        if(train->get_cap() > 0 && (*it)->check_boarding(train))
        {
            train->add_pass(*it);
            out_events.push_back(new OutEvent(time, (*it)->get_id(), 'e', {train->get_id()}));
            it = passengers.erase(it) - 1;
            last_action = time;
        }
    }
}

bool Station::add_passenger(Passenger* passenger)
{
    this->passengers.push_back(passenger);
    return this->passengers.size() > station_passengers;
}

int Station::add_line(Line* line)
{
    this->lines.push_back(line);
    return this->id;
}

Passenger::Passenger(int id, int time, int start_station_id, int end_station_type)
{
    this->id = id;
    this->time = time;
    this->start_station_id = start_station_id;
    this->end_station_type = end_station_type;
    this->station = all_stations[start_station_id];
}

bool Passenger::check_exit(Station* stat)
{
    if(stat->type == end_station_type) 
    {
        train = nullptr;
        station = stat;
        return true;
    }
    return false;
}

bool Passenger::check_boarding(Train* tra)
{
    this->train = tra;
    this->station = nullptr;
    return true;
}

Train::Train(int id, int time)
{
    this->id = id;
    this->time = time;
}

void Train::unboard(Station* station, int time, vector<OutEvent*>& out_events)
{
    for(auto it = passengers.begin(); it != passengers.end(); it ++)
    {
        if((*it)->check_exit(station))
        {
            out_events.push_back(new OutEvent(time, (*it)->get_id(), 'a', {station->get_id()}));
            it = passengers.erase(it) - 1;
            last_action = time;
        }
    }
}

void Train::add_pass(Passenger* p)
{
    this->passengers.push_back(p);
}

int Train::get_cap()
{
    return train_passengers - this->passengers.size();
}

void Train::set_line(Line* line, Station* station, int time, priority_queue<Event*, vector<Event*>, decltype(compare)>& events, vector<OutEvent*>& out_events)
{
    this->line = line;
    this->station = station;
    events.push(new TrainDeparture(time, true, this, station));
    out_events.push_back(new OutEvent(time, this->id, 'z', {this->line->get_id(), 0}));
}

void Train::set_station(Station* station)
{
    this->station = station;
}

pair<int, Station*> Train::find_next_station()
{
    return this->line->next_station(this->station);
}

Line::Line(int id, int time)
{
    this->id = id;
    this->time = time;

}

pair<int, Station*> Line::next_station(Station* s0)
{
    int i = 0;
    while(stations[i] != s0)
    {
        i ++;
    }
    Station* next_s = stations[(i + 1) % stations.size()];
    return  {dist(s0, next_s), next_s};
}

int Line::dist(Station* s1, Station* s2)
{
    return ceil(sqrtf(float((s2->x - s1->x) * (s2->x - s1->x) + (s2->y - s1->y) * (s2->y - s1->y))) / 12);
}

void Line::set_stations(vector<Station*> stations, int time, vector<OutEvent*>& out_events)
{
    this->stations = stations;
    vector<int> other;
    other.push_back(stations.size());
    for(Station* station : stations)
    {
        other.push_back(station->add_line(this));
    }
    out_events.push_back(new OutEvent(time, this->id, 'r', other));
}

void Line::add_trains(int time, priority_queue<Event*, vector<Event*>, decltype(compare)>& events, vector<OutEvent*>& out_events)
{
    this->trains = new_trains;
    for(auto train : new_trains) 
    {
        train->set_line(this, this->stations[0], time, events, out_events);
    }
    new_trains.clear();
}

bool Event::run(priority_queue<Event*, vector<Event*>, decltype(compare)>& events, vector<OutEvent*>& out_events)
{
    return false;
}

TrainArrival::TrainArrival(int time, Train* train, Station* station)
{
    this->time = time;
    this->train = train;
    this->station = station;
    this->priority = 0;
}

bool TrainArrival::run(priority_queue<Event*, vector<Event*>, decltype(compare)>& events, vector<OutEvent*>& out_events)
{
    train->set_station(this->station);
    train->unboard(station, this->time, out_events);
    return false;
}

NewStation::NewStation(Station* station, int time)
{
    this->time = time;
    this->station = station;
    this->priority = 1;
}

bool NewStation::run(priority_queue<Event*, vector<Event*>, decltype(compare)>& events, vector<OutEvent*>& out_events)
{
    last_action = this->time;
    new_stations.push_back(station);
    stations.push_back(station);
    return false;
}

NewPassenger::NewPassenger(Passenger* passenger, Station* station, int time)
{
    this->passenger = passenger;
    this->station = station;
    this->time = time;
    this->priority = 2;
}

bool NewPassenger::run(priority_queue<Event*, vector<Event*>, decltype(compare)>& events, vector<OutEvent*>& out_events)
{
    last_action = this->time;
    return this->station->add_passenger(this->passenger);
}


NewLine::NewLine(Line* line, int time)
{
    this->line = line;
    this->time = time;
    this->priority = 3;
}

bool NewLine::run(priority_queue<Event*, vector<Event*>, decltype(compare)>& events, vector<OutEvent*>& out_events)
{
    last_action = this->time;
    vector<Station*> lstations;
    if(new_stations.size() == stations.size()) lstations = new_stations;
    else
    {
        for(int i = 0; i < new_stations.size(); i ++) // adds the new stations as well as the same number of already added stations
        {
            lstations.push_back(new_stations[i]);
            int ind = (int)stations.size() - 2 * (int)new_stations.size() + i;
            if(ind >= 0 && ind < stations.size()) lstations.push_back(stations[stations.size() - 2 * new_stations.size() + i]); 
        }
    }
    
    this->line->set_stations(lstations, this->time, out_events);
    new_stations.clear();

    if(last_line == nullptr) 
    {
        this->line->add_trains(this->time, events, out_events);
        new_trains.clear();
    }
    last_line = this->line;

    return false;
}

NewTrain::NewTrain(Train* train, int time)
{
    this->train = train;
    this->time = time;
    this->priority = 4;
}

bool NewTrain::run(priority_queue<Event*, vector<Event*>, decltype(compare)>& events, vector<OutEvent*>& out_events)
{
    last_action = this->time;
    if(last_line == nullptr) new_trains.push_back(this->train);
    else train->set_line(last_line, last_line->get_start(), this->time, events, out_events);
    return false;
}

TrainDeparture::TrainDeparture(int time, bool newtrain, Train* train, Station* station)
{
    this->time = time;
    this->train = train;
    this->station = station;
    this->priority = 5;
    this->newtrain = newtrain;
}

bool TrainDeparture::run(priority_queue<Event*, vector<Event*>, decltype(compare)>& events, vector<OutEvent*>& out_events)
{
    if(!this->newtrain) station->board(this->train, this->time, out_events);

    pair<int, Station*> next_info = this->train->find_next_station();
    
    events.push(new TrainArrival(next_info.first + 1 + this->time, train, next_info.second));
    events.push(new TrainDeparture(next_info.first + 1 + this->time, false, train, next_info.second));

    return false;
}

void Solution::print()
{
    for(auto event : this->out_events)
    {
        event->print();
    }
}

OutEvent::OutEvent(int t, int id, char type, vector<int> other)
{
    this->t = t;
    this->id = id;
    this->type = type;
    this->other = other;
}

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
                Station* s = new Station(id, time, station_type, x, y);
                all_stations.push_back(s);
                input_events.push(new NewStation(s, time));
                break;
            }
            case 'p': // new passenger
            {
                int start_station_id, end_station_type;
                cin >> start_station_id >> end_station_type;
                Passenger* p = new Passenger(id, time, start_station_id, end_station_type);
                input_events.push(new NewPassenger(p, all_stations[start_station_id], time));
                break;
            }
            case 'l': // new line
            {
                Line* l = new Line(id, time);
                input_events.push(new NewLine(l, time));
                break;
            }
            default: // new train
            {
                Train* t = new Train(id, time);
                input_events.push(new NewTrain(t, time));
                break;
            }
        }
    }
}

Solution computeSolution(/*const Parameters& p*/)
{
    Solution s;
    priority_queue<Event*, vector<Event*>, decltype(compare)> events = input_events;
    bool overfilled = false;
    int t = 0;
    while(!overfilled && !events.empty() && t < last_action + 2000)
    {   
        Event* e = events.top();
        events.pop();
        t = e->get_time();
        overfilled = e->run(events, s.out_events);
    }
    return s;

}

Solution simulatedAnnealing()
{
    Parameters p;
    return computeSolution();
}

Solution geneticAlgorithm()
{
    Parameters p;
    return computeSolution();
}

signed main()
{
    readInput();
    /*Solution s1 = simulatedAnnealing();
    Solution s2 = geneticAlgorithm();
    s1.print();
    s2.print();*/
    Solution s = computeSolution();
    s.print();
}
