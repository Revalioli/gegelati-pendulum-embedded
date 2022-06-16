#ifndef SRC_BENCHMARK_BENCH_H_
#define SRC_BENCHMARK_BENCH_H_

enum class BenchState {
	Failed,		/// Last benchmark failed or the bench was not yet started, results are invalid
	Ok,			/// Last benchmark succeeded, results are valid
	Working		/// Benchmark is currently running
};

/**
 * \brief Base class for any type of benchmark. It takes a function pointer of type void fun(void) and do actions before and after calling it.
 *
 * This class is intended to be used for benchmarking like timing, current, or any other characteristics about the execution of a function
 * without really focusing on what the function will return.
 */
class Bench {

protected:
	/// The function to be used for the bench
	void (*benchFunction)(void);

	/// State of the Bench
	BenchState state;

public:
	Bench(void (*fun)(void));
	virtual ~Bench() {};

	/// Starts bench
	virtual void startBench() = 0;

	void setBenchFunction(void (*fun)(void));
};

#endif /* SRC_BENCHMARK_BENCH_H_ */
