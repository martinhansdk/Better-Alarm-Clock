/*
 * Convert a string to an integer.
 */

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "AlarmParser.h"


%%{ 
	machine atoi;
	write data;
}%%

Alarm parseAlarm(const char *str )
{
	const char *p = str, *pe = str + strlen( str );
	int cs;
	Alarm alarm;

	const int INTS = 3;
	int intvals[INTS] = { };
	date_t day;
	alarm_time_t time;

	%%{
		action int_add_digit { 
			intvals[0] = intvals[0] * 10 + (fc - '0');
        }

	    action new_int {
	    	for(int i=INTS-1; i>0 ; i--) {
	    		intvals[i]=intvals[i-1];
	    	}	    		
	        intvals[0] = 0;
	    }

	    action set_time {
	    	alarm.time = time;
	    }

		action set_first_day {
			alarm.first_day = day;
		}
		
		action set_last_day {
			alarm.last_day = day;
		}

		action save_time {
			time = alarm_time_t(minutes(intvals[1]*60+intvals[0]));
		}

		action save_date {
			day = sys_days(year{intvals[2]}/intvals[1]/intvals[0]);
		}

	    integer = ( digit @int_add_digit )+ >new_int;
	    time_of_day = (integer ':' integer) %save_time;
	    date = (integer '-' integer '-' integer) %save_date;
	    
	    weekdays = space+ 'every' %{
	                          alarm.weekdays=0;
                            }                            
	               (
	                 space+ 'mon' %{ alarm.weekdays |= (1 << Alarm::MON); } |
	                 space+ 'tue' %{ alarm.weekdays |= (1 << Alarm::TUE); } |
	                 space+ 'wed' %{ alarm.weekdays |= (1 << Alarm::WED); } | 
	                 space+ 'thu' %{ alarm.weekdays |= (1 << Alarm::THU); } |
	                 space+ 'fri' %{ alarm.weekdays |= (1 << Alarm::FRI); } |
	                 space+ 'sat' %{ alarm.weekdays |= (1 << Alarm::SAT); } |
	                 space+ 'sun' %{ alarm.weekdays |= (1 << Alarm::SUN); }
	               )+;
	     	     
	     first_day = space+ 'from' space+ date %set_first_day;
	     last_day = space+ 'until' space+ date %set_last_day;
	     
	     exception = space+ 'but on' space+ date space+ (
	                                                       'skip'       %{ alarm.addException(AlarmException(day)); }
	                                                     | time_of_day  %{ alarm.addException(AlarmException(day, time)); }
	                                                    ); 
	     exceptions = exception+;
	     
		main := 
			'alarm' space+ time_of_day %set_time first_day? last_day? weekdays? exceptions? space* ';';

		# Initialize and execute.
		write init;
		write exec; 
	}%%

	if ( cs < %%{ write first_final; }%% )
		fprintf( stderr, "parseAlarm: there was an error: %d\n", cs );

	return alarm;
};