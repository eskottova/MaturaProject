#include <vector>
#include "Parameters.hpp"
#include "random_generator.hpp"
#include <iostream>

Parameters::Parameters()
{
    this->pars.resize(this->numpars);
    this->pars = random_pars();
}

std::vector<int> Parameters::random_pars()
{
    std::vector<int> p(this->pars.size());
    for(int i = 0; i < this->pars.size(); i ++) p[i] = randint(this->minpars[i], this->maxpars[i]);
    return p;
}

void Parameters::random_par(int i)
{
    this->pars[i] = randint(this->minpars[i], this->maxpars[i]);
}
