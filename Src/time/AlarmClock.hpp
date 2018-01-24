#ifndef ALARMCLOCK_H
#define ALARMCLOCK_H

#ifdef __cplusplus
#define EXTERNC extern "C"
#else
#define EXTERNC
#endif

//#define ONLY_C_LOCALE 1

#include <array>
#include <experimental/optional>
#include <string>

#include "date/date.h"

using namespace date;
using namespace std::chrono;
using namespace std::experimental;

using time_point_secs_t = std::chrono::time_point<std::chrono::system_clock, std::chrono::seconds>;
using date_t = sys_days;

using time_of_day_minutes_t = time_of_day<std::chrono::minutes>;

class AlarmException {
  date_t _date;
  optional<time_of_day_minutes_t> replacement_time; // if no replacement time is present, the exception says skip

public:
  AlarmException() {};
  AlarmException(date_t date) : _date(date) {};
  AlarmException(date_t date, time_of_day_minutes_t replacement_time) : _date(date), replacement_time(replacement_time) {};

  date_t date() const {
    return _date;
  }

  bool skip() const {
    return !replacement_time;
  }

  time_of_day_minutes_t time() const {
    return replacement_time.value();
  }
};

class Alarm {
public:

  static const int SUN=0;
  static const int MON=1;
  static const int TUE=2;
  static const int WED=3;
  static const int THU=4;
  static const int FRI=5;
  static const int SAT=6;
  static const uint8_t ALL_WEEKDAYS = 0x7F;

  static const int EXCEPTIONS=10;

  time_of_day_minutes_t time;
  uint8_t weekdays;
  optional<date_t> first_day;
  optional<date_t> last_day;

  int no_exceptions;
  std::array<AlarmException, EXCEPTIONS> exceptions;

  Alarm();
  optional<time_point_secs_t> nextOccurrence(time_point_secs_t t) const;
  void addException(const AlarmException &e);
  bool onWeekday(weekday wd) const {
    return (weekdays & (1 << unsigned(wd))) != 0;
  }

  std::string toString() const;
};

#undef EXTERNC

#endif
