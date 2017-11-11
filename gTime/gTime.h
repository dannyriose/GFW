#ifndef GTIME_H
#define GTIME_H

#include <gfw_global.h>

namespace gfw{
    class SHARED_GFW gTime{
    public:
        gTime();
        gTime(const gTime &other);
        gTime(gs32 _year,gs32 _month,gs32 _day);
        gTime &operator = (const gTime &other);
        virtual ~gTime();
        //! Gets the current time
        void currentTime();
        inline void setYear(gs32 _year){
            m_year=_year;
        }
        inline gs32 year() const{
            return m_year;
        }

        inline void setMonth(gs32 _month){
            m_month=_month;
        }
        inline gs32 month() const{
            return m_month;
        }
        inline void setWeekDay(gs32 _weekDay){
            m_weekDay=_weekDay;
        }
        inline gs32 weekDay() const{
            return m_weekDay;
        }
        inline void setDay(gs32 _day){
            m_day=_day;
        }
        inline gs32 day() const{
            return m_day;
        }
        inline void setHour(gs32 _hour){
            m_hour=_hour;
        }
        inline gs32 hour() const{
            return m_hour;
        }
        inline void setMinute(gs32 _minute){
            m_minute=_minute;
        }
        inline gs32 minute() const{
            return m_minute;
        }
        inline void setSecond(gs32 _second){
            m_second=_second;
        }
        inline gs32 second() const{
            return m_second;
        }
        gTime &operator+=(const gTime &other);
        gTime operator + (const gTime &other) const;
        gTime &operator-=(const gTime &other);
        gTime operator - (const gTime &other) const;

        bool operator == (const gTime &other) const;
        bool operator != (const gTime &other) const;
        bool operator < (const gTime &other) const;
        bool operator > (const gTime &other) const;
        bool operator >=(const gTime &other) const;
        bool operator <=(const gTime &other) const;
    protected:
        gs32 m_second;
        gs32 m_minute;
        gs32 m_hour;
        gs32 m_day;
        gs32 m_weekDay;
        gs32 m_month;
        gs32 m_year;
    };
}
#endif // GTIME_H
