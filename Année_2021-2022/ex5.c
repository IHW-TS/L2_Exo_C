#include <stdio.h>
#include <string.h>
#include <time.h>

typedef enum
{
    MON = 1,
    TUE = 2,
    WED = 3,
    THU = 4,
    FRI = 5,
    SAT = 6,
    SUN = 7
} dayname;

typedef enum
{
    JAN = 1,
    FEB = 2,
    MAR = 3,
    APR = 4,
    MAY = 5,
    JUN = 6,
    JUL = 7,
    AUG = 8,
    SEP = 9,
    OCT = 10,
    NOV = 11,
    DEC = 12
} monthname;

typedef struct
{
    dayname weekday : 3;
    unsigned int day : 5;
    monthname month : 4;
    int year : 20;
} date;

char *dayname_str(dayname day)
{

    switch (day)
    {
    case 1:
        printf("Monday ");
        break;
    case 2:
        printf("Tuesday ");
        break;
    case 3:
        printf("Wednesday ");
        break;
    case 4:
        printf("Thursday ");
        break;
    case 5:
        printf("Friday ");
        break;
    case 6:
        printf("Saturday ");
        break;
    case 7:
        printf("Sunday ");
        break;
    default:
        printf("Nombre invalide ! Choisissez un nombre entre 1 et 7 !");
    }

    return 0;
}

char *monthname_str(monthname month)
{

    switch (month)
    {
    case 1:
        printf("January ");
        break;
    case 2:
        printf("February ");
        break;
    case 3:
        printf("March ");
        break;
    case 4:
        printf("April ");
        break;
    case 5:
        printf("May ");
        break;
    case 6:
        printf("June ");
        break;
    case 7:
        printf("July ");
        break;
    case 8:
        printf("August ");
        break;
    case 9:
        printf("September ");
        break;
    case 10:
        printf("October ");
        break;
    case 11:
        printf("November ");
        break;
    case 12:
        printf("December ");
        break;
    default:
        printf("Nombre invalide ! Choisissez un nombre entre 1 et 12 !");
    }

    return 0;
}

int leapyear(unsigned int year)
{

    if (year % 400 == 0)
    {
        printf("%d is a leap year.", year);
    }
    else if (year % 100 == 0)
    {
        printf("%d is not a leap year.", year);
    }
    else if (year % 4 == 0)
    {
        printf("%d is a leap year.", year);
    }
    else
    {
        printf("%d is not a leap year.", year);
    }
    return 0;
}


date from_time (time_t when){


 
   return t;
}


int main(void)
{

    dayname_str(MON);
    monthname_str(FEB);
    leapyear(2000);
    from_time();
}