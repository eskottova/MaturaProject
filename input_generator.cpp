#include <iostream>
#include <random>
#define int int64_t
using namespace std;

class Input
{
    private:
        const int station_type_n = 20;
        const int coordinate_max = 200;
        // station, passenger, line, train
        const vector<int> event_p = {5, 85, 5, 5}; // probabilities for each event
        vector<int> event_n = {0, 0, 0, 0}; // number of times an event has appeared
        vector<int> event_c = {0, 0, 0, 0, 0}; //cutoffs for the events
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

                int j = 0;
                while(e_type > event_c[j + 1]) j ++; // find type

                int index = event_n[j];
                event_n[j] ++;
                
                switch (j) // generates random input for the type and prints it
                {
                    case 0:
                    {
                        int type = uni(rng) % station_type_n;
                        int x = uni(rng) % coordinate_max;
                        int y = uni(rng) % coordinate_max; 
                        cout << t << " s " << index << " " << type << " " << x << " " << y << '\n';
                        break;
                    }
                    case 1:
                    {
                        int dep_type = uni(rng) % station_type_n;
                        int arr_type = uni(rng) % station_type_n; 
                        cout << t << " p " << index << " " << dep_type << " " << arr_type << '\n';
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