#ifndef _BENCH_FUNCTIONS_H_
#define _BENCH_FUNCTIONS_H_

#include "main.h"
#include "PendulumEnvironment.h"
#include "CurrentMonitor.h"

extern uint32_t seed;
extern uint16_t nbActions;
extern uint64_t actions[NB_ACTIONS];
extern PendulumEnvironment * pendulum_ptr;
extern CurrentMonitor * monitor_ptr;


void inferenceBenchWrapper(void);
void currentMeasurementTimingBenchWrappe(void);
void environmentEvolutionTimingBenchWrapper(void);
void inferenceCurrentBenchWrapper(void);

#endif
