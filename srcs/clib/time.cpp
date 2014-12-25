/*
    Copyright 2014 Matthew Fosdick

    Licensed under the Apache License, Version 2.0 (the "License");
    you may not use this file except in compliance with the License.
    You may obtain a copy of the License at

     http://www.apache.org/licenses/LICENSE-2.0

    Unless required by applicable law or agreed to in writing, software
    distributed under the License is distributed on an "AS IS" BASIS,
    WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
    See the License for the specific language governing permissions and
    limitations under the License.
*/
#include <time.h>
#include <string.h>
#include <conv.hpp>

bool is_leap(int year);
void set_mon(bool leap, time_t yday, int *mon, int *mday);
time_t subtract_leap(time_t time);
struct tm *gmtime(const time_t *time) {
	static tm *ptr;
	return gmtime_r(time,ptr);
}
#define LAZY_EPOCH 1388534400L
struct tm *gmtime_r(const time_t *t, struct tm *ptr) {
	memset(ptr,0,sizeof(tm));
	time_t time=subtract_leap(*t);
	time_t days=time/86400;
	time_t rem=time%86400;
	if(days<0) {
		return ptr;
	}
	time_t years=1970;
	while(true) {
		int day_year=is_leap(years)?366:365;
		if(days>day_year) {
			days-=day_year;
			years++;
		} else {
			break;
		}
	}
	ptr->tm_year=years-1900;
	ptr->tm_yday=days;
	ptr->tm_wday=0;
	bool leap=is_leap(years);
	ptr->tm_hour=rem/3600;
	rem=rem%3600;
	ptr->tm_min=rem/60;
	ptr->tm_sec=rem%60;
	set_mon(leap,days,&ptr->tm_mon,&ptr->tm_mday);
	return ptr;
}


bool is_leap(int year) {
	return (!(year%4)&&(year%100))?true:!(year%400);
}

time_t subtract_leap(time_t time) {
	time_t sub=0;
	if(time>=78796800L) {//1972-07-01
		sub++;
	}
	if(time>=94694400L) {//1973-01-01
		sub++;
	}
	if(time>=126230400L) {//1974-01-01
		sub++;
	}
	if(time>=157766400L) {//1975-01-01
		sub++;
	}
	if(time>=189302400L) {//1976-01-01
		sub++;
	}
	if(time>=220924800L) {//1977-01-01
		sub++;
	}
	if(time>=252460800L) {//1978-01-01
		sub++;
	}
	if(time>=283996800L) {//1979-01-01
		sub++;
	}
	if(time>=315532800L) {//1980-01-01
		sub++;
	}
	if(time>=362793600L) {//1981-07-01
		sub++;
	}
	if(time>=394329600L) {//1982-07-01
		sub++;
	}
	if(time>=425865600L) {//1983-07-01
		sub++;
	}
	if(time>=489024000L) {//1985-07-01
		sub++;
	}
	if(time>=567993600L) {//1988-01-01
		sub++;
	}
	if(time>=631152000L) {//1990-01-01
		sub++;
	}
	if(time>=662688000L) {//1991-01-01
		sub++;
	}
	if(time>=709948800L) {//1992-07-01
		sub++;
	}
	if(time>=741484800L) {//1993-07-01
		sub++;
	}
	if(time>=773020800L) {//1994-07-01
		sub++;
	}
	if(time>=820454400L) {//1996-01-01
		sub++;
	}
	if(time>=867715200L) {//1997-07-01
		sub++;
	}
	if(time>=915148800L) {//1999-01-01
		sub++;
	}
	if(time>=1136073600L) {//2006-01-01
		sub++;
	}
	if(time>=1230768000L) {//2009-01-01
		sub++;
	}
	if(time>=1341100800L) {//2012-07-01
		sub++;
	}
	return time-sub;
}
void set_mon(bool leap, time_t yday, int *mon, int *mday) {
	yday++; //this algorithm assumes Jan 1 is day 1 when really its day 0
	int mon_list[] = {31,28,31,30,31,30,31,31,30,31,30,31};
	if(leap) {
		mon_list[1]=29;
	}
	for(int i=0; i<12; i++) {
		if(yday>mon_list[i]) {
			yday-=mon_list[i];
		} else {
			*mon=i;
			*mday=yday;
			return;
		}
	}
}
char ptra[20];
char tmpa[5];
char *asctime(const tm *timeptr) {
	//YYYY-MM-DD HH:MM:SS AP
	memset(&ptra,0,23);
	std::numtostr(timeptr->tm_year+1900,tmpa,10,false,4);
	strcat(ptra,tmpa);

	strcat(ptra,"-");
	std::numtostr(timeptr->tm_mon+1,tmpa,10,false,2);
	strcat(ptra,tmpa);

	strcat(ptra,"-");
	std::numtostr(timeptr->tm_mday,tmpa,10,false,2);
	strcat(ptra,tmpa);

	strcat(ptra," ");
	std::numtostr(timeptr->tm_hour,tmpa,10,false,2);
	strcat(ptra,tmpa);

	strcat(ptra,":");
	std::numtostr(timeptr->tm_min,tmpa,10,false,2);
	strcat(ptra,tmpa);

	strcat(ptra,":");
	std::numtostr(timeptr->tm_sec,tmpa,10,false,2);
	strcat(ptra,tmpa);
	return ptra;
}