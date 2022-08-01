#ifndef SRC_BENCHMARK_INA219MONITOR_H_
#define SRC_BENCHMARK_INA219MONITOR_H_

#include "Monitor.h"

#include "main.h"
#include "ina219.h"
#include "TimeUnit.h"

/**
 * \brief Specification of the Monitor class for current and power records using an INA219 current sensor.
 */
class INA219Monitor: public Monitor {

protected:

	/// Size of the measurements histories circular buffer.
	static const int HISTORY_SIZE = 100;


	/* === Histories === */

	/// Current measurements history circular buffer.
	double currentHistory[INA219Monitor::HISTORY_SIZE];

	/// Power measurements history circular buffer.
	double powerHistory[INA219Monitor::HISTORY_SIZE];

	/// Index of the next empty position in the measurement histories.
	uint16_t historyIdx;


	/// True if the current must be recorded when calling record().
	bool recordCurrent;
	/// True if the power must be recorded when calling record().
	bool recordPower;

	/// Decounter of the number of calls to record() to skip.
	uint8_t skipRecord;

private:

	/// ina219 handler used for measurements.
	INA219_t * ina219t;

public:

	/// True if histories must be flushed into stdout when full.
	bool flushWhenFull;

	/**
	 * \brief Basic constructor.
	 *
	 * \param[in] ina219t the INA219_t * handler to be used for measurements.
	 * \param[out] tim the TIM_HandleTypeDef * of the timer used for auto measurement when this is the activeMonitor, give nullptr for no auto measurement.
	 * \param[in] timUnit the TimeUnit corresponding to the timer time step between recordActveMonitor() calls.
	 * \param[in] timMultiplier a float multiplier applied on timUnit to get the timer time step between recordActveMonitor() calls.
	 * \param[in] recordCurrent true if the current must be recorded when calling record().
	 * \param[in] recordPower true if the power must be recorded when calling record().
	 */
	INA219Monitor(INA219_t * ina219t, TIM_HandleTypeDef * tim = nullptr, TimeUnit timUnit = TimeUnit::None, float timMultiplier = 0.f, bool recordCurrent = true, bool recordPower = true);
	
	virtual ~INA219Monitor() {}


	/// Inherited from Monitor.
	virtual void clearHistory() override;

	/// Inherited from Monitor.
	virtual void flushHistory() override;

	/// Inherited from Monitor.
	virtual void record() override;

	/// Writes a header for the results in stdout.
	virtual void writeHeader();

};

#endif /* SRC_BENCHMARK_INA219MONITOR_H_ */
