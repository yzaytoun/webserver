/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Time.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yzaytoun <yzaytoun@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/01 12:58:32 by yzaytoun          #+#    #+#             */
/*   Updated: 2025/02/07 18:30:37 by yzaytoun         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Time.hpp"

Time::Time() : _time(std::time(NULL))
{
	_timeinfo = localtime(&_time);
	mktime(_timeinfo);
}

Time::Time(const int64_t seconds) : _time(seconds)
{
	_timeinfo = localtime(&_time);
	mktime(_timeinfo);
}

Time::~Time() {}

Time::Time(const Time& dt)
{
	this->_time = dt.getCurrent();
	this->_timeinfo = dt.getTimeInfo();
}

Time&	Time::operator=(const Time& dt)
{
	if (this != &dt)
	{
		this->_time = dt.getCurrent();
		this->_timeinfo = dt.getTimeInfo();
	}
	return (*this);
}

Time&	Time::operator=(const int64_t seconds)
{
	_time = seconds;
	_timeinfo = localtime(&_time);
	mktime(_timeinfo);
	return (*this);
}

bool	Time::operator==(const Time& dt)
{
	return (mktime(_timeinfo) == mktime(dt.getTimeInfo()));	
}

void	Time::setTime(const int64_t hour ,const int64_t minutes ,const int64_t seconds)
{
	_timeinfo->tm_hour = hour;
	_timeinfo->tm_min = minutes;
	_timeinfo->tm_sec = seconds;

	_time = mktime(_timeinfo);
}

void	Time::addMonths(const int64_t months_num)
{
	_timeinfo->tm_mon += months_num;

	_time = mktime(_timeinfo);
}

void	Time::addYears(const int64_t years_num)
{
	_timeinfo->tm_year += years_num;

	_time = mktime(_timeinfo);
}

void	Time::addDays(const int64_t days_num)
{
	_timeinfo->tm_yday += days_num;

	_time = mktime(_timeinfo);
}

bool	Time::operator<(const Time& dt)
{
	return (mktime(_timeinfo) < mktime(dt.getTimeInfo()));	
}

bool	Time::operator>(const Time& dt)
{
	return (mktime(_timeinfo) > mktime(dt.getTimeInfo()));	
}

std::time_t	Time::getCurrent(void) const
{
	return(std::time(NULL));
}

std::tm*	Time::getTimeInfo(void) const
{
	return (_timeinfo);
}

std::string	Time::getDateTimeString(void) const
{
	return (getDateTimeString(HTTP_DEF_FORMAT));
}

std::string	Time::getDateTimeString(const char* format) const
{
	char		buf[TIME_BUFFER_SIZE];
	std::string	_format = format;

	if (_format.empty())
		format = HTTP_DEF_FORMAT;
	strftime(buf, sizeof(buf), format, std::localtime(&_time));
	return (buf);
}

std::string	Time::getDateTimeString(const char* format)
{
	char		buf[TIME_BUFFER_SIZE];
	std::string	_format = format;

	if (_format.empty())
		format = HTTP_DEF_FORMAT;
	strftime(buf, sizeof(buf), format, std::localtime(&_time));
	return (buf);
}

int	Time::getYear(void) const
{
	return (_timeinfo->tm_year + 1900);
}

int	Time::getMonth(void) const
{
	return (_timeinfo->tm_mon + 1);
}

int	Time::getDay(void) const
{
	return (_timeinfo->tm_mday);
}

int	Time::getMinutes(void) const
{
	return (_timeinfo->tm_min);
}

int	Time::getHour(void) const
{
	return (_timeinfo->tm_hour);
}

int	Time::getSeconds(void) const
{
	return (_timeinfo->tm_sec);
}

std::string	Time::getDateString(void) const
{
	std::string	buff(TIME_BUFFER_SIZE, ' ');

	strftime((char *)buff.c_str(), sizeof(buff), DEF_DATE_FORMAT, std::localtime(&_time));
	return (buff);
}

std::string	Time::getTimeString(void) const
{
	std::string	buff(TIME_BUFFER_SIZE, ' ');

	strftime((char *)buff.c_str(), sizeof(buff), DEF_TIME_FORMAT, std::localtime(&_time));
	return (buff);
}
