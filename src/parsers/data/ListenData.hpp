#pragma once

#ifndef LISTENDATA_HPP
#define LISTENDATA_HPP

# include "macros.h"
# include "RegexState.hpp"

class AData;

/// @brief Class needed to give actions to RegexParser with regular expression for listen content.
class ListenData : public AData
{
	private:
		std::string host;
		std::string port;
		std::map<std::string, std::string>& target;

	public:
		ListenData(std::map<std::string, std::string>& target);
		~ListenData(){};
		void getIp(std::string str);
		void getPortToIp(std::string str);
		void setPortToIp(std::string str);
		void getPort(std::string str);
};
#endif
