/*
 * currentMonitor.cpp
 */

#include "CurrentMonitor.h"

#include "ina219.h"



CurrentMonitor* CurrentMonitor::activeMonitor = nullptr;

CurrentMonitor::CurrentMonitor() : currentHistory{0}, currentHistoryIdx(0), averageCurrent(0.0) {}


void CurrentMonitor::clearHistory(){
	for(int i = 0; i < CURRENT_HISTORY_SIZE; i++)
		this->currentHistory[i] = 0.0;
	
	this->currentHistoryIdx = 0;
	this->averageCurrent = 0.0;
}


void CurrentMonitor::recordCurrent(INA219_t* ina219t){

	this->currentHistory[currentHistoryIdx] = INA219_ReadCurrent(ina219t);
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
	CurrentMonitor::activeMonitor = this;
}



void recordActiveMonitor(INA219_t* ina219t){

	if(CurrentMonitor::activeMonitor != nullptr)
		CurrentMonitor::activeMonitor->recordCurrent(ina219t);

}
