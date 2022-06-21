#ifndef SRC_BENCHMARK_INA219BENCH_H_
#define SRC_BENCHMARK_INA219BENCH_H_

#include <Bench.h>

#include "main.h"
#include "INA219Monitor.h"
#include "ina219.h"

class INA219Bench: public Bench {

	INA219Monitor * monitor;
	const bool destroyMonitor;

public:

	/// Constructor using its own CurrentMonitor instance
	INA219Bench(void (*fun)(void), INA219_t * ina219t, TIM_HandleTypeDef * tim,
				bool recordCurrent = true, bool recordPower = true);

	/// Constructor using an external INA219Monitor object
	INA219Bench(void (*fun)(void),  INA219Monitor * monitor);

	virtual ~INA219Bench();

	/// Inherited from Bench
	virtual void startBench() override;
};

#endif /* SRC_BENCHMARK_INA219BENCH_H_ */
