/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Time.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yzaytoun <yzaytoun@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/01 12:58:45 by yzaytoun          #+#    #+#             */
/*   Updated: 2025/02/07 18:58:51 by yzaytoun         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# pragma once

# include <ctime>
# include <stdint.h>
# include <cstdio>
# include <string>
# include <iostream>
# include "utils.h"

# define ONE_DAY 			86400
# define ONE_HOUR 			360
# define ONE_MINUTE 		60
# define TIME_BUFFER_SIZE	64
# define HTTP_DEF_FORMAT	"%a, %d %b %Y %H:%M:%S %Z"
# define DEF_TIME_FORMAT	"%H:%m:%d"
# define DEF_DATE_FORMAT	"%Y-%m-%d"

class Time
{
	public:
		enum Period
		{
			DAY,
			MONTH,
			YEAR
		};

private:
	std::time_t	_time;
	std::tm		*_timeinfo;

public:
	Time();
	Time(const int64_t seconds);
	~Time();
	Time(const Time& dt);
	Time&	operator=(const Time& dt);
	Time&	operator=(const int64_t seconds);
	bool	operator<(const Time& dt);
	bool	operator>(const Time& dt);
	bool	operator==(const Time& dt);

	/* Setters */
	void			setTime(const int64_t hour ,const int64_t minutes ,const int64_t seconds);
	void			addMonths(const int64_t months_num);
	void			addYears(const int64_t years_num);
	void			addDays(const int64_t days_num);

	/* Getters */
	std::time_t			getCurrent(void) const;
	std::tm*			getTimeInfo(void) const;
	std::string			getDateString(void) const;
	std::string			getTimeString(void) const;
	int					getYear(void) const;
	int					getMonth(void) const;
	int					getDay(void) const;
	int					getHour(void) const;
	int					getMinutes(void) const;
	int					getSeconds(void) const;
	std::string			getDateTimeString(void) const;
	std::string			getDateTimeString(const char* format) const;
	std::string			getDateTimeString(const char* format);
};
