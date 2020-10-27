#ifndef PARAMETERS_H
#define PARAMETERS_H

#include <vector>
using std::vector;

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

#endif
