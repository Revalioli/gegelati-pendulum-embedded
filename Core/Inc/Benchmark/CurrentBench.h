#ifndef SRC_BENCHMARK_CURRENTBENCH_H_
#define SRC_BENCHMARK_CURRENTBENCH_H_

#include <Bench.h>

#include "main.h"
#include "CurrentMonitor.h"
#include "ina219.h"

class CurrentBench: public Bench {

	CurrentMonitor * monitor;

	const bool destroyMonitor;

public:

	/// Constructor using its own CurrentMonitor instance
	CurrentBench(void (*fun)(void), INA219_t * ina219t, TIM_HandleTypeDef * tim);

	/// Constructor using an external CurrentMonitor object
	CurrentBench(void (*fun)(void), CurrentMonitor * monitor);

	virtual ~CurrentBench();

	virtual void startBench() override;
};

#endif /* SRC_BENCHMARK_CURRENTBENCH_H_ */
