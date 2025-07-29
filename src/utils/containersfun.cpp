/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   containersfun.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yzaytoun <yzaytoun@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/27 15:13:00 by yzaytoun          #+#    #+#             */
/*   Updated: 2025/03/21 19:45:23 by yzaytoun         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "utils.h"

/*
	Search for string element inside vector
	@param Vector to search
	@param element to find
*/
bool	vecSearch(const std::vector<std::string>& vector, std::string element)
{
	bool	exists = false;
	size_t	i = 0;

	if (!vector.empty())
	{
		while (i < vector.size())
		{
			if (vector.at(i).compare(element) == 0)
			{
				exists = true;
				break;
			}
			++i;
		}
	}
	return (exists);
}

/*
	Convert string vector into a string
	@param strvec string vector

*/
std::string	vectorCompress(const std::vector<std::string>& strvec)
{
	std::ostringstream	sso;
	size_t		i = 0;

	if (!strvec.empty())
	{
		while (i < strvec.size())
		{
			sso << strvec.at(i);
			++i;
		}
	}
	return (sso.str());
}

/*
	Search for string value inside map
	@param Map to search
	@param value to find
*/
bool	mapValueSearch(const std::map<std::string, std::string>& map, std::string value)
{
	std::map<std::string, std::string>::const_iterator	it = map.begin();
	bool	exists = false;

	if (!map.empty())
	{
		while (it != map.end())
		{
			if (it->second.compare(value) == 0)
			{
				exists = true;
				break;
			}
			++it;
		}
	}
	return (exists);
}

bool	inRange(const int val, const int min, const int max)
{
	if (val >= min && val <= max)
		return (true);
	return (false);
}

