/*
 * currentMonitor.cpp
 */

#include "CurrentMonitor.h"

#include "ina219.h"
#include <iostream>


CurrentMonitor* CurrentMonitor::activeMonitor = nullptr;

CurrentMonitor::CurrentMonitor(INA219_t * ina219t, TIM_HandleTypeDef * tim) : currentHistory{0}, currentHistoryIdx(0), averageCurrent(0.0), ina219t(ina219t), timer(tim) {}


void CurrentMonitor::clearHistory(){
	for(int i = 0; i < CURRENT_HISTORY_SIZE; i++)
		this->currentHistory[i] = 0.0;
	
	this->currentHistoryIdx = 0;
	this->averageCurrent = 0.0;
}


void CurrentMonitor::recordCurrent(){

	double current = INA219_ReadCurrent(this->ina219t);
	this->currentHistory[currentHistoryIdx] = current;
	currentHistoryIdx++;

	if (currentHistoryIdx == CURRENT_HISTORY_SIZE) {
		currentHistoryIdx = 0;
		averageCurrent = 0.0;

		for(int i = 0; i < CURRENT_HISTORY_SIZE; i++)
			averageCurrent += currentHistory[i];

		averageCurrent = averageCurrent / CURRENT_HISTORY_SIZE;
	}

	// std::cout << current << std::endl;

}

void CurrentMonitor::makeActive(){

	// Stop timer doing auto measurement for old active monitor
	if(CurrentMonitor::activeMonitor != nullptr && CurrentMonitor::activeMonitor->timer != nullptr)
		HAL_TIM_Base_Stop_IT(CurrentMonitor::activeMonitor->timer);

	CurrentMonitor::activeMonitor = this;

	// Start timer for auto measurement
	if(timer != nullptr)
		HAL_TIM_Base_Start_IT(timer);
}



void recordActiveMonitor(){

	if(CurrentMonitor::activeMonitor != nullptr)
		CurrentMonitor::activeMonitor->recordCurrent();

}
