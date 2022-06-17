#ifndef SRC_BENCHMARK_CURRENTBENCH_H_
#define SRC_BENCHMARK_CURRENTBENCH_H_

#include <Bench.h>

#include "main.h"
#include "CurrentMonitor.h"
#include "ina219.h"

class CurrentBench: public Bench {

	CurrentMonitor monitor;


public:


	CurrentBench(void (*fun)(void), INA219_t * ina219t, TIM_HandleTypeDef * tim);

	virtual ~CurrentBench() {};

	virtual void startBench() override;
};

#endif /* SRC_BENCHMARK_CURRENTBENCH_H_ */
