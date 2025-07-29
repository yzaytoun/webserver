#pragma once

#include "ConfigObject.hpp"
#include "Server.hpp"
#include "EpollManager.hpp"
#include "RequestParser.hpp"

class Server;

class Master : public ConfigObject
{
	enum AttributeType
	{
		SERVER,
		CLIENT_MAX_BODY,
		ERROR_PAGE,
	};

	public:
		Master();
		Master(std::string configPath);
		~Master();
		
		/* Attributes */
		std::map<int, std::string>			error_pages;
		std::size_t							client_max_body;
		std::string							default_root;

		/* Methods */
		void			Innit(std::string path = "");
		void			Print_Content();
		std::string		getType();
		void			Mount(ConfigObject& obj);
		void			CheckConfig(ConfigObject& obj);
		void			SetDefaultConfig(ConfigObject& obj);
		void			Run();
		bool			checkSimilarConfig(void);


	private:
		std::vector<Server*>			servers;
		EpollManager				*_epollManager;
		std::map<std::string, AttributeType> dictTypes;
		void			AddAttribute(std::vector<std::string>& info);
		ConfigObject*	AddObject(std::vector<std::string>& info);
};
