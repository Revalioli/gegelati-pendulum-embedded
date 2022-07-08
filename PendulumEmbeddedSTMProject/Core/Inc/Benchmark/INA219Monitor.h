#ifndef SRC_BENCHMARK_INA219MONITOR_H_
#define SRC_BENCHMARK_INA219MONITOR_H_

#include "Monitor.h"

#include "main.h"
#include "ina219.h"
#include "TimeUnit.h"

class INA219Monitor: public Monitor {

protected:

	/// Size of the measurements histories circular buffer
	static const int HISTORY_SIZE = 100;


	/* === Histories === */

	/// Current measurements history circular buffer
	double currentHistory[INA219Monitor::HISTORY_SIZE];

	/// Power measurements history circular buffer
	double powerHistory[INA219Monitor::HISTORY_SIZE];

	/// Index of the next empty position in the measurement histories
	uint16_t historyIdx;


	/// True if the current must be recorded
	bool recordCurrent;
	/// True if the power must be recorded
	bool recordPower;

	uint8_t skipRecord;

private:

	/// ina219 handler to be used for measurements
	INA219_t * ina219t;

public:

	/// True if histories must be flushed into stdout when full
	bool flushWhenFull;

	/**
	 * \brief Basic constructor
	 *
	 * \param[out] INA219_t * the ina219 handler to be used for measurements
	 * \param[out] TIM_HandleTypeDef * the timer used for auto measurement when this is the activeMonitor, pass nullptr for no auto measurement
	 * \param[in] timUnit: the TimeUnit corresponding to the timer time step between recordActveMonitor() calls
	 * \param[in] timMultiplier: multiplier to be applied on timUnit to get the timer time step between recordActveMonitor() calls
	 * \param[in] recordCurrent: true if the current must be recorded when calling record()
	 * \param[in] recordPower: true if the power must be recorded when calling record()
	 */
	INA219Monitor(INA219_t * ina219t, TIM_HandleTypeDef * tim = nullptr, TimeUnit timUnit = TimeUnit::None, float timMultiplier = 0.f, bool recordCurrent = true, bool recordPower = true);
	virtual ~INA219Monitor() {}


	/// Inherited from Monitor
	virtual void clearHistory() override;

	/// Inherited from Monitor
	virtual void flushHistory() override;

	/// Inherited from Monitor
	virtual void record() override;

	/// Writes a header for the results in stdout
	virtual void writeHeader();

};

#endif /* SRC_BENCHMARK_INA219MONITOR_H_ */
