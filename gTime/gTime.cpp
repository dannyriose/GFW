#include "gTime.h"
#include <time.h>
#define YEAR_SECONDS 31556952
#define DAY_SECONDS 86400
using namespace gfw;
//convert to struct tm
time_t gTimeToTime(const gTime &t){
    time_t time,daymonth=0;
    gs32 months[]={0,31,28,31,30,31,30,31,31,30,31,30,31};
    time=t.second();
    time+=t.minute() * 60;
    time+=t.hour() * 3600;
    time+=t.day() * DAY_SECONDS;
    for(int i=1;i<t.month();i++){
        daymonth+=months[i];
    }
    time+=daymonth * DAY_SECONDS;
    time+=(t.year() - 1900) * YEAR_SECONDS;
    return time;
}
void TimeTogTime(gTime &out,time_t time){
    int sum=0;
    gs32 months[]={0,31,28,31,30,31,30,31,31,30,31,30,31};
    int years = (int)(time / YEAR_SECONDS);
    time= time % YEAR_SECONDS;
    int days=(time / DAY_SECONDS);
    time= time % DAY_SECONDS;
    int month=0;
    for(int i=1;i<12;i++){
        sum+=months[i];
        if(days==sum)
            month=i;
    }
    int day=time / DAY_SECONDS;
    time=time % DAY_SECONDS;
    int hour=time / 3600;
    time=time % 3600;
    int minute=time / 60;
    int second=time % 60;

    out.setYear(years + 1900);
    out.setMonth(month);
    out.setDay(day);
    out.setMinute(minute);
    out.setHour(hour);
    out.setSecond(second);

}

//function for operators
void gTimeArith(gTime &t,const gTime &t2,int op){
    time_t time1=gTimeToTime(t);
    time_t time2=gTimeToTime(t2);


    switch(op){
    case 0:time1=time1 + time2;break;
    case 1:time1=time1 - time2;break;
    }
    TimeTogTime(t,time1);


}

bool gTimeBool(const gTime &t,const gTime &t2,int op){
    time_t time1=gTimeToTime(t);
    time_t time2=gTimeToTime(t2);

    switch(op){
    case 0:return time1 == time2;
    case 1:return time1 != time2;
    case 2:return time1 < time2;
    case 3:return time1 > time2;
    case 4:return time1 <= time2;
    case 5:return time1 >= time2;
    }
    return false;
}

gTime::gTime():m_second(0),
    m_minute(0),
    m_hour(0),
    m_day(0),
    m_weekDay(0),
    m_month(0),
    m_year(0){

}
gTime::gTime(const gTime &other):m_second(other.m_second),
    m_minute(other.m_minute),
    m_hour(other.m_hour),
    m_day(other.m_day),
    m_weekDay(other.m_weekDay),
    m_month(other.m_month),
    m_year(other.m_year){

}
gTime::gTime(gs32 _year, gs32 _month, gs32 _day):m_second(0),
    m_minute(0),
    m_hour(0),
    m_day(_day),
    m_weekDay(0),
    m_month(_month),
    m_year(_year){


}
gTime &gTime::operator=(const gTime &other){
    m_second=other.m_second;
    m_minute=other.m_minute;
    m_hour=other.m_hour;
    m_day=other.m_day;
    m_weekDay=other.m_weekDay;
    m_month=other.m_month;
    m_year=other.m_year;
    return *this;
}

gTime::~gTime(){

}

gTime gTime::operator + (const gTime &other) const{
    gTime t=*this;
    gTimeArith(t,other,0);
    return t;
}
gTime &gTime::operator +=(const gTime &other){
    gTimeArith(*this,other,0);
    return *this;
}
gTime gTime::operator - (const gTime &other) const{
    gTime t=*this;
    gTimeArith(t,other,1);
    return t;
}
gTime &gTime::operator -=(const gTime &other){
    gTimeArith(*this,other,1);
    return *this;
}
bool gTime::operator == (const gTime &other) const{
    return gTimeBool(*this,other,0);
}
bool gTime::operator != (const gTime &other) const{
    return gTimeBool(*this,other,1);
}
bool gTime::operator < (const gTime &other) const{
    return gTimeBool(*this,other,2);
}
bool gTime::operator > (const gTime &other) const{
    return gTimeBool(*this,other,3);
}
bool gTime::operator <= (const gTime &other) const{
    return gTimeBool(*this,other,4);
}
bool gTime::operator >= (const gTime &other) const{
    return gTimeBool(*this,other,5);
}
void gTime::currentTime(){
    time_t t=time(0);
    struct tm *tt=localtime(&t);

    m_second=tt->tm_sec;
    m_minute=tt->tm_min;
    m_hour=tt->tm_hour;
    m_day=tt->tm_mday;
    m_month=tt->tm_mon;
    m_year=tt->tm_year;

}
