#ifndef SRC_BENCHMARK_MONITOR_H_
#define SRC_BENCHMARK_MONITOR_H_

#include "main.h"
#include "TimeUnit.h"

/// Global callback that calls the record() method of Monitor::activeMonitor.
extern "C" void recordActiveMonitor();

/**
 * \brief Base abstract class for all kind of Monitors.
 *
 * A Monitor can perform record of anything when the record() method is called,
 * and store data in a history.
 *
 * This class is designed to allow periodically record of the Monitor using a timer.
 * If a Monitor is the activeMonitor (use makeActive() method), then every call
 * to recordActiveMonitor() will call the activeMonitor record() method.
 * The recordActiveMonitor() may then be call by a timer interrupt
 * to periodically record the activeMonitor.
 */
class Monitor {

	/// Monitor used with global callback.
	static Monitor * activeMonitor;


	/* === Timer === */

	/// Associated timer when used as activeMonitor.
	TIM_HandleTypeDef * timer;

protected:
	/// Timer period unit (Seconds, Milliseconds or Microseconds).
	TimeUnit timerUnit;

	/// Unit multiplier for one timer period.
	float timerMultiplier;


	friend void recordActiveMonitor();periodperiod

public:

	/**
	 * \brief Basic constructor.
	 *
	 * \param[out] tim the TIM_HandleTypeDef* of the associated timer when used as activeMonitor, give nullptr if you don't want to use this callback.
	 * \param[in] timUnit: the TimeUnit corresponding to the timer period unit, unused if tim = nullptr.
	 * \param[in] timMultiplier: multiplier to be applied on timUnit to get the timer period unit multiplier, unused if tim = nullptr.
	 */
	Monitor(TIM_HandleTypeDef * tim = nullptr, TimeUnit timUnit = TimeUnit::None, float timMultiplier = 0.f);

	virtual ~Monitor() {}

    /// Clear the measurement history.
    virtual void clearHistory() = 0;

    /// Send history values to stdout.
    virtual void flushHistory() = 0;

    /// Do one record.
    virtual void record() = 0;



    /**
     * \brief Set this Monitor as the activeMonitor and starts auto measurement using its associated timer.
     * 
	 * If this Monitor has no timer, then this method is equivalent to Monitor::noActiveMonitor().
     */
    void makeActive();

    /// Set the activeMonitor to nullptr.
    static void noActiveMonitor();


};

#endif /* SRC_BENCHMARK_MONITOR_H_ */
