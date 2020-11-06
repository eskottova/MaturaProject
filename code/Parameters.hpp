#ifndef PARAMETERS_H
#define PARAMETERS_H

#include <vector>
using std::vector;

/*

PARAMETER DESCRIPTIONS:
    0: free space at station (boarding)
    1: free space on train (boarding)
    2: train line contains correct end station (boarding)
    3: station has other line with correct end station (boarding)
    4: free space at station (leaving train)
    5: free space on train (leaving train)
    6: train line contains correct end station (leaving train)
    7: station has other line with correct end station (leaving train)
    
    No line changes for now:
        8: wait between line changes
        9: new station (finding stations for new line)
        10: number of passengers at station (finding stations for new line)
        11: number of stations already chosen for line (finding stations for new line)

*/

class Parameters
{
    private:
        vector<int> maxpars = {50, 50, 1000, 500, 50, 50, 1000, 500};
        vector<int> minpars = {-50, -50, -1000, -500, -50, -50, -1000, -500};
        int numpars = minpars.size();
    public:
        Parameters();
        vector<int> pars;
        vector<int> random_pars();
        void random_par(int i);
};

#endif
