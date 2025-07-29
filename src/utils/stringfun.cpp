/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   stringfun.cpp                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yzaytoun <yzaytoun@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/27 15:13:00 by yzaytoun          #+#    #+#             */
/*   Updated: 2025/03/29 15:58:08 by yzaytoun         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "utils.h"

/*
	Iterate the string "str" and replaces all characters which
		return true when passed to function pointer "(*func)"
*/
int	RemoveChar(std::string& str, int (*func)(int))
{
	size_t	i = 0;
	int		ch_count = 0;

	while (i < str.length())
	{
		if (func(str.at(i)))
		{
			str.erase(i, 1);
			++ch_count;
		}
		++i;
	}
	if (str.length() == 1 && func(str.at(0)))
	{
		str.erase(0, 1);
		++ch_count;
	}
	return (ch_count);
}

int	is_dquote(int c)
{
	if (c == '\"')
		return (true);
	return (false);
}

int	isCRLF(int c)
{
	if (c == '\r' || c == '\n')
		return (true);
	return (false);
}

/*
	Iterate the string "str" and check all characters which
		return true when passed to function pointer "(*func)"
*/
bool	StringCheck(std::string& str, int (*func)(int))
{
	size_t	i = 0;
	bool	check = true;

	while (i < str.length())
	{
		if (!func(str.at(i)))
		{
			check = false;
			break;
		}
		++i;
	}
	return (check);
}

/*
	Iterates through "str" and replaces all substrings that match the "str_to_replace"
		with the new substring "new_str"
	@returns number of characters replaced
*/
int	ReplaceSubString(std::string& str, std::string str_to_replace, std::string new_str)
{
	size_t	pos = 0;
	int		count = 0;

	if (str.empty())
		return (-1);
	while (true)
	{
		pos = str.find(str_to_replace);
		if (pos != std::string::npos)
		{
			str.replace(pos, str_to_replace.length(), new_str);
			count += str_to_replace.length();
		}
		else
			break;
	}
	return (count);
}

bool startsWith(const std::string& str, const std::string& prefix)
{
    if (str.length() < prefix.length())
        return false;

    return str.compare(0, prefix.length(), prefix) == 0;
}

bool endsWith(const std::string& str, const std::string& suffix)
{
    if (str.length() < suffix.length())
        return false;

    return (
		str.compare(
			str.length() - suffix.length(),
			suffix.length(),
			suffix
			) == 0
	);
}

std::string trim(const std::string& str)
{
    size_t first = 0;
    size_t last = str.length();

    while (first < last && std::isspace(str[first]))
        ++first;

    while (last > first && std::isspace(str[last - 1]))
        --last;

    return str.substr(first, last - first);
}

int stringToInt(const std::string& str)
{
    std::stringstream ss(str);
    int result = 0;
    if (!(ss >> result))
        throw std::invalid_argument("Invalid integer: " + str);
    return result;
}

size_t stringToUnsignedLong(const std::string& str)
{
    std::stringstream ss(str);
    size_t result = 0;
    if (!(ss >> result))
        throw std::invalid_argument("Invalid number: " + str);
    return (result);
}

std::string intToString(int num)
{
    std::stringstream ss;

    if (!(ss << num))
        throw std::invalid_argument("Invalid integer");
    return (ss.str());
}

std::string unIntToString(unsigned int num)
{
    std::stringstream ss;

    if (!(ss << num))
        throw std::invalid_argument("Invalid integer");
    return (ss.str());
}

std::string	sizeToString(std::size_t num)
{
    std::stringstream ss;

    if (!(ss << num))
        throw std::invalid_argument("Invalid integer");
    return (ss.str());
}

/*
	Divide input by delimiter and convert to a vector
*/
std::vector<std::string> split_char(std::string input, char delimiter)
{
    std::vector<std::string> palabras;
    std::istringstream iss(input);
    std::string palabra;

    while (std::getline(iss, palabra, delimiter))
        palabras.push_back(palabra);

    return palabras;
}

/*
	Search str for pattern
*/
bool	ContainsSubstr(std::string str, std::string pattern)
{
	bool	contains = false;

	if (str.find(pattern) != str.npos)
		contains = true;
	return (contains);
}

char	**VecStringToArrChars(std::vector< std::string> strvec)
{
	char** strarr = new char*[strvec.size() + 1];

	for (size_t i=0; i < strvec.size() ; i++)
	{
		strarr[i] = new char[strvec[i].size() + 1];
		std::strncpy(strarr[i], strvec[i].c_str(), strvec[i].size());
		strarr[i][strvec[i].size()] = '\0';
	}
	strarr[strvec.size()] = NULL;
	return strarr;
}

/*
	@param std::string
	@returns A string stream with the extracted digits
*/
std::string	ExtractDigits(const std::string& str)
{
	std::stringstream	ss;
	ssize_t				len = str.length();
	ssize_t				i = 0;

	if (str.empty())
		return (str);
	while (i < len)
	{
		if (std::isdigit(static_cast<unsigned char>(str.at(i))))
			ss << str.at(i);
		++i;
	}
	return (ss.str());
}

/*
	Extract value from string
	@returns a string starting with the string indicated by val
	@param str string to search
	@param val string to extract
*/
std::string	ExtractString(std::string str, std::string val)
{
	std::string	result;
	size_t		pos;
	
	pos = str.find(val);
	if (pos != STRING_NULL)
		result = str.substr(pos); 
	return (result);
}
/*
	Extract value from string
	@returns a string starting with the string indicated by val
	@param str string to search
	@param val string to extract
	@param delimiter string that marks the string end
*/
std::string	ExtractString(std::string str, std::string val, std::string delimiter)
{
	std::string	result;
	size_t		pos = 0;
	size_t		dpos = 0;
	
	pos = str.find(val);
	if (pos != STRING_NULL)
	{
		dpos = str.find(delimiter, pos);
		if (dpos != STRING_NULL && dpos > pos)
			result = str.substr(pos, (dpos - pos));
		else
			result = str.substr(pos);
	}
	return (result);
}

std::string	ExtractString(std::vector<std::string> vec, std::string val)
{
	std::string	result;
	size_t		pos;
	
	if (!vec.empty())
	{
		pos = vec.at(0).find(val);
		if (pos != STRING_NULL)
			result = vec.at(0).substr(pos);
	}
	return (result);
}

bool	isSuffixExpansion(const std::string& path)
{
	return (startsWith(path, "~\\."));
}

bool	isValidNum(const std::string num)
{
	bool	is_valid = false;

	if (!num.empty())
	{
		if (std::strtoul(num.c_str(), 0, 10) != ULONG_MAX)
			is_valid = true;
	}
	return (is_valid);
}

/*
	Returns the first position of the string validated by function
*/
std::size_t	findString(std::string str, int (*function)(int))
{
	std::size_t	pos = STRING_NULL;

	if (!str.empty() && function)
	{
		for (size_t i = 0; i < str.size(); i++)
		{
			if (function(str.at(i)))
			{
				pos = i;
				break;
			}
		}
	}
	return (pos);
}

void	freeCharArray(char* const str[])
{
	int	i = 0;

	if (str)
	{
		while (str[i])
		{
			delete[] str[i];
			++i;
		}
		delete[] str;
	}
}