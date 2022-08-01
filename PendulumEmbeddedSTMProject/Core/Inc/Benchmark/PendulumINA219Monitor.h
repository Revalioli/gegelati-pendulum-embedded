#ifndef INC_BENCHMARK_PENDULUMINA219MONITOR_H_
#define INC_BENCHMARK_PENDULUMINA219MONITOR_H_

#include <INA219Monitor.h>

#include "PendulumEnvironment.h"

/**
 * \brief Specification of the INA219Monitor class for the Pendulum environment, add record of the Pendulum current step.
 */
class PendulumINA219Monitor: public INA219Monitor {

	/// The PendulumEnvironment instance used.
	PendulumEnvironment& env;

	/// Record the frame number in the PendulumEnvironment for each current measure.
	int frameRecord[INA219Monitor::HISTORY_SIZE];

public:
	/**
	 * \brief Base constructor.
	 * 
	 * \param[out] ina219t the INA219_t * handler to be used for measurements.
	 * \param[in] env the PendulumEnvironement used.
	 * \param[out] tim the TIM_HandleTypeDef * of the timer used for auto measurement when this is the activeMonitor, give nullptr for no auto measurement.
	 * \param[in] timUnit the TimeUnit corresponding to the timer time step between recordActveMonitor() calls.
	 * \param[in] timMultiplier a float multiplier applied on timUnit to get the timer time step between recordActveMonitor() calls.
	 */
	PendulumINA219Monitor(INA219_t * ina219t, PendulumEnvironment& env, TIM_HandleTypeDef * tim = nullptr, TimeUnit timUnit = TimeUnit::None, float timMultiplier = 0.f);
	
	virtual ~PendulumINA219Monitor() {};
	
	/// Inherited from Monitor.
	virtual void record() override;

	/// Inherited from Monitor.
	virtual void flushHistory() override;

	/// Inherited from INA219Monitor.
	virtual void writeHeader() override;
};

#endif /* INC_BENCHMARK_PENDULUMINA219MONITOR_H_ */
