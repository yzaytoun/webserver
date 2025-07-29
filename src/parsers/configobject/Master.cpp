#include "Master.hpp"

bool	isValidConfigFile(std::string& path)
{
	const size_t	pos = path.find(".");
	bool			is_valid = false;

	if (pos != std::string::npos)
	{
		if (path.substr(pos).compare(".conf") == 0)
			is_valid = true;
	}
	return (is_valid);
}

void Master::Innit(std::string configPath)
{
	AutomataReader a;
	bool	is_valid = isValidConfigFile(configPath);

	dictTypes["server"] = SERVER;
	dictTypes["client_max_body_size"] = CLIENT_MAX_BODY;
	dictTypes["error_page"] = ERROR_PAGE;
	if (!configPath.empty() && is_valid)
		a.read_config_file(this, configPath);
	else if (!configPath.empty() && !is_valid)
	{
		std::cerr << "invalid config file!!" << "\n *** Exiting webserv ***" << std::endl;
		exit(EXIT_FAILURE);
	}
	else
		this->SetDefaultConfig(*this);
	_epollManager = EpollManager::getEpollManager();
}

Master::Master() : client_max_body(0)
{
	Innit();
}

Master::Master(std::string configPath) : client_max_body(0)
{
	Innit(configPath);
}

Master::~Master()
{
	EpollManager::Destroy();
	RequestParser::destroyInstance();
	if (!resources.empty())
	{
		for (size_t i = 0; i < resources.size(); i++)
			delete resources.at(i);
		resources.clear();
	}
}

void Master::Run()
{
	/*
	* Main loop
	*/
	while (true) {
		/*
		* Check for timeout
		*/
		for (size_t i = 0; i < _epollManager->handlers.size(); i++)
		{
			if (_epollManager->handlers[i]->hasTimeOut())
			{
				_epollManager->modifyEventHandler(_epollManager->handlers[i], EPOLLOUT);
				_epollManager->handlers[i]->RemoveHandler = true;
			}
		}
		size_t num_events = static_cast<size_t >(_epollManager->waitEvents(EPOLL_WAIT_TIME_MS));
		/*
		* loop by active events
		*/
    	for (size_t i =  0; i < num_events; ++i) {
    	    uint32_t event_type = _epollManager->getEventsType(i);
    	    IEventHandler* handler = _epollManager->getEventHandler(i);
			
			if (!handler->deleted)
			{
    	    	if (event_type & EPOLLHUP && !handler->RemoveHandler) {
    	    	    handler->handleHangUp();
    	    	}
				if (event_type & EPOLLERR && !handler->RemoveHandler) {
    	    	    handler->handleError();
    	    	}
				if (event_type & EPOLLIN && !handler->RemoveHandler) {
    	    	    handler->handleRead();
    	    	}
    	    	if (event_type & EPOLLOUT && !handler->RemoveHandler) {
    	    	    handler->handleWrite();
    	    	}
				if (event_type & ~(EPOLLIN|EPOLLOUT|EPOLLERR|EPOLLHUP)) {
					handler->handleGeneric();
				}
				if (handler->RemoveHandler) {
					if (handler->has_timeout)
    	    	    	handler->handleWrite();
					handler->deleted = true;
					_epollManager->removeEventHandler(handler);
				}
			}
    	}
    }
}

bool	Master::checkSimilarConfig(void)
{
	bool	is_similar = false;
	size_t	server_idx = -1;
	size_t	j;

	if (!servers.empty() && servers.size() > 1)
	{
		while (!is_similar && ++server_idx < servers.size())
		{
			j = -1;
			while (++j < servers.size())
			{
				if (j != server_idx
					&& ((servers.at(j)->getHosts() == servers.at(server_idx)->getHosts())
					|| (servers.at(j)->server_name == servers.at(server_idx)->server_name))
				)
				{
					is_similar = true;
					break;
				}
			}
		}
	}
	return (is_similar);
}

void Master::Mount(ConfigObject &obj)
{
	(void)obj;
	size_t i = -1;

	this->CheckConfig(*this);
	if (this->checkSimilarConfig())
		ThrowCustomError(RED "Error: Servers have similar configration" RESET "\nExiting webserv!");
	while (++i < resources.size())
		resources[i]->Mount(*this);
}

ConfigObject* Master::AddObject(std::vector<std::string>& info)
{
	if (!info.empty())
	{
		if (dictTypes.find(info.at(0)) == dictTypes.end())
			Throw_Obj_Error(info.at(0));
		switch (dictTypes[info.at(0)])
		{
			case SERVER:
			{
				ConfigObject *newResource = new Server();
				servers.push_back(dynamic_cast<Server*>(newResource));
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

void Master::AddAttribute(std::vector<std::string>& info)
{
	if (!info.empty())
	{
		if (dictTypes.find(info.at(0)) == dictTypes.end())
			Throw_Att_Error(info.at(0));
		switch (dictTypes[info.at(0)])
		{
			case CLIENT_MAX_BODY:	Handle_unSignedInt(client_max_body, info); break;
			case ERROR_PAGE:		Handle_Map(error_pages, info);		break;
			case SERVER:
				Throw_Att_Error(info.at(0));
		}
	}
}

void Master::Print_Content()
{
	std::cout << "Master content:" << std::endl;
	std::cout << "\tclient_max_body: " << client_max_body << std::endl;
	Print_Map("\tError Pages", error_pages, "\t\t");

	int i = -1;
	while (++i < (int)resources.size())
		resources[i]->Print_Content();
}

void Master::CheckConfig(ConfigObject& obj)
{
	(void)obj;
	int	i = -1;

	if (default_root.empty())
		default_root = DOCROOT;
	while (++i < (int)resources.size())
		resources[i]->CheckConfig(*this);
}

std::string Master::getType()
{
	return "Master";
}

void	Master::SetDefaultConfig(ConfigObject& obj)
{
	(void)obj;

	default_root = DOCROOT;
	resources.push_back(new Server());
	resources[0]->SetDefaultConfig(*this);
}
