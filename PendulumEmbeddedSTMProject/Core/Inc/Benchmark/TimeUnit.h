#ifndef TIMEUNIT_H_
#define TIMEUNIT_H_

enum class TimeUnit {
	Seconds,
	Milliseconds,
	Microseconds,
	None
};

/// Return the stirng representation of a TimeUnit.
const char * getTimeUnitString(TimeUnit unit);

#endif
