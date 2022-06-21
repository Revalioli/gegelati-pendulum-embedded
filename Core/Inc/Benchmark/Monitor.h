#ifndef SRC_BENCHMARK_MONITOR_H_
#define SRC_BENCHMARK_MONITOR_H_

#include "main.h"

/// Global callback that calls the record() method of Monitor::activeMonitor
extern "C" void recordActiveMonitor();

/**
 * \brief Abstract class for all kind of Monitor
 *
 * A Monitor can perform record of anything when the record() method is called,
 * and stored these data in a history. It is design to be used with a timer that will call
 * the record() function every update event if the Monitor is the activeMonitor.
 */
class Monitor {

	/// Monitor used with global callback
	static Monitor * activeMonitor;

	/// Associated timer for global callback
	TIM_HandleTypeDef * timer;

	friend void recordActiveMonitor();

public:

	/**
	 * \brief Basic constructor
	 *
	 * \param[out] TIM_HandleTypeDef * the timer to be used for calling recordActiveMonitor() when this is the activeMonitor,
	 * nullptr if you don't want to use this callback.
	 */
	Monitor(TIM_HandleTypeDef * tim = nullptr);

	virtual ~Monitor() {}

    /// Clears the history
    virtual void clearHistory() = 0;

    /// Sends history values to stdout
    virtual void flushHistory() = 0;

    /// Do one record
    virtual void record() = 0;



    /// Sets this CurrentMonitor as the activeMonitor and starts auto measurement using its associated timer
    void makeActive();

    /// Deactivate the active monitor
    static void noActiveMonitor();


};

#endif /* SRC_BENCHMARK_MONITOR_H_ */
