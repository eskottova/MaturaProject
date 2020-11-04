#include "SimulatedAnnealing.hpp"
#include "Parameters.hpp"
#include "Solution.hpp"
#include "random_generator.hpp"
#include "Input.hpp"
#include "cmath"

void SimulatedAnnealing::optimize_solution(Solution::Input* input)
{
    this->input = input;
    Parameters* para = new Parameters();
    std::cerr << "para print ";
    for(int i : para->pars) std::cerr << i << " ";
    std::cerr << "\n";
    Solution* cursol = new Solution(this->input, para);
    this->bests = cursol;
    int temp = this->init_temp;
    while(temp > 0)
    {
        for(int i = 0; i < this->it_per_temp; i ++)
        {
            Solution* snew = new Solution(this->input, cursol->neighbor());
            if(snew->val > cursol->val || 1000 * exp((float) (snew->val - cursol->val) / temp) > randint(0, 1000))
            {
                cursol = snew;
            }
            if(cursol->val > bests->val) this->bests = cursol;
        }
        temp -= this->alpha;
    }
}
