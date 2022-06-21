#ifndef INC_BENCHMARK_PENDULUMINA219MONITOR_H_
#define INC_BENCHMARK_PENDULUMINA219MONITOR_H_

#include <INA219Monitor.h>

#include "PendulumEnvironment.h"

class PendulumINA219Monitor: public INA219Monitor {

	PendulumEnvironment& env;

	/// Record the frame number in the PendulumEnvironment for each current measure
	int frameRecord[INA219Monitor::HISTORY_SIZE];

public:
	PendulumINA219Monitor(INA219_t * ina219t, PendulumEnvironment& env, TIM_HandleTypeDef * tim = nullptr);
	virtual ~PendulumINA219Monitor() {};
	
	virtual void record() override;

	virtual void flushHistory() override;

	virtual void writeHeader() override;
};

#endif /* INC_BENCHMARK_PENDULUMINA219MONITOR_H_ */
