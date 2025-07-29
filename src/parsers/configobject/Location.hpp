#pragma once

#include "ConfigObject.hpp"
#include "Server.hpp"
#include <algorithm>
#include "HTTPElements.hpp"
#include "HTTPRequest.hpp"
#include "Directory.hpp"

class Location : public ConfigObject
{
	enum AttributeType
	{
		ROOT,
		INDEX,
		RETURN,
		AUTOINDEX,
		CLIENT_MAX_BODY,
		ADD_COOKIE,
		TRY_FILES,
		ERROR_PAGE,
		ACCEPTED_METHODS,
		CGI_PARAM,
		ROOT_ALIAS
	};

	private:
		bool	_is_cgi;
		bool	_check_method;

	public:
		Location();
		Location(std::string location);
		~Location();

		/* Attributes */
		std::string					_path;
		std::string					cgi_params[HTTP::NUM_CGI_VARIABLE];
		std::string					root;
		std::vector<std::string>	cookie;
		std::string					alias;
		std::vector<std::string>	index;

		bool 						autoindex;
		std::vector<std::string>	returns;
		std::map<int, std::string>	error_pages;
		std::size_t 				client_max_body;
		std::vector<std::string>	try_files;
		bool						accepted_methods[HTTP::NUM_METHODS];
	
		/* Methods */
		int				countAllowedMethods(void);
		std::string		getType();
		std::vector<std::string> getCookies(void) const;
		std::string		getRealPath(const std::string& path);
		void			Print_Content();
		void 			Mount(ConfigObject& obj);
		void 			CheckConfig(ConfigObject& obj);
		void			SetDefaultConfig(ConfigObject& obj);
		bool			isMethodAllowed(HTTP::Method method);
		void			useCGI(bool use_cgi = true);
		void			copyCGIParams(const std::string *cgi_params);
		void			skipCheckMethod(bool check_method);
		bool			isCGI(const std::string& filepath);
		bool			isCGI(void);
		bool			find(const std::string& path);
		bool			checkMethod(void);
		void			setCGIConfig(HTTPRequest& request);
		void			setCGIVariable(HTTP::CgiVarible identifier, std::vector<std::string> vec, int idx = 0, bool overwrite = false);
		void			setCGIVariable(HTTP::CgiVarible identifier, std::string val, bool overwrite = false);
		std::string		getCGIVariable(const HTTP::CgiVarible identifier, bool with_key = false);

	protected:
		virtual void			AddAttribute(std::vector<std::string>& info);
		virtual ConfigObject*	AddObject(std::vector<std::string>& info);
		std::map<std::string, AttributeType> dictTypes;
};
