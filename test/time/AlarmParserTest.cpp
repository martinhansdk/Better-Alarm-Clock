/*
 * AlarmParserTest.cpp
 *
 *  Created on: Jan 2, 2018
 *      Author: Hans
 */

#include "catch.hpp"
#include "date/date.h"
#include "AlarmParser.h"

using namespace date;

namespace Catch {

  std::string toString(time_point_secs_t const &value) {
    using namespace date;
    using namespace std::chrono;

    auto today = date::floor<days>(value);

    std::stringstream ss;
    ss << year_month_day(today) << ' ' << date::make_time(value - today) << " UTC";
    return ss.str();
  }

  template<>
  struct StringMaker<time_point_secs_t> {
    static std::string convert( time_point_secs_t const& value ) {
      return toString(value);
    }
  };

  template<>
  struct StringMaker<optional<time_point_secs_t>> {
    static std::string convert( optional<time_point_secs_t> const& value ) {
      if(value)
	return toString(value.value());
      else
	return "empty";
    }
  };

}

TEST_CASE( "Alarm is parsed", "[alarm,io]") {

#define TEST_PARSE_EQUALS_PRINT(s) REQUIRE( parseAlarm(s).toString() == s);
#define TEST_PARSE_EQUALS(s, r) REQUIRE( parseAlarm(s).toString() == r);

  TEST_PARSE_EQUALS_PRINT("alarm 06:15;");
  TEST_PARSE_EQUALS("alarm 06:15 every sun mon tue wed thu fri sat;", "alarm 06:15;");
  TEST_PARSE_EQUALS_PRINT("alarm 06:15 every sun mon tue wed thu fri;");
  TEST_PARSE_EQUALS_PRINT("alarm 06:15 every wed;");
  TEST_PARSE_EQUALS("alarm 06:15 every sun mon wed sun tue thu wed sun fri;", "alarm 06:15 every sun mon tue wed thu fri;");
  TEST_PARSE_EQUALS_PRINT("alarm 06:15 from 2018-05-26;");
  TEST_PARSE_EQUALS_PRINT("alarm 06:15 until 2018-05-30;");
  TEST_PARSE_EQUALS_PRINT("alarm 06:15 but on 2018-05-10 skip but on 2018-05-11 06:30;");
//  TEST_PARSE_EQUALS_PRINT("alarm 15:00 except on \"prod days\" skip;");
//  TEST_PARSE_EQUALS_PRINT("alarm 16:00 on \"prod days\";");

}

#define SET_START_TIME_AND_ALARM(start_day, start_time, alarm_string)         \
		time_point_secs_t t = sys_days(start_day) + start_time; \
		Alarm alarm = parseAlarm(alarm_string); \
		optional<time_point_secs_t> n = t; \
		time_point_secs_t exp;


#define SET_START_DAY_AND_ALARM(start_day, alarm_string)    SET_START_TIME_AND_ALARM(start_day, 0h, alarm_string)

#define CHECK_FIRST_OCCURRENCE(from_midnight)        \
		n=alarm.nextOccurrence(t);      \
		time_point_secs_t midnight = floor<days>(t); \
		exp=midnight+from_midnight;    \
		CHECK(n == exp);

#define CHECK_NEXT_OCCURRENCE_AT(alarm_day, alarm_time)        \
		n=alarm.nextOccurrence(t);      \
		exp=sys_days(alarm_day)+alarm_time;    \
		CHECK(n == exp);

#define CHECK_NEXT_OCCURRENCE_DIST(gap) \
		exp+=gap; \
		n=alarm.nextOccurrence(n.value()); \
		CHECK(n == exp);

#define CHECK_NO_MORE_OCCURRENCES \
                n=alarm.nextOccurrence(n.value()); \
                CHECK(n == empty);


TEST_CASE("Alarm finds next time", "[alarm]") {
  const optional<time_point_secs_t> empty = {};

  SECTION("for a daily alarm") {
    SET_START_DAY_AND_ALARM(2018_y/1/1, "alarm 06:15;");

    CHECK_FIRST_OCCURRENCE(6h+15min);
    for (int i=0; i<15; ++i) {
	CHECK_NEXT_OCCURRENCE_DIST(24h);
    }
  }

  SECTION("for a daily alarm, starting right after alarm triggered") {
    SET_START_TIME_AND_ALARM(2018_y/1/1, 6h + 16min, "alarm 06:15;");

    CHECK_FIRST_OCCURRENCE(24h+6h+15min);
    for (int i=0; i<15; ++i) {
	CHECK_NEXT_OCCURRENCE_DIST(24h);
    }
  }

  SECTION("for a weekly alarm") {
    SET_START_DAY_AND_ALARM(2018_y/1/1, "alarm 06:15 every wed;");

    CHECK_FIRST_OCCURRENCE(48h+6h+15min);
    for (int i=0; i<15; ++i) {
	CHECK_NEXT_OCCURRENCE_DIST(7*24h);
    }
  }


  SECTION("for a bi-weekly alarm") {
    SET_START_DAY_AND_ALARM(2018_y/1/1, "alarm 17:59 every sat sun;");

    CHECK_FIRST_OCCURRENCE(5*24h+17h+59min);
    for (int i=0; i<15; ++i) {
	CHECK_NEXT_OCCURRENCE_DIST(24h);
	CHECK_NEXT_OCCURRENCE_DIST(6*24h);
    }
  }

  SECTION("for an expired alarm") {
    SET_START_DAY_AND_ALARM(2018_y/1/1, "alarm 17:59 until 2017-12-31;");

    CHECK_NO_MORE_OCCURRENCES;
  }


  SECTION("for a single day alarm") {
    SET_START_DAY_AND_ALARM(2018_y/1/1, "alarm 17:59 from 2018-10-10 until 2018-10-10;");

    CHECK_NEXT_OCCURRENCE_AT(2018_y/10/10, 17h+59min);
    CHECK_NO_MORE_OCCURRENCES;
  }

  SECTION("for an alarm with start and end time") {
    SET_START_DAY_AND_ALARM(2018_y/1/1, "alarm 17:59 from 2018-10-10 until 2018-10-11;");

    CHECK_NEXT_OCCURRENCE_AT(2018_y/10/10, 17h+59min);
    CHECK_NEXT_OCCURRENCE_DIST(24h);
    CHECK_NO_MORE_OCCURRENCES;
  }

  SECTION("for an alarm with a skip exception") {
    SET_START_DAY_AND_ALARM(2018_y/1/1, "alarm 06:15 from 2018-05-08 until 2018-10-13 but on 2018-05-10 skip;");

    CHECK_NEXT_OCCURRENCE_AT(2018_y/5/8, 6h+15min);
    CHECK_NEXT_OCCURRENCE_DIST(24h);
    CHECK_NEXT_OCCURRENCE_DIST(48h);
    CHECK_NEXT_OCCURRENCE_DIST(24h);
    CHECK_NEXT_OCCURRENCE_DIST(24h);
    CHECK_NEXT_OCCURRENCE_DIST(24h);
    CHECK_NO_MORE_OCCURRENCES;
  }
}
