/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yzaytoun <yzaytoun@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/27 12:08:19 by yzaytoun          #+#    #+#             */
/*   Updated: 2025/03/29 15:58:36 by yzaytoun         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef UTILS_H
# define UTILS_H

/* includes */
# include <iostream>
# include <string>
# include <cstring>
# include <sys/stat.h>
# include "macros.h"
# include <sstream>
# include <fstream>
# include <unistd.h>
# include <vector>
# include <map>
# include "Directory.hpp"
# include "Timer.hpp"

/* String Functions */
int							RemoveChar(std::string& str, int (*func)(int));
int							ReplaceSubString(std::string& str, std::string str_to_replace, std::string new_str);
bool						startsWith(const std::string& str, const std::string& prefix);
bool						endsWith(const std::string& str, const std::string& suffix);
std::string					trim(const std::string& str);
int							stringToInt(const std::string& str);
size_t 						stringToUnsignedLong(const std::string& str);
std::string					intToString(int num);
std::string                 unIntToString(unsigned int num);
std::string                 sizeToString(std::size_t num);
std::vector<std::string>	split_char(std::string input, char delimiter);
bool						ContainsSubstr(std::string string, std::string pattern);
std::size_t					findString(std::string str, int (*function)(int));
char	                    **VecStringToArrChars(std::vector< std::string> strvec);
std::string					ExtractDigits(const std::string& str);
std::string 	            ExtractString(std::string str, std::string val);
std::string                 ExtractString(std::string str, std::string val, std::string delimiter);
std::string					ExtractString(std::vector<std::string> vec, std::string val);
bool						StringCheck(std::string& str, int (*func)(int));
void	                    freeCharArray(char* const str[]);

/* Checkers */
int	        is_dquote(int c);
int 	    isCRLF(int c);
bool	    isSuffixExpansion(const std::string& path);
bool        isValidNum(const std::string num);

/* Containers Functions */
bool		vecSearch(const std::vector<std::string>& vector, std::string element);
bool		mapValueSearch(const std::map<std::string, std::string>& map, std::string value);
std::string	vectorCompress(const std::vector<std::string>& strvec);
bool	    inRange(const int val, const int min, const int max);

/* Debug */
void			Print(std::string str, bool formated = false, bool end_line = true);
void			PrintRequest(std::string bufferstr, ssize_t bytes_read);

#endif