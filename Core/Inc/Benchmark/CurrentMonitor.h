/*
 * currentMonitor.h
 */
#ifndef CURRENTMONITOR_H_
#define CURRENTMONITOR_H_

#include "PendulumEnvironment.h"

#include "tim.h"
#include "ina219.h"

/// Calls the recordCurrent() method of CurrentMonitor::activeMonitor
extern "C" void recordActiveMonitor();


class CurrentMonitor {

	/// CurrentMonitor used with global callback
	static CurrentMonitor* activeMonitor;

	/// Size of the current measurements history circular buffer
	static const int CURRENT_HISTORY_SIZE = 50;	// Default = 100 --> 800 bytes



	/// Current measurements history circular buffer
	double currentHistory[CURRENT_HISTORY_SIZE];

	/// Index of the current position in the current measurement history
	uint16_t currentHistoryIdx;

	/// Last compute averageCurrent
	double averageCurrent;
	
	/// ina219 handler to be used for current measurement
	INA219_t * ina219t;

	/// Associated timer for auto measurement
	TIM_HandleTypeDef * timer;


	friend void recordActiveMonitor();

public:

	/// True if the currentHistory must be flushed into stdout when full
	bool flushWhenFull;

	/**
	 * \brief Basic constructor
	 *
	 * \param[out] INA219_t * the ina219 handler to be used for current measurements
	 * \param[out] TIM_HandleTypeDef * the timer used for auto measurement when this is the activeMonitor, pass nullptr for no auto measurement
	 */
	CurrentMonitor(INA219_t * ina219t, TIM_HandleTypeDef * tim = nullptr);
	virtual ~CurrentMonitor() {}

    /// Clears the current measurement history
    void clearHistory();

    /// Fetches and stores the current value from the INA219
    void recordCurrent();

    /// Sets this CurrentMonitor as the activeMonitor and starts auto measurement using its associated timer
    void makeActive();


    /// Deactivate the active monitor
    static void noActiveMonitor();

};


#endif /* CURRENTMONITOR_H_ */
