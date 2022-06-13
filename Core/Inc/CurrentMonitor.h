/*
 * currentMonitor.h
 */
#ifndef CURRENTMONITOR_H_
#define CURRENTMONITOR_H_

#include "PendulumEnvironment.h"

/// Calls the recordCurrent() method of CurrentMonitor::activeMonitor
extern "C" void recordActiveMonitor(INA219_t* ina219t);


class CurrentMonitor {

	/// CurrentMonitor used with global callback
	static CurrentMonitor* activeMonitor;

	/// Size of the current measurements history circular buffer
	static const int CURRENT_HISTORY_SIZE = 100;	// Default = 100 --> 800 bytes

	/// Current measurements history circular buffer
	double currentHistory[CURRENT_HISTORY_SIZE];

	/// Index of the current position in the current measurement history
	uint16_t currentHistoryIdx;

	/// Last compute averageCurrent
	double averageCurrent;
	

	friend void recordActiveMonitor(INA219_t* ina219t);

public:

	CurrentMonitor();
	virtual ~CurrentMonitor() {}

    /// Clears the current measurement history
    void clearHistory();

    /**
     * \brief Retrieve from the ina219t the current
     *
     * \param[in] ina219t the INA219_t* for the current sensor ina219t
     */
    void recordCurrent(INA219_t* ina219t);

    /// Sets this CurrentMonitor as the activeMonitor
    void makeActive();

};


#endif /* CURRENTMONITOR_H_ */
