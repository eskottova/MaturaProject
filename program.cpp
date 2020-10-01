#include <iostream>
#include <vector>
#include <queue>
#include <cmath>
#include <random>
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

class Optimization;
class SimulatedAnnealing;
class GeneticAlgorithm;

class Solution;
class Parameters;

class OutEvent;

// random generator
mt19937 rng(10007);
uniform_int_distribution<int> uni(0, 1e12);

int randint(int mi, int ma)
{
    return uni(rng) % (ma - mi) + mi;
}

auto compare = [](auto const& lhs, auto const& rhs)
{
    if(lhs->get_time() == rhs->get_time()) return lhs->get_prio() > rhs->get_prio();
    else return lhs->get_time() > rhs->get_time();
};

auto compare_solptr = [](auto const& lhs, auto const& rhs)
{
    return lhs->val < rhs->val;
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
        int x, y;
        vector<Line*> lines;
        vector<Passenger*> passengers;
    public:
        int type;
        Station(int id, int time, int type, int x, int y);
        bool add_passenger(Passenger* passenger);
        void board(Parameters* para, Train* train, int time, vector<OutEvent*>& out_events);
        int add_line(Line*);
        int get_cap();
        int get_id(){return this->id;}
        pair<int, int> get_coordinates(){return {this->x, this->y};}
        void reset();
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
        bool check_exit(Parameters* para, Station* stat);
        bool check_boarding(Parameters* para, Train* train);
        int get_id(){return this->id;}
        void reset();
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
        void unboard(Parameters* para, Station* station, int time, vector<OutEvent*>& out_events);
        int get_cap();
        void set_line(Line* line, Station* station, int time, priority_queue<Event*, vector<Event*>, decltype(compare)>& events, vector<OutEvent*>& out_events);
        void set_station(Station* station);
        pair<int, Station*> find_next_station();
        int get_id(){return this->id;}
        void add_pass(Passenger* p);
        bool line_contains(int station_type);
        void reset();
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
        bool contains(int station_type);
        void reset();
};

class Event
{
    protected:
        int time;
        int priority;
    public:
        virtual bool run(Parameters* para, priority_queue<Event*, vector<Event*>, decltype(compare)>& events, vector<OutEvent*>& out_events);
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
        virtual bool run(Parameters* para, priority_queue<Event*, vector<Event*>, decltype(compare)>& events, vector<OutEvent*>& out_events);
};

class NewStation : public Event
{
    private:
        Station* station;
    public:
        NewStation(Station* station, int time);
        virtual bool run(Parameters* para, priority_queue<Event*, vector<Event*>, decltype(compare)>& events, vector<OutEvent*>& out_events);
};

class NewPassenger : public Event
{
    private:
        Passenger* passenger;
        Station* station;
    public:
        NewPassenger(Passenger* passenger, Station* station, int time);
        virtual bool run(Parameters* para, priority_queue<Event*, vector<Event*>, decltype(compare)>& events, vector<OutEvent*>& out_events);
};

class NewLine : public Event
{
    private:
        Line* line;
    public:
        NewLine(Line* line, int time);
        virtual bool run(Parameters* para, priority_queue<Event*, vector<Event*>, decltype(compare)>& events, vector<OutEvent*>& out_events);
};

class NewTrain : public Event
{
    private:
        Train* train;
    public:
        NewTrain(Train* train, int time);
        virtual bool run(Parameters* para, priority_queue<Event*, vector<Event*>, decltype(compare)>& events, vector<OutEvent*>& out_events);
};

class TrainDeparture : public Event
{
    private:
        Train* train;
        Station* station;
        bool newtrain;
    public:
        TrainDeparture(int time, bool newtrain, Train* train, Station* station);
        virtual bool run(Parameters* para, priority_queue<Event*, vector<Event*>, decltype(compare)>& events, vector<OutEvent*>& out_events);
};

class SimulatedAnnealing
{
    private:
        Solution* bests;
        int init_temp = 100;
        int it_per_temp = 20;
        int alpha = 10;
    public:
        void optimize_solution();
        Solution* get_best() {return bests;}
};

class GeneticAlgorithm
{
    private:
        Solution* bests;
        vector<Solution*> population;
        int population_size = 10;
        int num_gens = 20;
        int num_replaced = 3;
        int num_offspring = 10;
        int num_mutations = 5;
    public:
        void optimize_solution();
        vector<pair<Parameters*, Parameters*>> selection();
        vector<Parameters*> crossing(vector<pair<Parameters*, Parameters*>> parents);
        vector<Parameters*> mutation(vector<Parameters*> offspring);
        void compute_offspring(vector<Parameters*> offspring);
        Solution* get_best() {return bests;}

};


class Solution
{
    private:
        int pass_finished = 0;
        int steps = 0;
    public:
        Parameters* para;
        int val;
        vector<OutEvent*> out_events;
        Solution(Parameters* para);
        Parameters* neighbor();
        void print();
        bool operator<(Solution const& s) {return this->val < s.val;}
};

class Parameters
{
    private:
        vector<int> maxpars = {100, 100};
        vector<int> minpars = {0, 0};
        int numpars = 2;
    public:
        Parameters();
        vector<int> pars;
        vector<int> random_pars();
        void random_par(int i);
};

class OutEvent
{
    private:
        int t, id;
        char type;
        vector<int> other;
    public:
        OutEvent(int t, int id, char type, vector<int> other);
        char get_type(){return this->type;}
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

void Station::reset()
{
    this->lines.clear();
    this->passengers.clear();
}
        
void Station::board(Parameters* para, Train* train, int time, vector<OutEvent*>& out_events)
{
    for(auto it = this->passengers.begin(); it != this->passengers.end(); it ++)
    {
        if((*it)->check_boarding(para, train))
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

int Station::get_cap()
{
    return station_passengers - this->passengers.size();
}

Passenger::Passenger(int id, int time, int start_station_id, int end_station_type)
{
    this->id = id;
    this->time = time;
    this->start_station_id = start_station_id;
    this->end_station_type = end_station_type;
    this->station = all_stations[start_station_id];
}

void Passenger::reset()
{
    this->station = all_stations[this->start_station_id];
    this->train = nullptr;
    this->line = nullptr;
}


bool Passenger::check_exit(Parameters* para, Station* stat)
{
    if(stat->type == end_station_type) 
    {
        train = nullptr;
        station = stat;
        return true;
    }
    return false;
}

bool Passenger::check_boarding(Parameters* para, Train* tra)
{
    if((tra->line_contains(this->end_station_type) && tra->get_cap() > 0) && para->pars[0] * this->station->get_cap() < para->pars[1] * tra->get_cap())
    {
        this->train = tra;
        this->station = nullptr;
        return true;
    }
    else return false;
}

Train::Train(int id, int time)
{
    this->id = id;
    this->time = time;
}

void Train::reset()
{
    this->passengers.clear();
    this->line = nullptr;
    this->station = nullptr;
}

void Train::unboard(Parameters* para, Station* station, int time, vector<OutEvent*>& out_events)
{
    for(auto it = passengers.begin(); it != passengers.end(); it ++)
    {
        if((*it)->check_exit(para, station))
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

bool Train::line_contains(int station_type)
{
    return this->line->contains(station_type);
}

Line::Line(int id, int time)
{
    this->id = id;
    this->time = time;
}

void Line::reset()
{
    this->stations.clear();
    this->trains.clear();
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
    pair<int, int> c1 = s1->get_coordinates();
    pair<int, int> c2 = s2->get_coordinates();
    return ceil(sqrtf(float((c2.first - c1.first) * (c2.first - c1.first) + (c2.second - c1.second) * (c2.second - c1.second))) / 12);
}


// dif, best_swap and find_order were mostly copied from the old program

int dif(int x1, int y1, int x2, int y2, int x3, int y3, int x4, int y4)
{
    int dist12 = sqrt((x1 - x2) * (x1 - x2) + (y1 - y2) * (y1 - y2));
    int dist34 = sqrt((x3 - x4) * (x3 - x4) + (y3 - y4) * (y3 - y4));
    int dist13 = sqrt((x1 - x3) * (x1 - x3) + (y1 - y3) * (y1 - y3));
    int dist24 = sqrt((x2 - x4) * (x2 - x4) + (y2 - y4) * (y2 - y4));
    return dist13 + dist24 - dist12 - dist34;
}

void best_swap(vector<Station*>& line)
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

vector<Station*> find_order(vector<Station*> stations)
{
    vector<Station*> ordered = stations;
    for(int i = 0; i < 300; i ++)
    {
        best_swap(ordered);
    }
    return ordered;
}


void Line::set_stations(vector<Station*> stations, int time, vector<OutEvent*>& out_events)
{
    stations = find_order(stations);
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

bool Line::contains(int station_type)
{
    bool poss = false;
    for(Station* s : this->stations) poss |= s->type == station_type;
    return poss;
}

bool Event::run(Parameters* para, priority_queue<Event*, vector<Event*>, decltype(compare)>& events, vector<OutEvent*>& out_events)
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

bool TrainArrival::run(Parameters* para, priority_queue<Event*, vector<Event*>, decltype(compare)>& events, vector<OutEvent*>& out_events)
{
    train->set_station(this->station);
    train->unboard(para, station, this->time, out_events);
    return false;
}

NewStation::NewStation(Station* station, int time)
{
    this->time = time;
    this->station = station;
    this->priority = 1;
}

bool NewStation::run(Parameters* para, priority_queue<Event*, vector<Event*>, decltype(compare)>& events, vector<OutEvent*>& out_events)
{
    this->station->reset();
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

bool NewPassenger::run(Parameters* para, priority_queue<Event*, vector<Event*>, decltype(compare)>& events, vector<OutEvent*>& out_events)
{
    this->passenger->reset();
    last_action = this->time;
    return this->station->add_passenger(this->passenger);
}


NewLine::NewLine(Line* line, int time)
{
    this->line = line;
    this->time = time;
    this->priority = 3;
}

bool NewLine::run(Parameters* para, priority_queue<Event*, vector<Event*>, decltype(compare)>& events, vector<OutEvent*>& out_events)
{
    this->line->reset();
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
    
    if(lstations.size() > 2)
    {
        this->line->set_stations(lstations, this->time, out_events);
        new_stations.clear();

        if(last_line == nullptr) 
        {
            this->line->add_trains(this->time, events, out_events);
            new_trains.clear();
        }
        last_line = this->line;
    }
    else
    {
        events.push(new NewLine(this->line, this->time + 40));
    }

    return false;
}

NewTrain::NewTrain(Train* train, int time)
{
    this->train = train;
    this->time = time;
    this->priority = 4;
}

bool NewTrain::run(Parameters* para, priority_queue<Event*, vector<Event*>, decltype(compare)>& events, vector<OutEvent*>& out_events)
{
    this->train->reset();
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

bool TrainDeparture::run(Parameters* para, priority_queue<Event*, vector<Event*>, decltype(compare)>& events, vector<OutEvent*>& out_events)
{
    if(!this->newtrain) station->board(para, this->train, this->time, out_events);

    pair<int, Station*> next_info = this->train->find_next_station();
    
    events.push(new TrainArrival(next_info.first + 1 + this->time, train, next_info.second));
    events.push(new TrainDeparture(next_info.first + 1 + this->time, false, train, next_info.second));
    return false;
}

void SimulatedAnnealing::optimize_solution()
{
    Parameters* para = new Parameters();
    Solution* cursol = new Solution(para);
    this->bests = cursol;
    int temp = this->init_temp;
    while(temp > 0)
    {
        for(int i = 0; i < this->it_per_temp; i ++)
        {
            Solution* snew = new Solution(cursol->neighbor());
            if(snew->val > cursol->val || 1000 * exp((float) (snew->val - cursol->val) / temp) > randint(0, 1000))
            {
                cursol = snew;
            }
        }
        temp -= this->alpha;
        if(cursol->val > bests->val) this->bests = cursol;
    }
}

vector<pair<Parameters*, Parameters*>> GeneticAlgorithm::selection()
{
    int valsum = 0;
    for(auto individual : this->population) valsum += individual->val;
    
    vector<int> parents(2 * this->num_offspring);
    for(int i = 0; i < 2 * this->num_offspring; i ++) parents[i] = randint(0, valsum);

    sort(parents.begin(), parents.end());

    vector<pair<Parameters*, Parameters*>> parent_pars(num_offspring);

    int i = 0;
    int j = 0;
    int curvalsum = 0;
    while(i < population_size)
    {
        curvalsum += population[i]->val;
        while(j < 2 * this->num_offspring && curvalsum > parents[j])
        {
            if(j % 2 == 0) parent_pars[j / 2].first = population[i]->para;
            else parent_pars[j / 2].second = population[i]->para;
            j ++;
        }
        i ++;
    }
    
    return parent_pars;
}

vector<Parameters*> GeneticAlgorithm::crossing(vector<pair<Parameters*, Parameters*>> parent_pars)
{
    vector<Parameters*> offspring_pars(num_offspring);

    int chrom_size = parent_pars[0].first->pars.size();

    for(int i = 0; i < num_offspring; i ++)
    {
        int cutoff = randint(0, chrom_size);

        offspring_pars[i] = new Parameters;

        for(int gene = 0; gene < chrom_size; gene ++)
        {
            if(gene < cutoff) offspring_pars[i]->pars[gene] = parent_pars[i].first->pars[gene];
            else offspring_pars[i]->pars[gene] = parent_pars[i].second->pars[gene];
        }
    }
    return offspring_pars;
}

vector<Parameters*> GeneticAlgorithm::mutation(vector<Parameters*> offspring_pars)
{
    int chrom_size = offspring_pars[0]->pars.size();
    for(int i = 0; i < num_mutations; i ++)
    {
        offspring_pars[randint(0, this->num_offspring)]->random_par(randint(0, chrom_size));
    }
    return offspring_pars;
}

void GeneticAlgorithm::compute_offspring(vector<Parameters*> offspring_pars)
{
    vector<Solution*> offspring(num_offspring);
    for(int i = 0; i < num_offspring; i ++) offspring[i] = new Solution(offspring_pars[i]);

    sort(offspring.begin(), offspring.end(), compare_solptr);
    sort(this->population.begin(), this->population.end(), compare_solptr);

    int i = 0;
    while(i < num_replaced && population[i] < offspring[this->num_offspring - 1 - i])
    {
        population[i] = offspring[this->num_offspring - i - 1];
    }
}

void GeneticAlgorithm::optimize_solution()
{
    //create population
    this->population.resize(population_size);
    for(int i = 0; i < population_size; i ++)
    {
        Parameters* para = new Parameters();
        this->population[i] = new Solution(para);
    }

    // compute all generations
    int generation = 0;
    while(generation < this->num_gens)
    {
        this->compute_offspring(this->mutation(this->crossing(this->selection())));
        generation ++;
    }

    //set best solution*/
    this->bests = this->population[0];
    for(auto individual : population) if(individual->val > bests->val) bests = individual;
}

Solution::Solution(Parameters* para)
{
    cerr << "new solution " << para->pars[0] << " " << para->pars[1] << "\n";
    new_stations.clear();
    stations.clear();
    this->para = para;
    priority_queue<Event*, vector<Event*>, decltype(compare)> events = input_events;
    bool overfilled = false;
    this->steps = 0;
    while(!overfilled && !events.empty() && this->steps < last_action + 2000)
    {   
        Event* e = events.top();
        events.pop();
        this->steps = e->get_time();
        overfilled = e->run(para, events, this->out_events);
    }

    this->pass_finished = 0;
    for(auto oe : this->out_events) if(oe->get_type() == 'a') pass_finished ++;

    this->val = this->pass_finished * 10000 - this->steps;
}

Parameters* Solution::neighbor()
{
    Parameters* neighbor = new Parameters;
    for(int i = 0; i < this->para->pars.size(); i ++) neighbor->pars[i] = this->para->pars[i];
    int changep = randint(0, neighbor->pars.size());
    neighbor->random_par(changep);
    return neighbor;
}

void Solution::print()
{
    for(auto event : this->out_events)
    {
        event->print();
    }
}

Parameters::Parameters()
{
    this->pars.resize(this->numpars);
    this->pars = random_pars();
}

vector<int> Parameters::random_pars()
{
    vector<int> p(this->pars.size());
    for(int i = 0; i < this->pars.size(); i ++) p[i] = randint(this->minpars[i], this->maxpars[i]);
    return p;
}

void Parameters::random_par(int i)
{
    this->pars[i] = randint(this->minpars[i], this->maxpars[i]);
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


signed main()
{
    readInput();

    GeneticAlgorithm* GA = new GeneticAlgorithm();
    GA->optimize_solution();
    Solution* sGA = GA->get_best();
    sGA->print();
    
    SimulatedAnnealing* SA = new SimulatedAnnealing();
    SA->optimize_solution();
    Solution* sSA = SA->get_best();
    sSA->print();
}
