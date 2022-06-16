#include "TimingBench.h"

#include "main.h"

TimingBench::TimingBench(void (*function)(void), TIM_HandleTypeDef * timer, int nbAttempts) : tim(timer), bench_fun(function), nbAttempts(nbAttempts), result(0) {
	HAL_TIM_Base_Stop(timer);
}

int TimingBench::startBench(){

	this->result = 0;
	this->attemptResults.clear();

	for(int i = 0; i < this->nbAttempts; i++){
		// Reset counter and start timer
		__HAL_TIM_SET_COUNTER(this->tim, (int32_t)(0x0));
		HAL_TIM_Base_Start(this->tim);

		(*this->bench_fun)();

		HAL_TIM_Base_Stop(tim);
		uint32_t raw = (uint32_t)(__HAL_TIM_GET_COUNTER(this->tim));
		int timing = (int)raw;
		this->attemptResults.push_back(timing);
		this->result += timing;
	}

	this->result = this->result / this->nbAttempts;

	return this->result;

}
