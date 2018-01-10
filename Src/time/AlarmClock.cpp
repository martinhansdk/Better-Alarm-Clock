/*
 * time.cpp
 *
 *  Created on: Dec 30, 2017
 *      Author: Hans
 */
#include <sstream>

#include "AlarmClock.h"

const char * const wdnames[] = {"sun", "mon", "tue", "wed", "thu", "fri", "sat" };

Alarm::Alarm() {
  weekdays = ALL_WEEKDAYS;
  no_exceptions = 0;
}

void Alarm::addException(const AlarmException& e) {
  if(no_exceptions < EXCEPTIONS) {
      exceptions[no_exceptions++] = e;
  }
}

optional<time_point_secs_t> Alarm::nextOccurrence(time_point_secs_t t) const {
  t += 1s; // advance to next instant after t
  // find first time point at or after t with
  sys_days date = date::floor<date::days>(t);
  auto timepart = t - date;
  auto alarmtime = time.to_duration();

  if(timepart > alarmtime) {
      date += days(1);
  }

  if(first_day && date < first_day.value())
    date = first_day.value(); // fast forward to first_day

  // naively search for a day that is not excluded somehow
  int tries = 366;
  while(tries--) {
      date += days(1);
      if(date > last_day)
	break; // past the last day - won't find anything

      weekday wd = weekday{date};
      if(!onWeekday(wd)) continue;

      // found a match!
      return date + alarmtime;
  }

  // no match found :-(
  return {};
}

std::string Alarm::toString() const {
  std::stringstream ss;
  ss << "alarm " << time;

  if(weekdays != ALL_WEEKDAYS) {
    // we have missing weekdays! So print the full list
      ss << " every ";
      bool first=true;
      for(int j=0 ; j<7 ; j++) {
	  if(weekdays & (1<<j)) {
	      if(!first) ss << " ";
	      first = false;
	      ss << wdnames[j];
	  }
      }
  }

  if(first_day) ss << " from " << year_month_day{floor<days>(first_day.value())};
  if(last_day) ss << " until " << year_month_day{floor<days>(last_day.value())};

  for(int i=0 ; i < no_exceptions ; i++) {
      ss << " but on " << year_month_day{floor<days>(exceptions[i].date())};
      if(exceptions[i].skip()) {
	  ss << " skip";
      } else {
	  ss << " " << exceptions[i].time().value();
      }

  }

  ss << ";";
  return ss.str();
}

//Simple population of the exclusion set
bool isNear(time_point_secs_t t, time_point_secs_t now)
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



    time_point_secs_t now = day + 1h+2min+seconds(secs);
    time_point_secs_t alarm = day + 1h+2min+10s;

    return isNear(alarm, now);
    return true;
  }

}

