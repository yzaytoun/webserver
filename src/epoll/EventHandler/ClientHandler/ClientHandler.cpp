/*
 * By: pedromar <pedromar@student.42madrid.com>
 * Created on: 2024-12-24
 * Last modified: 2024-12-24
 */

#include "ClientHandler.hpp"
#include "HTTPCGI.hpp"

ClientHandler::ClientHandler() : _server(NULL), _socket(NULL), _request(NULL)
{
	this->RemoveHandler = false;
	this->has_timeout = false;
	this->deleted = false;
	this->_timeout = DEF_TIMEOUT;
}

ClientHandler::ClientHandler(Server *server, ActiveSocket* clientsocket, u_int64_t timeout)
	: _server(server), _socket(clientsocket)
{
	this->fileDescriptor = clientsocket->GetSocketDescriptor();
	this->RemoveHandler = false;
	this->deleted = false;
	this->_timeout = timeout;
	this->_timer.Initialize();
	_request = new HTTPRequest();
	if (_request)
	{
		_request->addServer(_server);
		if (_server->client_max_body)
			_request->setMaxBody(_server->client_max_body);
	}
}

ClientHandler::~ClientHandler()
{
	if (_request)
		delete _request;
	if (_socket)
		delete _socket;
}

void ClientHandler::handleWrite()
{
	HTTPResponse	response(_socket->GetPort());
	ProcessHandler*	process = NULL;

    if (_request->getState() == HTTP::REQUEST_ERROR)
	{
		response.setMessage(_error_message);
		if (_request->flag[HTTP::IS_METHOD_NOT_ALLOWED] && _request->getLocationPointer())
			response.addAllowedMethods(*_request->getLocationPointer());
		response.handleError(_server->error_pages);
		response.transferResponse(_socket);
	}
	else if (_request->getState() == HTTP::REQUEST_HEADERS_END)
	{
		if (!_request->flag[HTTP::HAS_BODY])
			std::cout << *_request << std::endl;
		_request->getLocationPointer()->setCGIConfig(*_request);
		if (_request->flag[HTTP::IS_CGI] && !_request->getLocationPointer()->getCGIVariable(HTTP::CgiScriptName).empty())
		{
			process = new ProcessHandler(_socket, _server->getTimeOut(true));
			process->fileDescriptor = process->GetProcess()->GetInputFd();
			EpollManager::getEpollManager()->addEventHandler(process, EPOLLIN);
			process->GetProcess()->SendInput(_request->params[HTTP::CONTENT]);
			process->checkChunkedInput(_request->flag[HTTP::IS_CHUNKED], _request->params[HTTP::FULL_CHUNKED_BODY].length());
			process->run(_request->getLocationPointer());
		}
		else
		{
			response.generateFromRequest(*_request, _server);
			response.transferResponse(_socket);
		}
	}
	EpollManager::getEpollManager()->modifyEventHandler(this, EPOLLIN | EPOLLHUP | EPOLLERR);
	if (_request->flag[HTTP::CLOSE_CONNECTION])
		this->RemoveHandler = true;
	_request->clear();
};

void ClientHandler::handleRead()
{
	std::string 	request_str;
	ssize_t			bytes_read = this->_socket->Recv(request_str);

	/* Process message */
	try
	{
		if (bytes_read)
		{
			if (!_timer.isRunning())
				_timer.SetStartTime();
			_request->append(request_str);
			if (_request->getState() != HTTP::REQUEST_HEADERS_END
				&& _request->getState() != HTTP::REQUEST_ERROR && _request->hasHeaderSeparator())
				RequestParser::getInstance()->processHeaders(_request->params[HTTP::HEADERS_STRING], *_request);
			if (_request->getState() == HTTP::REQUEST_HEADERS_END && _request->flag[HTTP::HAS_BODY])
			{
				if (_request->parseBody()
					&& (!_request->flag[HTTP::IS_CHUNKED]
						|| (_request->flag[HTTP::IS_CHUNKED] && _request->flag[HTTP::IS_LAST_CHUNK])))
					_request->flag[HTTP::HAS_BODY] = false;
			}
			if (hasTimeOut())
				throw HTTPException(HTTP::REQUEST_TIMEOUT);
		}
	}
	catch(const std::exception& e)
	{
		_request->setState(HTTP::REQUEST_ERROR);
		_error_message = e.what();
	}
	if ((_request->getState() == HTTP::REQUEST_HEADERS_END && !_request->flag[HTTP::HAS_BODY])
		|| (_request->getState() == HTTP::REQUEST_HEADERS_END && _request->flag[HTTP::IS_COMPLETE_CHUNK])
		|| _request->getState() == HTTP::REQUEST_ERROR)
	{
		EpollManager::getEpollManager()->modifyEventHandler(this, EPOLLIN | EPOLLHUP | EPOLLERR | EPOLLOUT);
		_timer.SetEndTime();	
	}
};

void ClientHandler::handleError()
{
	this->RemoveHandler = true;
};

void ClientHandler::handleHangUp()
{
	std::cerr << "Client (" << fileDescriptor << ") hanged up connection" << std::endl;
	this->RemoveHandler = true;
};

void ClientHandler::handleGeneric()
{};

bool	ClientHandler::hasTimeOut()
{
	has_timeout = ((_timer.isRunning() && _timer.TimeDiffFromCurrent() >= _timeout) || _timer.GetSeconds() >= _timeout);
	if (has_timeout)
	{
		_request->setState(HTTP::REQUEST_ERROR);
		_error_message = HTTP::GetCodeMessage(HTTP::REQUEST_TIMEOUT);
	}
	return (has_timeout);
}
