// Include everything necessary here 
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

#include "functions.h"


double generate_random(double min, double max)
{
    // implement a function to return a value between min and max
    double random = ((double) rand())/RAND_MAX;
    double range = (max-min)*random+min;

    return range;

}

unsigned int generate_int()
{
    // implement the function to return a random integer value

    int generated = rand();

    return generated; 

}

// Function to initialize a random population
void generate_population(int POPULATION_SIZE, int NUM_VARIABLES, double population[POPULATION_SIZE][NUM_VARIABLES], double Lbound[NUM_VARIABLES], double Ubound[NUM_VARIABLES])
{
    // randomly initialize for all values in "population[i][j]""
    double random; 

    for(int i = 0; i < POPULATION_SIZE; ++i){
        for(int j=0; j < NUM_VARIABLES; ++j){
            random = generate_random(Lbound[j], Ubound[j]);
            population[i][j] = random;
        }
    }

}

// Function to compute the objective function for each member of the population
void compute_objective_function(int POPULATION_SIZE, int NUM_VARIABLES, double population[POPULATION_SIZE][NUM_VARIABLES], double fitness[POPULATION_SIZE])
{
    double sum1 = 0.0, sum2 = 0.0;
    /* compute "fitness[i]"" for each set of decision variables (individual) or each row in "population"
    by calling "Objective_function" */
    for(int i =0; i < POPULATION_SIZE; ++i){
        
        fitness[i] = Objective_function(NUM_VARIABLES, population[i]);
        
    }


    
}

void crossover(int POPULATION_SIZE, int NUM_VARIABLES, double fitness[POPULATION_SIZE], double new_population[POPULATION_SIZE][NUM_VARIABLES], double population[POPULATION_SIZE][NUM_VARIABLES], double crossover_rate)
{
    /* Implement the logic of crossover function here based on "fitness_probs" or each set
    of decision variables (individual) or each row in "population".
    And save the new population in "new_population"*/

    double *fitness_probs = (double *) malloc(POPULATION_SIZE * sizeof(double));
    double sum = 0.0; 
    double *cumulative = (double*) malloc(POPULATION_SIZE*sizeof(double));

    for(int i =0; i < POPULATION_SIZE; ++i){
        fitness_probs[i] = 1/(1e-15+fitness[i]);
        sum += fitness_probs[i];
    }
    //normalizing 
    for(int i =0; i < POPULATION_SIZE; ++i){
        fitness_probs[i] = fitness_probs[i]/sum;
    }
    cumulative[0] = fitness_probs[0];

    for(int i =1; i < POPULATION_SIZE; ++i){
        cumulative[i] = cumulative[i-1] + fitness_probs[i];
    }

    //do general
    for(int i=0; i < POPULATION_SIZE; ++i){
        double random = generate_random(0,1);

        for(int j=0; j < POPULATION_SIZE; ++j){
            if(random < cumulative[j]){
                for(int k=0; k < NUM_VARIABLES; ++k){
                    new_population[i][k] = population[j][k];
                } 
                break; 
            }
        }
    }

    double parent1[NUM_VARIABLES], parent2[NUM_VARIABLES];
    double child1[NUM_VARIABLES], child2[NUM_VARIABLES];
    double random; 

    double crosspoint[NUM_VARIABLES-1];

    for(int i = 1; i < NUM_VARIABLES; ++i){
        crosspoint[i-1] = i;
    }

    for(int i=0; i < POPULATION_SIZE-1; ++i){
        random = generate_random(0,1);
        if(random <= crossover_rate){
            for(int j=0; j < NUM_VARIABLES; ++j){
                parent1[j] = new_population[i][j];
                parent2[j] = new_population[i+1][j];

            }
            //random crosspoint
            int index = rand() % (NUM_VARIABLES-1);
            int cross = crosspoint[index];


            for(int j=0; j < cross; ++j){
                child1[NUM_VARIABLES-j-1] = parent2[NUM_VARIABLES-j-1];
                child1[j] = parent1[j];

                child2[NUM_VARIABLES-j-1] = parent1[NUM_VARIABLES-j-1];
                child2[j] = parent2[j];
            }

        }

        for(int k=0; k < NUM_VARIABLES; ++k){
            new_population[i][k] = child1[k];
            new_population[i+1][k] = child2[k];
        }
    }

    free(fitness_probs);
    free(cumulative);


}

void mutate(int POPULATION_SIZE, int NUM_VARIABLES, double new_population[POPULATION_SIZE][NUM_VARIABLES], double population[POPULATION_SIZE][NUM_VARIABLES], double Lbound[NUM_VARIABLES], double Ubound[NUM_VARIABLES], double mutate_rate)
{   
    /*Implement the logic of mutation on "new_population" and then copy everything into "population"*/
    
    int total_gen = POPULATION_SIZE * NUM_VARIABLES;
    int total_mutate = total_gen*mutate_rate;

    int filled = 0;
    int *indices = (int *) malloc(total_mutate*sizeof(int));
    int *index = (int *) malloc(total_gen * sizeof(int));
    for (int i= 0; i < total_gen; ++i){
        index[i] = i;
    }


    for(int i =0; i < total_mutate; ++i){
        int random = rand() % total_gen;
        if(index[random] != -1){
            indices[filled] = index[random];
            index[random] = -1;
            filled++;
        }
        else{
            i--;
        }
    }

    int row, col; 
    int dummy;
    double value;

    for(int i =0; i < total_mutate; ++i){
        dummy = indices[i];
        row = dummy / NUM_VARIABLES;
        col = dummy % NUM_VARIABLES;

        value = generate_random(Lbound[col],Ubound[col]);

        new_population[row][col] = value;
    }

    for(int i =0; i < POPULATION_SIZE; ++i){
        for(int j=0; j < NUM_VARIABLES; ++j){
            population[i][j]= new_population[i][j];
        }
    }

    free(indices);
    free(index);


}
