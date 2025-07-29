#include "Server.hpp"
#include "ListenData.hpp"

Server::Server() : _timeout(DEF_TIMEOUT), _cgi_timeout(DEF_TIMEOUT), autoindex(false), client_max_body(0)
{
	dictTypes["location"] = LOCATION;
	dictTypes["listen"] = LISTEN;
	dictTypes["root"] = ROOT;
	dictTypes["index"] = INDEX;
	dictTypes["autoindex"] = AUTOINDEX;
	dictTypes["server_name"] = SERVER_NAME;
	dictTypes["client_max_body_size"] = CLIENT_MAX_BODY;
	dictTypes["error_page"] = ERROR_PAGE;
	dictTypes["add_header"] = ADD_HEADER;
	dictTypes["timeout"] = TIMEOUT;
	dictTypes["cgi_timeout"] = CGI_TIMEOUT;

	accepted_mimetypes[HTTP::StrToMimeTypeId("text/htm")] = true;
	accepted_mimetypes[HTTP::StrToMimeTypeId("text/html")] = true;
	accepted_mimetypes[HTTP::StrToMimeTypeId("image/jpeg")] = true;
	accepted_mimetypes[HTTP::StrToMimeTypeId("application/x-httpd-php")] = true;
	accepted_mimetypes[HTTP::StrToMimeTypeId("*")] = true;
	accepted_mimetypes[HTTP::StrToMimeTypeId("application/x-sh")] = true;
	accepted_mimetypes[HTTP::StrToMimeTypeId("application/py")] = true;
	implemented_encodings[HTTP::chunked] = true;
}

Server::~Server()
{
	int	i = -1;

	while (++i < (int)sockets.size())
	{
		delete sockets[i];
	}
	if (!resources.empty())
		for (size_t i = 0; i < resources.size(); i++)
			delete resources.at(i);
}

bool	Server::operator==(const Server& server)
{
	bool	is_equal = false;

	if ((this == &server) || ((this->getHosts() == server.getHosts())))
		is_equal = true;
	return (is_equal);
}

std::vector<std::string>	Server::getHosts(void) const
{
	std::map<std::string, std::string>::const_iterator	it = listen.begin();
	std::vector<std::string>	hosts;

	while (it != listen.end())
	{
		hosts.push_back(it->first);
		++it;
	}
	return (hosts);
}

void Server::Mount(ConfigObject& obj)
{
	(void)obj;
	EpollManager*	epollmng = EpollManager::getEpollManager();
	std::map<std::string, std::string>::iterator	it = listen.begin();
	std::string	host;

	/* Open Server Sockets */
	int i = -1;
	while (it != listen.end())
	{
		try
		{
			/* Creating Passive Socket */
			if (startsWith(it->first, DEF_HOST))
				host = DEF_HOST;
			else
				host = it->first;
			PassiveSocket	*socket = new PassiveSocket(host, it->second);
			socket->SetRecvTimeout(RECV_TIMEOUT_SEC, RECV_TIMEOUT_USEC);
			socket->SetSendTimeout(SEND_TIMEOUT_SEC, SEND_TIMEOUT_USEC);
			socket->SetNonBlocking();
			socket->Listen();
			sockets.push_back(socket);
			/* Creating Socket Handler */
			SocketHandler	*handler = new SocketHandler(socket, this);
			epollmng->addEventHandler(handler, EPOLLIN | EPOLLHUP | EPOLLERR);
		}
		catch(const std::exception& e)
		{
			std::cerr << e.what() << std::endl;
		}
		++it;
	}

	/* Mount Locations */
	i = -1;
	while (++i < (int)resources.size())
		resources[i]->Mount(*this);
}

ConfigObject* Server::AddObject(std::vector<std::string>& info)
{
	ConfigObject *newResource = NULL;

	if (!info.empty())
	{
		if (dictTypes.find(info.at(0)) == dictTypes.end())
			Throw_Obj_Error(info.at(0));
		switch (dictTypes[info.at(0)])
		{
			case LOCATION:
			{
				if (info.size() != 2)
					Throw_Obj_Error(info.at(0));
				newResource = new Location(info.at(1));
				resources.push_back(newResource);
				return newResource;
			}
				break;
			default:
				Throw_Obj_Error(info.at(0));
		}
	}
	return NULL;
}

void	Server::evaluateListen(std::map<std::string, std::string>& target, std::vector<std::string>& info)
{
	if (info.size() != 2)
		return;

	ListenData	data(target);
	std::string	ipNum = "(25[012345]|2[01234][\\d]|1[\\d]{2,2}|[123456789][\\d]?|0)";
	std::string	port = "[\\d]+";
	RegexParser	regex(&data, "((" + ipNum + "(." + ipNum + "){3,3})~0(:(" + port + ")~1)? )~2|(" + port + " )~3");

	if (!regex.Match(info.at(1) + " "))	// Se añade espacio para asegurar que no se ha incluido un caracter final que no cumpla.
		std::cout << "Not valid listen value: " << info.at(1) << std::endl;
}

void Server::AddAttribute(std::vector<std::string>& info)
{
	if (!info.empty())
	{
		if (dictTypes.find(info.at(0)) == dictTypes.end())
			Throw_Att_Error(info.at(0));
		switch (dictTypes[info.at(0)])
		{
			case LISTEN:			evaluateListen(listen, info);			break;
			case ROOT:				Handle_Str(root, info);					break;
			case SERVER_NAME:		Handle_MultiStrings(server_name, info); break;
			case INDEX:				Handle_MultiStrings(index, info);		break;
			case TIMEOUT:			Handle_unSignedInt(_timeout, info);		break;
			case CGI_TIMEOUT:		Handle_unSignedInt(_cgi_timeout, info);	break;
			case AUTOINDEX:
			{
				if (info.size() != 2)
					Throw_Att_Error(info.at(0));
				if (!info.at(1).compare("on"))
					autoindex = true;
				break;
			}
			case CLIENT_MAX_BODY:	Handle_unSignedInt(client_max_body, info);	break;
			case ERROR_PAGE:		Handle_Map(error_pages, info);			break;
			case ADD_HEADER:		Handle_StrMap(headers, info);			break;
			default:
				Throw_Att_Error(info.at(0));
		}
	}
}

uint64_t	Server::getTimeOut(bool is_cgi)
{
	if (is_cgi)
		return (_cgi_timeout);
	else
		return (_timeout);
}

void Server::Print_Content()
{
	std::cout << "\n\tServer content:" << std::endl;
	Print_StrMap("\t\tlisten", listen, "\t\t\t");
	std::cout << "\t\troot: " + root << std::endl;
	Print_Str_Vector("\t\tserver_name", server_name);
	Print_Str_Vector("\t\tindex", index);
	std::cout << "\t\tclient_max_body_size: " << client_max_body << std::endl;
	std::cout << "\t\tautoindex: " << (autoindex ? "true" : "false") << std::endl;
	//Print_Map("\t\tError pages", error_pages, "\t\t\t");
	Print_StrMap("\t\tHeaders", headers, "\t\t\t");

	std::cout << CYAN "\t\tLocations:" RESET << std::endl;
	if (location_map.size() > 0)
	{
		for (std::map<std::string, Location*>::const_iterator it = location_map.begin(); it != location_map.end(); ++it)
    	    std::cout << "\t\t\tLocation: |" << it->first << "|" << std::endl;
	}

	int i = -1;
	while (++i < (int)resources.size())
		resources[i]->Print_Content();
}

static bool	existInResources(std::vector<ConfigObject*> resources, std::string element)
{
	bool	exists = false;
	size_t	j = -1;

	if (!resources.empty())
	{
		while (++j < resources.size())
		{
			if (((Location *)resources.at(j))->_path.compare(element) == 0)
			{
				exists = true;
				break;
			}
		}
	}
	return (exists);
}

void	Server::addDefaultLocations(void)
{
	std::vector<std::string>	locations = GetDefautlLocations();
	size_t	i = -1;
	size_t	j = -1;

	if (resources.empty())
	{
		while (++i < locations.size())
		{
			resources.push_back(new Location(locations.at(i)));
			location_map[locations.at(i)];
		}
	}
	else
	{
		while (++i < locations.size())
		{
			j = -1;
			while (++j < resources.size())
			{
				if (locations.at(i).compare(((Location *)resources.at(j))->_path) != 0
					&& !existInResources(resources, locations.at(i)))
				{
					resources.push_back(new Location(locations.at(i)));
					location_map[locations.at(i)];
				}
			}
		}
	}
}

/*
	Check Server configuration and replace empty fields with default values
*/
void	Server::CheckConfig(ConfigObject& obj)
{
	Master	*master = dynamic_cast<Master *>(&obj);
	size_t	i = -1;

	if (root.empty())
		this->root = master->default_root;
	if (index.empty())
		index.push_back(DEF_INDEX);
	if (listen.empty())
		listen[DEF_HOST] = DEF_PORT;
	if (server_name.empty())
		server_name = GetDefautlServerNames();
	if (error_pages.empty())
		error_pages = master->error_pages;
	addDefaultLocations();
	while (++i < resources.size())
		resources[i]->CheckConfig(*this);
}

std::string Server::getType()
{
	return "Server";
}

void	Server::SetDefaultConfig(ConfigObject &obj)
{
	Master	*master = dynamic_cast<Master *>(&obj);
	size_t	i = -1;

	root = master->default_root;
	server_name = GetDefautlServerNames();
	listen[DEF_HOST] = DEF_PORT;
	index.push_back(DEF_INDEX);
	error_pages = master->error_pages;
	client_max_body = master->client_max_body;
	addDefaultLocations();
	while (++i < resources.size())
		resources[i]->SetDefaultConfig(*this);
}

void	Server::applySuffixConfig(Location* location, const std::string& path, HTTP::Method method)
{
	std::map<std::string, Location *>::iterator	itr = location_map.begin();
	std::size_t	pos = path.find(".");
	std::string	suffix;
	std::string	buff;

	if (pos != STRING_NULL)
	{
		suffix = path.substr(pos);
		while (itr != location_map.end())
		{
			if (isSuffixExpansion(itr->first))
			{
				buff = itr->first;
				buff.erase(0, 2);
				if (buff.compare(suffix) == 0)
				{
					if (itr->second->isMethodAllowed(method))
					{
						location->useCGI();
						location->skipCheckMethod(true);
						location->copyCGIParams(itr->second->cgi_params);
					}
					break;
				}
			}
			++itr;
		}
	}
}

/*
	Search server for path
	@returns Location object pointer if path is valid
*/

Location*	Server::findLocation(const std::string& path, HTTP::Method method)
{
	std::map<std::string, Location *>::iterator	itr = location_map.begin();
	Location*	location = NULL;
	std::string	dirpath = Directory::getDirectoryPath(path, false);

	/* Search location map with path*/
	if (!location && location_map.find(path) != location_map.end())
		location = location_map[path];
	/* Search location map with dirpath*/
	if (!location && !dirpath.empty())
	{
		if (location_map.find(dirpath) != location_map.end())
			location = location_map[dirpath];
	}
	/* Search each location with path*/
	if (!location)
	{
		while (itr != location_map.end())
		{
			if (itr->second->find(path))
			{
				location = itr->second;
				break ;
			}
			++itr;
		}
	}
	/* Check for suffix expansions config */
	if (location)
		applySuffixConfig(location, path, method);
	return (location);
}

/*
	Checks if content type is accepted
*/
bool	Server::isMimeTypeAccepted(std::string content_type)
{
	bool accepted = false;

	if (!content_type.empty())
		accepted = accepted_mimetypes[HTTP::StrToMimeTypeId(content_type)];
	return (accepted);
}

/*
	Checks if content type is accepted
*/
bool	Server::isMimeTypeAccepted(std::vector<std::string> contents)
{
	bool accepted = false;

	if (!contents.empty())
	{
		for (size_t i = 0; i < contents.size(); i++)
		{
			if (!contents.at(i).empty())
				accepted = accepted_mimetypes[HTTP::StrToMimeTypeId(contents.at(i))];
		}
	}
	return (accepted);
}

/*
	Checks if content type is accepted
*/
bool	Server::isEncodingImplemented(std::vector<std::string> contents)
{
	bool implemented = false;

	if (!contents.empty())
	{
		for (size_t i = 0; i < contents.size(); i++)
		{
			if (!contents.at(i).empty())
				implemented = implemented_encodings[HTTP::StrToEncodingId(contents.at(i))];
		}
	}
	return (implemented);
}
