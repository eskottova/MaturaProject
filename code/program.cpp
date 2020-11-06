#include "Input.hpp"

#include "Parameters.hpp"
#include "SimulatedAnnealing.hpp"
#include "GeneticAlgorithm.hpp"
#include "Solution.hpp"


class Event;

signed main()
{

    bool one_sol = false;
    Solution* input_solution = new Solution();

    if(one_sol) // single solution testing
    {
        Parameters* test_pars = new Parameters();
        test_pars->pars = {-8, 44, 305, 494, 34, 33, -965, 291};

        Solution* test_sol = new Solution(input_solution->get_input(), test_pars);

        test_sol->print_result();

        test_sol->print();
    }
    else
    {
        SimulatedAnnealing* SA = new SimulatedAnnealing();
        SA->optimize_solution(input_solution->get_input());
        Solution* sSA = SA->get_best();

        GeneticAlgorithm* GA = new GeneticAlgorithm();
        GA->optimize_solution(input_solution->get_input());
        Solution* sGA = GA->get_best();

        
        if(sSA > sGA) sSA->print();
        else sGA->print();
    }
    
}
