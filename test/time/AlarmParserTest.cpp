/*
 * AlarmParserTest.cpp
 *
 *  Created on: Jan 2, 2018
 *      Author: Hans
 */

#include "catch.hpp"
#include "AlarmParser.h"

TEST_CASE( "Alarm is parsed", "[parsealarm]") {

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


TEST_CASE("Alarm finds next time", "[alarm]") {
  time_point_t t = sys_days(2018_y/1/1); // a Monday, at midnight

  SECTION("for a daily alarm") {
    Alarm alarm = parseAlarm("alarm 06:15;");

    auto n=alarm.nextOccurrence(t);
    auto exp=t+6h+15min;
    REQUIRE(n == exp);
    for (int i=0; i<15; ++i) {

    }

  }
}
