/*
 * AlarmParserTest.cpp
 *
 *  Created on: Jan 2, 2018
 *      Author: Hans
 */

#include "catch.hpp"
#include "AlarmParser.h"

unsigned int Factorial( unsigned int number ) {
    return number <= 1 ? number : Factorial(number-1)*number;
}

TEST_CASE( "Factorials are computed", "[factorial]" ) {
    REQUIRE( Factorial(1) == 1 );
    REQUIRE( Factorial(2) == 2 );
    REQUIRE( Factorial(3) == 6 );
    REQUIRE( Factorial(10) == 3628800 );
}

TEST_CASE( "Int is parsed", "[parseint]") {
  REQUIRE( alarmAtoi("6") == 6) ;
}
