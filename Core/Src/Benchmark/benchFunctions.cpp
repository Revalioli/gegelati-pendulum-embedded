#include "benchFunctions.h"


void inferenceBenchWrapper(void){
	// Setup
	seed = HAL_GetTick();
	pendulum_ptr->reset(seed);
	pendulum_ptr->startInference((int)nbActions);
}

void currentMeasurementTimingBenchWrappe(void){
	monitor_ptr->record();
}

void environmentEvolutionTimingBenchWrapper(void){
	seed = HAL_GetTick();
	pendulum_ptr->reset(seed);
	for(int i = 0; i < 20; i++){
		for(int j = 0; j < NB_ACTIONS; j++){
			pendulum_ptr->doAction(actions[j]);
		}
	}
}

void inferenceCurrentBenchWrapper(void){
	seed = HAL_GetTick();
	pendulum_ptr->reset(seed);
	pendulum_ptr->startInference((int)nbActions);
}
