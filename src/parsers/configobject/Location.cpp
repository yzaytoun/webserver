# include "Location.hpp"

Location::Location() :
	_is_cgi(false), _check_method(true), autoindex(false), client_max_body(0) {}

Location::Location(std::string location) : 
	_is_cgi(false), _check_method(true), _path(location), autoindex(false), client_max_body(0)
{
	for (std::size_t i = 0; i < HTTP::NUM_METHODS; i++)
	{
		accepted_methods[i] = false;
	}
	dictTypes["root"] = ROOT;
	dictTypes["add_cookie"] = ADD_COOKIE;
	dictTypes["alias"] = ROOT_ALIAS;
	dictTypes["index"] = INDEX;
	dictTypes["return"] = RETURN;
	dictTypes["autoindex"] = AUTOINDEX;
	dictTypes["client_max_body_size"] = CLIENT_MAX_BODY;
	dictTypes["error_page"] = ERROR_PAGE;
	dictTypes["limit_except"] = ACCEPTED_METHODS;
	dictTypes["cgi_param"] = CGI_PARAM;
	dictTypes["try_files"] = TRY_FILES;

	/* CGI Def values */
	cgi_params[HTTP::CgiGatewayInterface] = "CGI/1.1";
	cgi_params[HTTP::CgiServerProtocol] = "HTTP/1.1";
	cgi_params[HTTP::CgiDocumentRoot] = WEBSERV_HOME DOCROOT;
}

Location::~Location() {}

void Location::Mount(ConfigObject& obj)
{
	Server	*server = dynamic_cast<Server *>(&obj);

	server->location_map[this->_path] = this;
}

ConfigObject* Location::AddObject(std::vector<std::string>& info)
{
	Throw_Obj_Error(info.at(0));
	return NULL;
}

void Location::AddAttribute(std::vector<std::string>& info)
{
	if (!info.empty())
	{
		if (dictTypes.find(info.at(0)) == dictTypes.end())
			Throw_Att_Error(info.at(0));
		switch (dictTypes[info.at(0)])
		{
			case CGI_PARAM: 		FillCGIVariable(cgi_params, info); 		break;
			case ROOT:				Handle_Str(root, info);							break;
			case ROOT_ALIAS:		Handle_Str(alias, info);						break;
			case INDEX:				Handle_MultiStrings(index, info);				break;
			case TRY_FILES:			Handle_MultiStrings(try_files, info);			break;
			case ERROR_PAGE:		Handle_Map(error_pages, info);					break;
			case ACCEPTED_METHODS:	FillAcceptMethod(accepted_methods, info);	    break;
			case CLIENT_MAX_BODY:	Handle_unSignedInt(client_max_body, info);		break;
			case ADD_COOKIE:
			{
				if (info.size() > 3 || info.size() < 2)
					Throw_Att_Error(info.at(0));
				cookie.push_back(info.at(2) + "; Max-age=" + info.at(1));
			}
				break;
			case RETURN:			Handle_MultiStrings(returns, info); 	break;
			case AUTOINDEX:
			{
				if (info.size() != 2)
					Throw_Att_Error(info.at(0));
				if (!info.at(1).compare("on"))
					autoindex = true;
			}
				break;
		}
	}
}

void Location::Print_Content()
{
	std::cout << "\n\t\tLocation content:" << std::endl;
	std::cout << "\t\t\tpath: " + _path << std::endl;
	std::cout << "\t\t\troot: " + root << std::endl;
	std::cout << "\t\t\tclient_max_body: " << client_max_body << std::endl;
	Print_Str_Vector("\t\t\tindex", index);
	std::cout << "\t\t\tautoindex: " << (autoindex ? "true" : "false") << std::endl;
	//Print_Str_Vector(YELLOW "\t\t\tAccepted Methods" RESET, accepted_methods);
	//Print_Map("\t\t\tReturns", returns,  "\t\t\t\t");
	//Print_Map("\t\t\tError Pages", error_pages, "\t\t\t\t");
}

void Location::CheckConfig(ConfigObject& obj)
{
	Server	*server = dynamic_cast<Server *>(&obj);

	if (root.empty())
		root = server->root + _path;
	if (index.empty())
		index = server->index;
	if (error_pages.empty())
		error_pages = server->error_pages;
	bool* some_method = std::find(accepted_methods, accepted_methods + HTTP::NUM_METHODS - 1, true);
	if ((*some_method) == false) {
		accepted_methods[HTTP::StrToMethodId("GET")] = true;
		accepted_methods[HTTP::StrToMethodId("POST")] = true;
		accepted_methods[HTTP::StrToMethodId("DELETE")] = true;
	}
	if (!client_max_body)
		client_max_body = server->client_max_body;
}

std::string Location::getType()
{
	return "Location";
}

void	Location::SetDefaultConfig(ConfigObject& obj)
{
	Server	*server = dynamic_cast<Server *>(&obj);
	
	if (_path != server->root)
		root = server->root + _path;
	index = server->index;
	error_pages = server->error_pages;
	client_max_body = server->client_max_body; 
	accepted_methods[HTTP::StrToMethodId("GET")] = true;
	accepted_methods[HTTP::StrToMethodId("POST")] = true;
	accepted_methods[HTTP::StrToMethodId("DELETE")] = true;
}

void	Location::setCGIVariable(HTTP::CgiVarible identifier, std::string val, bool overwrite)
{
	if (overwrite)
		cgi_params[identifier] = val;
	else if (!val.empty() && cgi_params[identifier].empty())
		cgi_params[identifier] = val;
}

void	Location::setCGIVariable(HTTP::CgiVarible identifier, std::vector<std::string> vec, int idx, bool overwrite)
{
	if (!vec.empty())
	{
		if (overwrite)
			cgi_params[identifier] = vec.at(idx);
		else if (cgi_params[identifier].empty())
			cgi_params[identifier] = vec.at(idx);
	}
}

std::string	Location::getCGIVariable(const HTTP::CgiVarible identifier, bool with_key)
{
	std::string	var;

	if (!cgi_params[identifier].empty())
	{
		if (with_key)
			var = HTTP::CGIVariableToString(identifier) + "=" + cgi_params[identifier];
		else
			var = cgi_params[identifier];
	}
	return (var);
}

/*
	Check allowed method
	@note this check is always true for suffix expansions
*/
bool	Location::isMethodAllowed(HTTP::Method method)
{
	return (accepted_methods[method]);
}

void	Location::useCGI(bool use_cgi)
{
	_is_cgi = use_cgi;
}

/*
	Returns true:
	- if location object is a process directory
	- if filepath is an executable
*/
bool	Location::isCGI(const std::string& filepath)
{
	bool is_cgi = ContainsSubstr(filepath, "cgi-bin");

	if (!is_cgi)
		is_cgi = Directory::isExecutable(filepath);
	if (!is_cgi)
		is_cgi = _is_cgi;
	return (is_cgi);
}

bool	Location::isCGI(void)
{
	return (_is_cgi);
}

std::vector<std::string>	 Location::getCookies(void) const
{
	return cookie;
}

/*
	@returns real path of file
*/
std::string	Location::getRealPath(const std::string& path)
{
	std::string	real_path;
	std::string	filepath = path;
	size_t		i = 0;

	if (!this->alias.empty())
		ReplaceSubString(filepath, _path, alias);
	real_path = Directory::getRealPath(filepath);
	if (Directory::isDir(real_path) && !try_files.empty())
	{
		while (i < try_files.size())
		{
			if (StringCheck(try_files.at(i), std::isdigit))
				throw HTTPException(HTTP::StrToStatusCode(try_files.at(i)));
			else
			{
				real_path = Directory::getRealPath(real_path + "/" + try_files.at(i));
				if (real_path.compare(try_files.at(i)) != 0)
					break;
			}
			++i;
		}
		
	}
	return (real_path);
}

void	Location::copyCGIParams(const std::string *cgi_params)
{
	for (size_t i = 0; i < HTTP::NUM_CGI_VARIABLE; i++)
	{
		this->cgi_params[i] = cgi_params[i];
	}
}

/*
	Find path in location
	@returns true if path exists
*/
bool	Location::find(const std::string& path)
{
	return (Directory::pathExists(getRealPath(path)));
}

void	Location::skipCheckMethod(bool check_method)
{
	_check_method = !check_method;
}

bool	Location::checkMethod(void)
{
	return (_check_method);
}

/*
	Sets path_info and script_path for CGI execution
*/
void	Location::setCGIConfig(HTTPRequest& request)
{
	std::string::size_type	end_scriptname = 0;
	std::string				path_info;
	std::string				script_path;

	while (end_scriptname != std::string::npos)
	{
		end_scriptname = request.params[HTTP::FULL_PATH].find('/', end_scriptname);
		script_path = request.params[HTTP::FULL_PATH].substr(0, end_scriptname);
		if (Directory::isFile(script_path))
		{
			path_info = request.params[HTTP::FULL_PATH].substr(script_path.size());
			this->setCGIVariable(HTTP::CgiPathInfo, path_info);
			this->setCGIVariable(HTTP::CgiScriptName, script_path);
			break;
		}
		// continue in directoriy path, pass slash 
		if (end_scriptname != std::string::npos)
			++end_scriptname;
	}
	/* Set PATH_INFO only if empty */
	setCGIVariable(HTTP::CgiPathInfo, request.getLocation());
	setCGIVariable(HTTP::CgiRequestMethod, HTTP::MethodIdToString(request.method));
	setCGIVariable(HTTP::CgiPathTranslated, request.params[HTTP::FULL_PATH]);
	/* Content Length */
	if (!request.headers[HTTP::ContentLength].empty())
		setCGIVariable(HTTP::CgiContentLength, request.headers[HTTP::ContentLength]);
	else if (request.params[HTTP::CONTENT].length() > 0)
		setCGIVariable(HTTP::CgiContentLength, sizeToString(request.params[HTTP::CONTENT].length()));
	setCGIVariable(HTTP::CgiContentType, request.headers[HTTP::ContentType]);
	setCGIVariable(HTTP::CgiHTTPAccept, request.headers[HTTP::Accept]);
	setCGIVariable(HTTP::CgiHTTPConnection, request.headers[HTTP::Connection]);
	setCGIVariable(HTTP::CgiContentType, request.headers[HTTP::ContentType]);
	setCGIVariable(HTTP::CgiQueryString, request.params[HTTP::QUERY]);
	setCGIVariable(HTTP::CgiHTTPAgent, request.headers[HTTP::UserAgent]);
	setCGIVariable(HTTP::CgiServerName, request.getServer()->server_name);
	setCGIVariable(HTTP::CgiServerPort, request.params[HTTP::PORT]);
}

int	Location::countAllowedMethods(void)
{
	int	count = 0;

	for (std::size_t i = 0; i < HTTP::NUM_METHODS; i++)
	{
		if (accepted_methods[i])
			++count;
	}
	return (count);
}
