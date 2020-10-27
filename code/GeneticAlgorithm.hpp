#ifndef GENETICALGORITHM_H
#define GENETICALGORITHM_H

#include "Solution.hpp"
#include "Input.hpp"

class GeneticAlgorithm
{
    private:
        Solution* bests;
        Solution::Input* input;
        vector<Solution*> population;
        int population_size = 10;
        int num_gens = 20;
        int num_replaced = 3;
        int num_offspring = 10;
        int num_mutations = 5;
    public:
        void optimize_solution(Solution::Input* input);
        vector<std::pair<Parameters*, Parameters*>> selection();
        vector<Parameters*> crossing(vector<std::pair<Parameters*, Parameters*>> parents);
        vector<Parameters*> mutation(vector<Parameters*> offspring);
        void compute_offspring(vector<Parameters*> offspring);
        Solution* get_best() {return bests;}

};

#endif