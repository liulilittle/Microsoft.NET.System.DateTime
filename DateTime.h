#pragma once

#include <string>

#ifndef NULL
#define NULL 0
#endif

namespace Ppp {
    //////////////////////////////////////////////////////////////////////////
    /*
     * 时间计量单位为：千万分之一秒，即一个周期100纳秒时。
     */
    enum DayOfWeek {
        DayOfWeek_Sunday,
        DayOfWeek_Monday,
        DayOfWeek_Tuesday,
        DayOfWeek_Wednesday,
        DayOfWeek_Thursday,
        DayOfWeek_Friday,
        DayOfWeek_Saturday
    };
    
    struct DateTime {
    private:
        long long               m_ticks;
    
    public:
        inline DateTime() noexcept {
            m_ticks = 0;
        }
        inline DateTime(long long ticks) noexcept {
            m_ticks = ticks;
        };
        inline DateTime(int year, int month, int day = 0, int hour = 0, int minutes = 0, int seconds = 0, int millisecond = 0) noexcept {
            this->m_ticks = 0;
            DateTime& dateTime = *this;
            dateTime = dateTime.AddYears(year - 1);
            dateTime = dateTime.AddMonths(month - 1);
            dateTime = dateTime.AddDays(day - 1);
            dateTime = dateTime.AddHours(hour);
            dateTime = dateTime.AddMinutes(minutes);
            dateTime = dateTime.AddSeconds(seconds);
            dateTime = dateTime.AddMilliseconds(millisecond);
        }
    
    public:
        inline long long        Ticks() noexcept {
            return m_ticks & 0x3FFFFFFFFFFFFFFF;
        }
        inline int              Millisecond() noexcept {
            return (int)(Ticks() / 10000 % 1000);
        }
        inline int              Second() noexcept {
            return (int)(Ticks() / 10000000 % 60);
        }
        inline int              Minute() noexcept {
            return (int)(Ticks() / 600000000 % 60);
        }
        inline int              Hour() noexcept {
            return (int)(Ticks() / 36000000000LL % 24);
        }
        inline int              Microseconds() noexcept {
            return (int)(Ticks() / 10LL % 1000);
        }
    
    private:
        static int*             DaysToMonth366() noexcept {
            static int buf[] = {
                0,
                31,
                60,
                91,
                121,
                152,
                182,
                213,
                244,
                274,
                305,
                335,
                366
            };
            return buf;
        }
        static int*             DaysToMonth365() noexcept {
            static int buf[] = {
                0,
                31,
                59,
                90,
                120,
                151,
                181,
                212,
                243,
                273,
                304,
                334,
                365
            };
            return buf;
        }
        inline int              GetDatePart(int part) noexcept {
            long long internalTicks = Ticks();
            int num = (int)(internalTicks / 864000000000LL);
            int num2 = num / 146097;
            num -= num2 * 146097;
            int num3 = num / 36524;
            if (num3 == 4)
            {
                num3 = 3;
            }
            num -= num3 * 36524;
            int num4 = num / 1461;
            num -= num4 * 1461;
            int num5 = num / 365;
            if (num5 == 4)
            {
                num5 = 3;
            }
            if (part == 0)
            {
                return num2 * 400 + num3 * 100 + num4 * 4 + num5 + 1;
            }
            num -= num5 * 365;
            if (part == 1)
            {
                return num + 1;
            }
            int* array = (num5 == 3 && (num4 != 24 || num3 == 3)) ? DaysToMonth366() : DaysToMonth365();
            int i = 0;
            for (i = num >> 6; num >= array[i]; i++)
            {
            }
            if (part == 2)
            {
                return i;
            }
            return num - array[i - 1] + 1;
        }
    
    public:
        inline int              Day() noexcept {
            return GetDatePart(3);
        }
        inline int              DayOfYear() noexcept {
            return GetDatePart(1);
        }
        inline DayOfWeek        DayOfWeeks() noexcept {
            return (DayOfWeek)((Ticks() / 864000000000LL + 1) % 7);
        }
        inline int              Month() noexcept {
            return GetDatePart(2);
        }
        inline int              Year() noexcept {
            return GetDatePart(0);
        }
    
    public:
        inline DateTime         Date() noexcept {
            long long internalTicks = Ticks();
            return DateTime((unsigned long long)(internalTicks - internalTicks % 864000000000LL));
        }
        DateTime                ToUtc() noexcept;
        DateTime                ToLocal() noexcept;
    
    public:
        inline static int       Compare(DateTime t1, DateTime t2) noexcept {
            long long internalTicks = t1.Ticks();
            long long internalTicks2 = t2.Ticks();
            if (internalTicks > internalTicks2) {
                return 1;
            }
            if (internalTicks < internalTicks2) {
                return -1;
            }
            return 0;
        }
        inline int              CompareTo(DateTime value) noexcept {
            return Compare(*this, value);
        }
        inline bool             Equals(DateTime value) noexcept {
            return Ticks() == value.Ticks();
        }
        inline static bool      Equals(DateTime t1, DateTime t2) noexcept {
            return t1.Ticks() == t2.Ticks();
        }
    
    public:
        inline static bool      IsLeapYear(int year) noexcept {
            if (year < 1 || year > 9999) {
                return false;
            }
            if (year % 4 == 0) {
                if (year % 100 == 0) {
                    return year % 400 == 0;
                }
                return true;
            }
            return false;
        }
        inline static int       DaysInMonth(int year, int month) noexcept {
            if (month < 1 || month > 12) {
                return -1;
            }
            int* array = IsLeapYear(year) ? DaysToMonth366() : DaysToMonth365();
            return array[month] - array[month - 1];
        }
    
    public:
        inline DateTime         AddTicks(long long value) noexcept {
            long internalTicks = Ticks();
            if (value > 3155378975999999999LL - internalTicks) {
                value = 3155378975999999999LL - internalTicks;
            }
            if (value < -internalTicks) {
                value = -internalTicks;
            }
            return DateTime((unsigned long long)(internalTicks + value));
        }
        inline DateTime         Add(long double value, int scale) noexcept {
            long long num = (long long)(value * (long double)scale + ((value >= 0.0) ? 0.5 : (-0.5)));
            if (num <= -315537897600000LL) {
                num = -315537897600000LL;
            }
            if (num >= 315537897600000LL) {
                num = 315537897600000LL;
            }
            return AddTicks(num * 10000);
        }
        inline DateTime         AddDays(long double value) noexcept {
            return Add(value, 86400000);
        }
        inline DateTime         AddHours(long double value) noexcept {
            return Add(value, 3600000);
        }
        inline DateTime         AddSeconds(long double value) noexcept {
            return Add(value, 1000);
        }
        inline DateTime         AddMinutes(long double value) noexcept {
            return Add(value, 60000);
        }
        inline DateTime         AddMilliseconds(long double value) noexcept {
            return Add(value, 1);
        }
    
    private:
        inline long             DateToTicks(int year, int month, int day) noexcept {
            if (year >= 1 && year <= 9999 && month >= 1 && month <= 12) {
                int* array = IsLeapYear(year) ? DaysToMonth366() : DaysToMonth365();
                if (day >= 1 && day <= array[month] - array[month - 1]) {
                    int num = year - 1;
                    int num2 = num * 365 + num / 4 - num / 100 + num / 400 + array[month - 1] + day - 1;
                    return num2 * 864000000000LL;
                }
            }
            return 0;
        }
        inline void             GetDatePart(int& year, int& month, int& day) noexcept {
            long long internalTicks = Ticks();
            int num = (int)(internalTicks / 864000000000LL);
            int num2 = num / 146097;
            num -= num2 * 146097;
            int num3 = num / 36524;
            if (num3 == 4) {
                num3 = 3;
            }
            num -= num3 * 36524;
            int num4 = num / 1461;
            num -= num4 * 1461;
            int num5 = num / 365;
            if (num5 == 4) {
                num5 = 3;
            }
            year = num2 * 400 + num3 * 100 + num4 * 4 + num5 + 1;
            num -= num5 * 365;
            int* array = (num5 == 3 && (num4 != 24 || num3 == 3)) ? DaysToMonth366() : DaysToMonth365();
            int i = 0;
            for (i = (num >> 5) + 1; num >= array[i]; i++)
            {
            }
            month = i;
            day = num - array[i - 1] + 1;
        }
    
    public:
        inline DateTime         AddMonths(int months) noexcept {
            if (months < -120000 || months > 120000) {
                return *this;
            }
            int year;
            int month;
            int day;
            GetDatePart(year, month, day);
            int num = month - 1 + months;
            if (num >= 0) {
                month = num % 12 + 1;
                year += num / 12;
            }
            else {
                month = 12 + (num + 1) % 12;
                year += (num - 11) / 12;
            }
            if (year < 1 || year > 9999) {
                return *this;
            }
            int num2 = DaysInMonth(year, month);
            if (day > num2) {
                day = num2;
            }
            return DateTime((unsigned long long)((DateToTicks(year, month, day) + Ticks() % 864000000000LL)));
        }
        inline DateTime         Subtract(DateTime value) noexcept {
            return DateTime(Ticks() - value.Ticks());
        }
        inline DateTime         AddYears(int year) noexcept {
            if (year < -10000 || year > 10000)
            {
                return *this;
            }
            return AddMonths(year * 12);
        }
    
    public:
        inline long double           TotalDays() noexcept {
            return (long double)Ticks() * 1.1574074074074074E-12;
        }
        inline long double           TotalHours() noexcept {
            return (long double)Ticks() * 2.7777777777777777E-11;
        }
        inline long double           TotalMilliseconds() noexcept {
            long double num = (long double)Ticks() * 0.0001;
            if (num > 922337203685477.0) {
                return 922337203685477.0;
            }
            if (num < -922337203685477.0) {
                return -922337203685477.0;
            }
            return num;
        }
        inline long double           TotalMinutes() noexcept {
            return (long double)Ticks() * 1.6666666666666667E-09;
        }
        inline long double           TotalSeconds() noexcept {
            return (long double)Ticks() * 1E-07;
        }
    
    public:
        static DateTime         Now() noexcept;
        static DateTime         UtcNow() noexcept;
        static int              GetGMTOffset(bool abs = false) noexcept;
        inline static DateTime  MinValue() noexcept {
            return DateTime(0);
        }
        inline static DateTime  MaxValue() noexcept {
            return DateTime(3155378975999999999LL);
        }
    
    public:
        inline static bool          TryParse(const char* s, DateTime& dateTime) noexcept {
            return TryParse(s, -1, dateTime);
        }
        static bool                 TryParse(const char* s, int len, DateTime& dateTime) noexcept;
        inline static bool          TryParse(const std::string& s, DateTime& dateTime) noexcept {
            std::string& sx = const_cast<std::string&>(s);
            return TryParse(sx.c_str(), sx.length(), dateTime);
        }
        inline static DateTime      Parse(const char* s) noexcept {
            return Parse(s, -1);
        }
        inline static DateTime      Parse(const char* s, int len) noexcept {
            DateTime dateTime;
            TryParse(s, len, dateTime);
            return dateTime;
        }
        inline static DateTime      Parse(const std::string& s) noexcept {
            DateTime dateTime;
            TryParse(s, dateTime);
            return dateTime;
        }
    
    public:
        inline std::string          ToString(const std::string& s) noexcept {
            std::string& sx = const_cast<std::string&>(s);
            return ToString(sx.data(), sx.length());
        }
        inline std::string          ToString(const char* format) noexcept {
            return ToString(format, -1);
        }
        std::string                 ToString(const char* format, int len) noexcept;
    
    public:
        inline bool             operator==(const DateTime& right) noexcept {
            return Ticks() == const_cast<DateTime&>(right).Ticks();
        }
        inline bool             operator!=(const DateTime& right) noexcept {
            return Ticks() != const_cast<DateTime&>(right).Ticks();
        }
        inline bool             operator>(const DateTime& right) noexcept {
            return Ticks() > const_cast<DateTime&>(right).Ticks();
        }
        inline bool             operator<(const DateTime& right) noexcept {
            return Ticks() < const_cast<DateTime&>(right).Ticks();
        }
        inline bool             operator>=(const DateTime& right) noexcept {
            return Ticks() >= const_cast<DateTime&>(right).Ticks();
        }
        inline bool             operator<=(const DateTime& right) noexcept {
            return Ticks() <= const_cast<DateTime&>(right).Ticks();
        }
        inline DateTime         operator+(const DateTime& right) noexcept {
            long long ticks = const_cast<DateTime&>(right).Ticks();
            return this->AddTicks(ticks);
        }
        inline DateTime         operator-(const DateTime& right) noexcept {
            return this->Subtract(right);
        }
    };
}