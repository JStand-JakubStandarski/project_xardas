/*
 * Author: Jakub Standarski
 * Email: jstand.jakub.standarski@gmail.com
 *
 * Date: 08.03.2022
 */

#ifndef RTC_H
    #define RTC_H

/*****************************************************************************/
/* PUBLIC ENUMS */
/*****************************************************************************/

enum rtc_month {
    RTC_MONTH_JANUARY = 1,
    RTC_MONTH_FEBRUARY,
    RTC_MONTH_MARCH,
    RTC_MONTH_APRIL,
    RTC_MONTH_MAY,
    RTC_MONTH_JUNE,
    RTC_MONTH_JULY,
    RTC_MONTH_AUGUST,
    RTC_MONTH_SEPTEMBER,
    RTC_MONTH_OCTOBER,
    RTC_MONTH_NOVEMBER,
    RTC_MONTH_DECEMBER
};



enum rtc_weekday {
    RTC_WEEKDAY_MONDAY = 1,
    RTC_WEEKDAY_TUESDAY,
    RTC_WEEKDAY_WEDNESDAY,
    RTC_WEEKDAY_THURSDAY,
    RTC_WEEKDAY_FRIDAY,
    RTC_WEEKDAY_SATURDAY,
    RTC_WEEKDAY_SUNDAY
};



/*****************************************************************************/
/* PUBLIC STRUCTURES */
/*****************************************************************************/

typedef struct rtc_time {
    unsigned int hour;
    unsigned int minute;
    unsigned int second;
}rtc_time_t;



typedef struct rtc_date {
    unsigned int year;
    enum rtc_month month;
    unsigned int day;
    enum rtc_weekday weekday;
}rtc_date_t;



/*****************************************************************************/
/* PUBLIC API */
/*****************************************************************************/

/**
 * @brief   Initialize RTC (Real-Time Clock). Its time and date are set to
 *          default values, i.e. 00:00:00 00-00-2000. RTC counts years from 00
 *          to 99, but we have to add offset of 2000 to make date correct.
 *
 * @param   None.
 *
 * @retval  None.
 */
void rtc_init(void);



#endif /* RTC_H */

