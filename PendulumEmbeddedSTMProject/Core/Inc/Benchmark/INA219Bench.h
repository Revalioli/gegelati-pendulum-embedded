#ifndef SRC_BENCHMARK_INA219BENCH_H_
#define SRC_BENCHMARK_INA219BENCH_H_

#include <Bench.h>

#include "main.h"
#include "INA219Monitor.h"
#include "ina219.h"

/**
 * \brief Specification of the Bench class for current and power benchmarks using an INA219 sensor. 
 */
class INA219Bench: public Bench {

	/// Pointer to the used INA219Monitor
	INA219Monitor * monitor;

	// True if the monitor instance is own only by this object and must be destroyed with it.
	const bool destroyMonitor;

public:

	/// Constructor using its own INA219Monitor instance.
	INA219Bench(void (*fun)(void), INA219_t * ina219t, TIM_HandleTypeDef * tim,
			TimeUnit timUnit = TimeUnit::None, float timMultiplier = 0.f, bool recordCurrent = true, bool recordPower = true);

	/// Constructor using an external INA219Monitor object.
	INA219Bench(void (*fun)(void),  INA219Monitor * monitor);

	virtual ~INA219Bench();

	/// Inherited from Bench.
	virtual void startBench() override;
};

#endif /* SRC_BENCHMARK_INA219BENCH_H_ */
