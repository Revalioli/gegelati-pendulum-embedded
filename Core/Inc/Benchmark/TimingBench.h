#ifndef SRC_TIMINGBENCH_H_
#define SRC_TIMINGBENCH_H_

#include "main.h"

#include <vector>

#include "Bench.h"


enum class TimeUnit {
	Seconds,
	Milliseconds,
	Microseconds
};

class TimingBench : public Bench {

	/* === Timer === */

	/// Timer used for benchmark
	TIM_HandleTypeDef * tim;

	/// Time unit for timer
	TimeUnit timerUnit;

	/// Multiplier to applied to timer result
	float timerMultiplier;


	/* === Results == */

	/// Number of timing test to do
	int nbAttempts;

	/// Raw timing results for each attempts
	std::vector<int> attemptResults;

	/// Raw average timing of last bench
	int result;

public:

	/**
	 * \brief Base constructor for TimingBench.
	 *
	 * This bench times the execution duration of its bench function using a timer.
	 * The measure is done multiples times, and an average value is computed at the end.
	 * Any interrupt that may occur during the execution of the bench function will be
	 * part of the final result.
	 *
	 * \param[out] fun: pointer to the bench function
	 * \param[out] timer: timer handler of the timer used
	 * \param[in] nbAttempts: number of timing measure to do
	 * \param[in] unit: TimeUnit corresponding to the timer tick
	 * \param[in] unitMultiplier: unit multiplier corresponding to the timer tick
	 */
	TimingBench(void (*fun)(void), TIM_HandleTypeDef * timer, int nbAttempts = 5, TimeUnit unit = TimeUnit::Milliseconds, float unitMultiplier = 1.f);

	virtual ~TimingBench() {};

	/// Starts bench, return the timing (average on results of all attempts)
	/// Multiple this result by the period of the timer to get the actual elapsed time
	void startBench() override;

	/// Gets last raw average timing result, -1 if the bench was not yet started
	int getResult();

	/// Prints results on stdout (only if the bench is not working)
	void printResult();
};

#endif /* SRC_TIMINGBENCH_H_ */
