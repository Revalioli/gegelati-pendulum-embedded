/*
 * currentMonitor.cpp
 */

#include "CurrentMonitor.h"

#include "ina219.h"



CurrentMonitor* CurrentMonitor::activeMonitor = nullptr;

CurrentMonitor::CurrentMonitor(INA219_t * ina219t, TIM_HandleTypeDef * tim) : currentHistory{0}, currentHistoryIdx(0), averageCurrent(0.0), timer(tim), ina219t(ina219t) {}


void CurrentMonitor::clearHistory(){
	for(int i = 0; i < CURRENT_HISTORY_SIZE; i++)
		this->currentHistory[i] = 0.0;
	
	this->currentHistoryIdx = 0;
	this->averageCurrent = 0.0;
}


void CurrentMonitor::recordCurrent(){

	this->currentHistory[currentHistoryIdx] = INA219_ReadCurrent(this->ina219t);
	currentHistoryIdx++;

	if (currentHistoryIdx == CURRENT_HISTORY_SIZE) {
		currentHistoryIdx = 0;
		averageCurrent = 0.0;

		for(int i = 0; i < CURRENT_HISTORY_SIZE; i++)
			averageCurrent += currentHistory[i];

		averageCurrent = averageCurrent / CURRENT_HISTORY_SIZE;
	}

}

void CurrentMonitor::makeActive(){

	// Stop timer doing auto measurement for old active monitor
	if(CurrentMonitor::activeMonitor != nullptr && CurrentMonitor::activeMonitor->timer != nullptr)
		HAL_TIM_Base_Stop_IT(timer);

	CurrentMonitor::activeMonitor = this;

	// Start timer for auto measurement
	if(CurrentMonitor::activeMonitor->timer != nullptr)
		HAL_TIM_Base_Start_IT(timer);
}



void recordActiveMonitor(){

	if(CurrentMonitor::activeMonitor != nullptr)
		CurrentMonitor::activeMonitor->recordCurrent();

}
