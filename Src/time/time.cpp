/*
 * time.cpp
 *
 *  Created on: Dec 30, 2017
 *      Author: Hans
 */

#include <experimental/optional>
#include "date/date.h"
#include "AlarmParser.h"

using namespace date;
using namespace std::chrono;
using namespace std::experimental;

using time_point_t = std::chrono::time_point<std::chrono::system_clock, std::chrono::seconds>;
using date_t = std::chrono::time_point<std::chrono::system_clock, std::chrono::seconds>;

using alarm_time_t = time_of_day<std::chrono::minutes>;


class AlarmException {
  date_t date;
  optional<alarm_time_t> replacement_time; // if no replacement time is present, the exception says skip

public:
  bool skip() const {
    return !replacement_time;
  }
};

class Alarm {
  alarm_time_t time;
  bool weekdays[7];
  date_t first_day;
  optional<date_t> last_day;
  optional<AlarmException> exceptions[10];

  optional<time_point_t> nextOccurrence(time_point_t t);
};


//Simple population of the exclusion set
bool isNear(time_point_t t, time_point_t now)
{
	const auto halfrange = seconds(5);
	auto start_time = t - halfrange;
	auto end_time = t + halfrange;

	return start_time <= now && now <= end_time;
}

extern "C"
{

  int testAlarm (int secs)
  {
    auto day = sys_days(2002_y/jan/10);

    secs += alarmAtoi("15");

    time_point_t now = day + 1h+2min+seconds(secs);
    time_point_t alarm = day + 1h+2min+10s;

    return isNear(alarm, now);
    return true;
  }

}

