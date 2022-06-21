/*
 * currentMonitor.cpp
 */

#include "CurrentMonitor.h"
#include "Monitor.h"

#include "ina219.h"
#include <iostream>


CurrentMonitor::CurrentMonitor(INA219_t * ina219t, TIM_HandleTypeDef * tim)
		: Monitor(tim), ina219t(ina219t), currentHistory{0}, currentHistoryIdx(0), averageCurrent(0.0), flushWhenFull(true) {}


void CurrentMonitor::clearHistory(){
	for(int i = 0; i < CURRENT_HISTORY_SIZE; i++)
		this->currentHistory[i] = 0.0;
	
	this->currentHistoryIdx = 0;
	this->averageCurrent = 0.0;
}

void CurrentMonitor::flushHistory(){

	for(int i = 0; i < this->currentHistoryIdx; i++)
		std::cout << "Current : " << this->currentHistory[i] << std::endl;

}


void CurrentMonitor::record(){

	double current = INA219_ReadCurrent(this->ina219t);
	this->currentHistory[currentHistoryIdx] = current;
	currentHistoryIdx++;

	if (currentHistoryIdx == CURRENT_HISTORY_SIZE) {
		averageCurrent = 0.0;

		for(int i = 0; i < CURRENT_HISTORY_SIZE; i++)
			averageCurrent += currentHistory[i];

		averageCurrent = averageCurrent / CURRENT_HISTORY_SIZE;


		if(this->flushWhenFull)
			flushHistory();

		currentHistoryIdx = 0;
	}

}

