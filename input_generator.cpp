#include <iostream>
#include <random>
#include <set>
#define int int64_t
using namespace std;

class Input
{
    private:
        const int station_type_n = 20;
        const int max_coordinate_d = 100;
        int cur_station_types = 0;
        // station, passenger, line, train
        const vector<int> event_p = {120, 3890, 11, 33}; // probabilities for each event (taken from input file)
        vector<int> event_n = {0, 0, 0, 0}; // number of times an event has appeared
        vector<int> event_c = {0, 0, 0, 0, 0}; //cutoffs for the events
        vector<pair<int, int>> station_coordinates;
        set<pair<int, int>> station_set;
        void compute_cutoffs()
        {
            for(int i = 1; i < 5; i ++)
            {
                event_c[i] = event_c[i - 1] + event_p[i - 1];
            }
        }
    public:
        Input(int n, int seed)
        {
            this->compute_cutoffs();

            // random generator
            mt19937 rng(seed);
            uniform_int_distribution<int> uni(0, 1e12);

            int t = 0; // time since the beginning 
            for(int i = 0; i < n; i ++)
            {
                int e_type = uni(rng) % event_c[4]; // random event type
                if(i < 3) e_type = 0;
                int j = 0;
                while(e_type > event_c[j + 1]) j ++; // find type

                int index = event_n[j];
                event_n[j] ++;
                
                switch (j) // generates random input for the type and prints it
                {
                    case 0:
                    {
                        int type = uni(rng) % station_type_n;
                        if(type >= cur_station_types) // ensures that the station types appear increasingly
                        {
                            type = cur_station_types;
                            cur_station_types ++;
                        }

                        int x, y;
                        bool not_new = true; //used to make sure the coordinates of stations are unique
                        while(not_new)
                        {
                            x = uni(rng) % 2 * max_coordinate_d - max_coordinate_d;
                            y = uni(rng) % 2 * max_coordinate_d - max_coordinate_d;
                            if(station_set.size() == 0) 
                            {
                                not_new = false;
                                break;
                            }
                            pair<int, int> other_station = station_coordinates[uni(rng) % station_coordinates.size()];
                            x += other_station.first;
                            y += other_station.second; 
                            if(station_set.count({x, y}) == 0) not_new = false;
                        }
                        station_set.insert({x, y});
                        station_coordinates.push_back({x, y});
                        cout << t << " s " << index << " " << type << " " << x << " " << y << '\n';
                        break;
                    }
                    case 1:
                    {
                        int dep_station = uni(rng) % station_coordinates.size(); // makes sure passenger starts at a valid station
                        int arr_type = uni(rng) % cur_station_types; // ensures that the station type is already active
                        cout << t << " p " << index << " " << dep_station << " " << arr_type << '\n';
                        break;
                    }
                    case 2:
                    {
                        cout << t << " l " << index << '\n';
                        break;
                    }
                    default:
                    {
                        cout << t << " z " << index << '\n';
                        break;
                    }
                }
                t += uni(rng) % 50;
            }
        }
};

signed main()
{
    int n, seed;
    cin >> n >> seed;
    Input in(n, seed);
    return 0;
}