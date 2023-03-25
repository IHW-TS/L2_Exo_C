#include <stdio.h>
#include <time.h>

typedef enum {MON = 1, TUE = 2, WED = 3, THU = 4, FRI = 5, SAT = 6, SUN = 7} dayname;

typedef enum {JAN = 1, FEB = 2, MAR = 3, APR = 4, MAY = 5, JUN = 6, JUL = 7,
AUG = 8, SEP = 9, OCT = 10, NOV = 11, DEC = 12} monthname;

typedef struct {
    dayname weekday : 3;
    unsigned int day : 5;
    monthname month : 4;
    int year : 20;
} date;

const char* dayname_str(dayname day) {
    const char* names[] = {"", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday", "Sunday"};
    return names[day];
}

const char* monthname_str(monthname month) {
    const char* names[] = {"", "January", "February", "March", "April", "May", "June",
                           "July", "August", "September", "October", "November", "December"};
    return names[month];
}

dayname weekday(time_t when) {
    return ((when / (60 * 60 * 24)) + 4) % 7 + 1;
}

int leapyear(unsigned int year) {
    return (year % 4 == 0 && year % 100 != 0) || (year % 400 == 0);
}

date from_time(time_t when) {
    const int days_in_month[] = {0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
    int days = when / (60 * 60 * 24);

    date result;
    result.weekday = (days + 4) % 7 + 1;
    result.year = 1970;

    while (1) {
        int days_in_year = leapyear(result.year) ? 366 : 365;
        if (days < days_in_year) {
            break;
        }
        days -= days_in_year;
        result.year++;
    }

    result.month = 1;
    for (; result.month <= 12; result.month++) {
        int days_in_current_month = days_in_month[result.month] + (result.month == 2 && leapyear(result.year));
        if (days < days_in_current_month) {
            break;
        }
        days -= days_in_current_month;
    }

    result.day = days + 1;
    return result;
}

int main() {
    time_t current_time = time(NULL);
    date today = from_time(current_time);
    printf("Today is %s, %s %d, %d\n", dayname_str(today.weekday), monthname_str(today.month), today.day, today.year);
    return 0;
}
