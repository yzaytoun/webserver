/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   debug.cpp	                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yzaytoun <yzaytoun@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/27 15:13:00 by yzaytoun          #+#    #+#             */
/*   Updated: 2024/12/28 15:44:43 by yzaytoun         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "utils.h"

/* Debug Function */
void	Print(std::string str, bool formated, bool end_line)
{
	if (formated)
	{
		for (size_t i = 0; i < str.size(); i++)
		{
			switch (str.at(i))
			{
			case '\n':
				std::cout << "\\n" ;
				break;
			case '\r':
				std::cout << "\\r" ;
				break;
			case '\t':
				std::cout << "\\t" ;
				break;
			case '\\':
				std::cout << "\\\\" ;
				break;
			
			default:
				std::cout << str.at(i);
				break;
			}
		}
	}
	else
		std::cout << str;
	if (end_line)
		std::cout << std::endl;
}

void PrintRequest(std::string bufferstr, ssize_t bytes_read)
{
	std::cout
		<< CYAN "\n*----------- Request ------------*\n" RESET
		<< "Bytes leidos: " << bytes_read << "\n" 
		<< bufferstr
		<< "----------------------------------\n"
		<< std::endl;
}
