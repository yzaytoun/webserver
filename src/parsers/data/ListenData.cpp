#include "ListenData.hpp"
#include "utils.h"
            
ListenData::ListenData(std::map<std::string, std::string>& target) : target(target)
{
	Actions.push_back(static_cast<void (AData::*)(std::string)>(&ListenData::getIp));
	Actions.push_back(static_cast<void (AData::*)(std::string)>(&ListenData::getPortToIp));
	Actions.push_back(static_cast<void (AData::*)(std::string)>(&ListenData::setPortToIp));
	Actions.push_back(static_cast<void (AData::*)(std::string)>(&ListenData::getPort));
};

/// @brief Verifica que si se está usando el puerto y si este es válido.
/// @param target puertos del servidor.
/// @param port cadena con el puerto.
/// @return true si se está usando.
bool validPort(std::map<std::string, std::string>& target, std::string port)
{
	if (port.empty() || !std::isdigit(port[port.size() - 1]))
		return false;
	
	if (mapValueSearch(target, port))
	{
		std::cout << port << " Port already used!!" << std::endl;
		return false;
	}
	return true;
}

void ListenData::getIp(std::string host)
{
	host = trim(host);
	this->host = host;
};

void ListenData::getPortToIp(std::string port)
{
	port = trim(port);
	this->port = port;
};

void ListenData::setPortToIp(std::string ip_port)
{
	(void)ip_port;
	if (this->host.empty())
		return;

	if (this->port.empty())
	{
		if (validPort(target, DEF_PORT))
			target[this->host] = DEF_PORT;
	}
	else if (validPort(target, this->port))
	{
		target[this->host] = this->port;
	}
	this->host = "";
	this->port = "";
};

void ListenData::getPort(std::string port)
{
	port = trim(port);
	if (validPort(target, port))
		target[DEF_HOST] = port;
};