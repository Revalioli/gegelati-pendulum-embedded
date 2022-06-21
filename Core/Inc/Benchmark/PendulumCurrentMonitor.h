#ifndef INC_BENCHMARK_PENDULUMCURRENTMONITOR_H_
#define INC_BENCHMARK_PENDULUMCURRENTMONITOR_H_

#include <INA219Monitor.h>

#include "PendulumEnvironment.h"

class PendulumCurrentMonitor: public INA219Monitor {

	PendulumEnvironment& env;

	/// Record the frame number in the PendulumEnvironment for each current measure
	int frameRecord[INA219Monitor::HISTORY_SIZE];

public:
	PendulumCurrentMonitor(INA219_t * ina219t, PendulumEnvironment& env, TIM_HandleTypeDef * tim = nullptr);
	virtual ~PendulumCurrentMonitor() {};
	
	virtual void record() override;

	virtual void flushHistory() override;

	virtual void writeHeader() override;
};

#endif /* INC_BENCHMARK_PENDULUMCURRENTMONITOR_H_ */
