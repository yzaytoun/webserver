#pragma once

#include "ConfigObject.hpp"
#include "Location.hpp"
#include "PassiveSocket.hpp"
#include "SocketHandler.hpp"
#include "Master.hpp"
#include "utils.h"

class Location;

class Server : public ConfigObject
{
	enum AttributeType
	{
		LOCATION,
		LISTEN,
		ROOT,
		INDEX,
		AUTOINDEX,
		SERVER_NAME,
		CLIENT_MAX_BODY,
		ERROR_PAGE,
		TIMEOUT,
		CGI_TIMEOUT,
		ADD_HEADER
	};

	private:
		bool		accepted_mimetypes[HTTP::NUM_MIME_TYPE];
		bool		implemented_encodings[HTTP::NUM_HTTP_ENCODING];
		uint64_t	_timeout;
		uint64_t	_cgi_timeout;
		
		void	addDefaultLocations(void);
		void	evaluateListen(std::map<std::string, std::string>& target, std::vector<std::string>& info);
	
	public:
		Server();
		~Server();
		bool	operator==(const Server& server);
		
		/* Attributes */
		std::map<std::string, Location*>	location_map;
		std::map<std::string, std::string>	listen;
		std::vector<PassiveSocket*>			sockets;
		std::vector<std::string>			server_name;
		std::vector<std::string>			index;
		bool								autoindex;
		std::string							root;
		std::size_t							client_max_body;
		std::map<int, std::string>			error_pages;
		std::map<std::string, std::string>	headers;
		
		/* Methods */
		std::string					getType();
		void 						Print_Content();
		void 						Mount(ConfigObject& obj);
		void 						CheckConfig(ConfigObject& obj);
		void						SetDefaultConfig(ConfigObject& obj);
		Location*					findLocation(const std::string& path, HTTP::Method method);
		bool						isMimeTypeAccepted(std::string content_type);
		bool						isMimeTypeAccepted(std::vector<std::string> contents);
		bool						isEncodingImplemented(std::vector<std::string> contents);
		void						applySuffixConfig(Location* location, const std::string& path, HTTP::Method method);
		uint64_t					getTimeOut(bool is_cgi = false);

		std::vector<std::string>	getHosts(void) const;
	
	protected:
		virtual void AddAttribute(std::vector<std::string>& info);
		virtual ConfigObject* AddObject(std::vector<std::string>& info);
		std::map<std::string, AttributeType> dictTypes;
};
