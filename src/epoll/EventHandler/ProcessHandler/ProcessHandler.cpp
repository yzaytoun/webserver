/*
 * By: pedromar <pedromar@student.42madrid.com>
 * Created on: 2024-12-24
 * Last modified: 2024-12-24
 */

#include "ProcessHandler.hpp"

ProcessHandler::ProcessHandler() : _socket(NULL), _success(false), _write_headers(false)
{
	this->_epollmng = EpollManager::getEpollManager();
	has_timeout = false;
	deleted = false;
	_timeout = DEF_TIMEOUT;
}

ProcessHandler::ProcessHandler(ActiveSocket *socket, int64_t timeout) :  _socket(socket), _success(false), _write_headers(false)
{
	 _process = new Process();

	this->RemoveHandler = false;
	this->_epollmng = EpollManager::getEpollManager();
	has_timeout = false;
	deleted = false;
	_timeout = timeout;
	_timer.Initialize();
	if (_process->GetInputFd() == -1 || _process->GetOutFd() == -1) {
		  delete _process;
		  _process = NULL; 
		  return ;
	}

	// Register processHandler for read child process output
	this->fileDescriptor = _process->GetOutFd();
	EpollManager::getEpollManager()->addEventHandler(this, EPOLLIN);
}

Process*  ProcessHandler::GetProcess(void) const
{
	return (_process);
}

void 	ProcessHandler::checkChunkedInput(bool is_chunked, const std::size_t content_len)
{
	_is_chunked = is_chunked;
	_content_len = content_len;
}

void	ProcessHandler::clear(bool is_full_reset)
{
	_content.clear();
	_success = false;
	_bytes_read = 0;
	if (is_full_reset)
	{
		_envp.clear();
		_args.clear();
		_write_headers = false;
	}
}

void  ProcessHandler::run(Location* location)
{
	std::string	cgi_var;

	if (_args.empty())
	{
		for (int i = 0; i < HTTP::NUM_CGI_VARIABLE; i++)
		{
			cgi_var = location->getCGIVariable(static_cast<HTTP::CgiVarible>(i), true);
			if (!cgi_var.empty() && !ContainsSubstr(cgi_var, "SCRIPT"))
				_envp.push_back(cgi_var);
		}
		if (Directory::getFileType(location->getCGIVariable(HTTP::CgiScriptName)).compare("application/py") == 0)
			_args.push_back("/usr/bin/python3");
		_args.push_back(Directory::getRealPath(location->getCGIVariable(HTTP::CgiScriptName)));
	}
	try
	{
		if (!_timer.isRunning())
			_timer.SetStartTime();
		_success = _process->ExecProcess(_args[0], _args, _envp);
	}
	catch(const std::exception& e)
	{
		_success = false;
		this->fileDescriptor = _socket->GetSocketDescriptor();
		EpollManager::getEpollManager()->modifyEventHandler(this, EPOLLOUT);
	}
}

ProcessHandler::~ProcessHandler()
{
	delete _process;
}

void ProcessHandler::handleWrite()
{
	HTTPResponse				response;
	std::stringstream			ss;
	std::string					header;
	std::size_t					header_sep = _content.find(HTTP_double_newline);
	int							status = 0;

	if (has_timeout)
	{
		response.setMessage(HTTP::GetCodeMessage(HTTP::REQUEST_TIMEOUT));
		response.addBodyFromFile(NULL, Directory::getRealPath(DOCROOT DEF_ERROR_ROOT DEF_ERROR_PAGE));
	}
	else if (_success && !_write_headers)
	{
		if (header_sep != STRING_NULL)
		{
			ss << _content.substr(0, header_sep);
			_content = _content.substr(header_sep + HTTP_double_newline.length());
			while (getline(ss, header))
			{
				RemoveChar(header, isCRLF);
				if (startsWith(header, "Status"))
				{
					status = std::atoi(ExtractDigits(header).c_str());
					response.setMessage(HTTP::GetCodeMessage(static_cast<HTTP::StatusCode>(status)));
				}
				else
					response.addFullHeader(header);
			}
		}
		if (!_content.empty())
			response.addBody(_content, false);
		if (response.getMessage().empty())
			response.setMessage(HTTP::GetCodeMessage(HTTP::OK));
		if (_is_chunked)
			response.addHeader("Transfer-Encoding", HTTP::EncodingIdToString(HTTP::chunked));
	}
	else if (!_success)
	{
		response.setMessage(HTTP::GetCodeMessage(HTTP::INTERNAL_SERVER_ERROR));
		response.addBodyFromFile(NULL, Directory::getRealPath(DOCROOT DEF_ERROR_ROOT DEF_ERROR_PAGE));
		_write_headers = false;
		_content_len = 0;
	}
	if (!_write_headers)
		_write_headers = true;
	response.transferResponse(_socket, _write_headers, _content_len);
	this->RemoveHandler = true;
}

void ProcessHandler::handleRead()
{
	_bytes_read = _process->ReadOutput(_content);

	this->fileDescriptor = _process->GetOutFd();
	EpollManager::getEpollManager()->removeEventHandler(this, false);
	if (_bytes_read)
	{
		this->fileDescriptor = _socket->GetSocketDescriptor();
		EpollManager::getEpollManager()->modifyEventHandler(this, EPOLLOUT);
	}
};

void ProcessHandler::handleError() {
};

void ProcessHandler::handleHangUp() {
};

void ProcessHandler::handleGeneric() {
};

bool ProcessHandler::hasTimeOut()
{
	has_timeout = ((_timer.isRunning() && _timer.TimeDiffFromCurrent() >= _timeout) || _timer.GetSeconds() >= _timeout);
	return (has_timeout);
}
