/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HTTPResponse.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yzaytoun <yzaytoun@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/27 12:08:19 by yzaytoun          #+#    #+#             */
/*   Updated: 2025/03/29 19:38:35 by yzaytoun         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "HTTPResponse.hpp"

HTTPResponse::HTTPResponse() : autoindex(false){}

HTTPResponse::HTTPResponse(const std::string port) : _port(port), autoindex(false){}

HTTPResponse::~HTTPResponse() {}

HTTPResponse::HTTPResponse(const HTTPResponse& response)
{
	*this = response;
}

HTTPResponse&	HTTPResponse::operator=(const HTTPResponse& response)
{
	if (this != &response)
	{
		_message = response._message;
		_filename = response._filename;
		_time = response._time;
	}
	return (*this);
}

/*  Setters  */
void	HTTPResponse::setMessage(const std::string& message)
{
	_headers << "HTTP/1.1 " << message << HTTP_newline;
	_message = message;
}

std::string	HTTPResponse::getMessage(void) const
{
	return(_message);
}

void	HTTPResponse::addAllowedMethods(Location& location)
{
	int	i = -1;
	int	add_methods = 0;
	int	allowed_methods = location.countAllowedMethods();

	_headers << "Allow: ";
	while (++i < HTTP::NUM_METHODS)
	{
		if (location.accepted_methods[i])
		{
			++add_methods;
			_headers << HTTP::MethodIdToString(static_cast<HTTP::Method>(i));
			_headers << ((add_methods == allowed_methods) ? HTTP_newline : ", ");
		}
	}
}

void	HTTPResponse::addBodyFromFile(const HTTPRequest *request, const std::string &path)
{
	std::string body;

	if (autoindex && Directory::isDir(path))
	{
		_body << generateAutoindex(request, path);
	}
	else
	{
		body = Directory::readFile(path);
		if (!body.empty())
		{
			ReplaceSubString(body, "#content", "An unexpected condition was encountered");
			ReplaceSubString(body, "#message", _message);
			ReplaceSubString(body, "#file_name", _filename);
		}
		_body << body;
	}
	_body << HTTP_newline;
}

void	HTTPResponse::addBody(const std::string content, bool add_newline)
{
	_body << content;
	if (add_newline)
		_body << HTTP_newline;
}

std::string	HTTPResponse::generateAutoindex(const HTTPRequest *request, const std::string &path)
{
	Directory dir(Directory::getRealPath(path));	
	std::vector<std::pair<std::string,struct stat> > listdir = dir.listAllDir();
	std::stringstream	body;
	std::string uri = "";
	bool json = false;
	if (request)
	{
		uri = request->params[HTTP::PATH];
		const std::vector<std::string>& accept_values = request->headers[HTTP::Accept];

		for (size_t i = 0; i < accept_values.size(); ++i) {
			if (accept_values[i] == ("application/json")) {
				json = true;
				break;
			}
    	}
	}

	if (json)
	{
		body << "[\n";
		for (size_t i = 0; i < listdir.size(); ++i)
		{
			std::string name = listdir[i].first;
			const struct stat& info = listdir[i].second;
			bool is_dir = S_ISDIR(info.st_mode);

			body << "  {\n"
				<< "    \"name\": \"" << name << "\",\n"
				<< "    \"is_dir\": " << (is_dir ? "true" : "false") << ",\n"
				<< "    \"size\": " << (is_dir ? 0 : info.st_size) << "\n"
				<< "  }";

			if (i < listdir.size() - 1)
				body << ",";
			body << "\n";
		}
		body << "]";
	} else {
		body << "<!DOCTYPE html>\n"
			<< "<html>\n"
			<< "<head>\n"
			<< "    <meta charset=\"UTF-8\">\n"
			<< "    <title>Index of " << uri << "</title>\n"
			<< "    <style>\n"
			<< "        body { font-family: Arial; padding: 2em; background-color: #f9f9f9; }\n"
			<< "        h1 { color: #333; }\n"
			<< "        ul { list-style-type: none; padding: 0; }\n"
			<< "        li { margin: 0.5em 0; }\n"
			<< "        a { color: #4CAF50; text-decoration: none; }\n"
			<< "        a:hover { text-decoration: underline; }\n"
			<< "    </style>\n"
			<< "</head>\n"
			<< "<body>\n"
			<< "    <h1>Index of " << uri << "</h1>\n"
			<< "    <ul>\n";
		// List items in directory
		for (size_t i = 0; i < listdir.size(); i++)
		{
			std::string name = listdir[i].first;
			bool is_dir = S_ISDIR(listdir[i].second.st_mode);
			std::string display_name = name + (is_dir ? "/" : "");
			std::string href = uri + "./" + name;
			body << "        <li><a href=\"" << href << "\">" << display_name << "</a></li>\n";
			// body << "<li><a href=\"" << listdir[i].first << "> " << listdir[i].first << "</a></li>";
		}
		body << "    </ul>\n"
			<< "</body>\n"
			<< "</html>\n";
	}

	return (body.str());
}

/* Getters */
std::string	HTTPResponse::getCurrentTime(void) const
{
	return _time.getDateTimeString(HTTP_DEF_FORMAT);
}

std::string	HTTPResponse::getCurrentTime(const int64_t offset, Time::Period period)
{
	switch (period)
	{
	case Time::DAY:
		_time.addDays(offset);
		break;
	case Time::MONTH:
		_time.addMonths(offset);
		break;
	case Time::YEAR:
		_time.addYears(offset);
		break;
	default:
		break;
	}
	return _time.getDateTimeString(HTTP_DEF_FORMAT);
}

void	HTTPResponse::handleMethod(HTTPRequest& request, const std::string path)
{
	switch (request.method)
	{
		case HTTP::MethodGET:
		case HTTP::MethodHEAD:		handleGet(request, path);		break;
		case HTTP::MethodPOST:		handlePost(request, path);	 	break;
		case HTTP::MethodPUT:		handlePut(request, path);	 	break;
		case HTTP::MethodDELETE:	handleDelete(request, path);	break;
		default:
			break;
	}
}

/*  Methods  */
void	HTTPResponse::handleGet(HTTPRequest& request, const std::string& path)
{
	std::string	get_path;

	if (Directory::isFile(path) || (Directory::isDir(path) && autoindex))
		get_path = path;
	else
		get_path = Directory::getRealPath(DOCROOT DEF_INDEX);
	if (!Directory::checkPermissions(get_path, R_OK))
		throw HTTPException(HTTP::FORBIDDEN);
	this->setMessage(HTTP::GetCodeMessage(HTTP::OK));
	this->addHeader("Content-type", Directory::getFileType(get_path));
	if (request.method == HTTP::MethodGET)
	{
		this->addHeader("Last-Modified", Directory::getLastModification(get_path));
		this->addBodyFromFile(&request, get_path);
	}
}

void	HTTPResponse::handlePost(HTTPRequest& request, const std::string& path)
{
	std::string	fullpath = path;

	if (request.params[HTTP::CONTENT].empty())
		throw HTTPException(HTTP::BAD_REQUEST);
	/* Handle Attachments */
	if (!request.params[HTTP::FILE_NAME].empty())
		_filename = request.params[HTTP::FILE_NAME];
	else if (!request.params[HTTP::FILE_PATH].empty())
		_filename = request.params[HTTP::FILE_PATH];
	if (!_filename.empty())
	{
		fullpath = path + "/" + _filename;
		ReplaceSubString(fullpath, "//", "/");
		if (Directory::pathExists(fullpath))
			throw HTTPException(HTTP::CONFLICT);
		Directory::writeToFile(fullpath, request.params[HTTP::CONTENT]);
		if (Directory::pathExists(fullpath))
			this->setMessage(HTTP::GetCodeMessage(HTTP::CREATED));
		else
			throw HTTPException(HTTP::INTERNAL_SERVER_ERROR);
		this->addHeader("Content-Location", fullpath);
		this->addHeader("Content-type", Directory::getFileType(fullpath));
		this->addBodyFromFile(&request, fullpath);
	}
	/* Handle Appends */
	else if (Directory::pathExists(fullpath) && !Directory::isDir(fullpath))
	{
		if (!Directory::checkPermissions(fullpath, W_OK))
			throw HTTPException(HTTP::FORBIDDEN);
		Directory::writeToFile(fullpath, request.params[HTTP::CONTENT]);
		this->setMessage(HTTP::GetCodeMessage(HTTP::OK));
		this->addHeader("Content-Location", fullpath);
		this->addHeader("Content-type", Directory::getFileType(fullpath));
		this->addBodyFromFile(&request, fullpath);
	}
	/* Handle Inline */
	else
	{
		this->setMessage(HTTP::GetCodeMessage(HTTP::OK));
		this->addHeader("Content-type", request.getHeaderVal(HTTP::ContentType));
		this->addBodyFromFile(&request, request.params[HTTP::CONTENT]);
	}
}
	
void	HTTPResponse::handleDelete(HTTPRequest& request, const std::string& path)
{
	const std::vector<std::string>	def_locations = GetDefautlLocations();

	if (vecSearch(def_locations ,request.getLocation()))
		throw HTTPException(HTTP::FORBIDDEN);
	if (std::remove(path.c_str()) < 0)
		throw HTTPException(HTTP::NOT_FOUND);
	this->setMessage(HTTP::GetCodeMessage(HTTP::OK));
	_filename = request.getLocation();
	this->addBodyFromFile(&request, Directory::getRealPath(DOCROOT DEF_DELETE_SUCCESS));
}

void	HTTPResponse::handlePut(HTTPRequest& request, const std::string& path)
{
	std::string	fullpath = path;
	bool		skip = false;

	if (request.params[HTTP::CONTENT].empty())
		throw HTTPException(HTTP::BAD_REQUEST);
	if (!request.params[HTTP::FILE_PATH].empty())
	{
		_filename = request.params[HTTP::FILE_PATH];
		fullpath = path + "/" + _filename;
		ReplaceSubString(fullpath, "//", "/");
		if (Directory::pathExists(fullpath))
		{
			if (Directory::isDir(fullpath) || !Directory::checkPermissions(fullpath, W_OK))
				throw HTTPException(HTTP::FORBIDDEN);
			if (!request.getHeaderVal(HTTP::ContentType).empty()
				&& request.getHeaderVal(HTTP::ContentType).compare(Directory::getFileType(fullpath)) != 0)
				throw HTTPException(HTTP::CONFLICT);
			this->setMessage(HTTP::GetCodeMessage(HTTP::OK));
			skip = true;
		}
		Directory::writeToFile(fullpath, request.params[HTTP::CONTENT], std::ios::binary | std::ios::trunc);
		if (!skip && Directory::pathExists(fullpath))
			this->setMessage(HTTP::GetCodeMessage(HTTP::CREATED));
		this->addHeader("Content-Location", fullpath);
		this->addHeader("Content-type", Directory::getFileType(fullpath));
		this->addBodyFromFile(&request, fullpath);
	}
	else
		throw HTTPException(HTTP::FORBIDDEN);
}

bool HTTPResponse::isRedirection(const Location & location)
{
	if (location.returns.size() == 2 && 
		location.returns[0].size() == 3 && 
		(location.returns[0] == "300" || 
		location.returns[0] == "301" || 
		location.returns[0] == "302" || 
		location.returns[0] == "303" || 
		location.returns[0] == "304" || 
		location.returns[0] == "307" || 
		location.returns[0] == "308"))
		return true;
	return false;
}

void HTTPResponse::handleRedirection(const std::string status_redirection, const std::string new_path)
{
	this->setMessage(status_redirection + HTTP::StatusCodeToString(HTTP::StrToStatusCode(status_redirection)));
	this->addHeader("Location", new_path);
}

void	HTTPResponse::handleError(std::map<int, std::string> error_pages)
{
	if (!error_pages.empty())
		addBodyFromFile(NULL, error_pages[std::atoi(ExtractDigits(_message).c_str())]);
	else
		addBodyFromFile(NULL, Directory::getRealPath(DOCROOT DEF_ERROR_ROOT DEF_ERROR_PAGE));
}

void	HTTPResponse::generateFromRequest(HTTPRequest& request, Server* server)
{
	Location*					location = request.getLocationPointer();
	std::map<int, std::string>	error_pages;

	try
	{
		if (!location)
			throw HTTPException(HTTP::NOT_FOUND);
		if (!location->getCookies().empty())
			_cookies = location->getCookies();
		if (!location->returns.empty())
		{
			// Location have 3xx configurations.
			if (isRedirection(*location))
			{
				handleRedirection(location->returns[0], location->returns[1]);
				return;
			}
			else
				throw HTTPException(HTTP::StrToStatusCode(location->returns.at(0)));
		}
		if (server->autoindex || location->autoindex)
			this->autoindex = true;
		handleMethod(request, request.params[HTTP::FULL_PATH]);
	}
	catch(const std::exception& e)
	{
		_headers.clear();
		setMessage(e.what());
		if (location)
			error_pages = location->error_pages;
		handleError(error_pages);
	}
}

void	HTTPResponse::closeHeaders(void)
{
	addHeader("Date", getCurrentTime());
	if (!_cookies.empty())
	{
		for (std::size_t i = 0; i < _cookies.size(); i++)
		{
			this->addHeader("Set-Cookie", _cookies[i]);
		}
	}
	_headers << HTTP_newline;
}

void	HTTPResponse::sendChunked(ActiveSocket *socket, std::string& content, std::size_t content_size)
{
	std::stringstream	sso;
	std::size_t 		i = 0;

	if(content_size > BUFSIZ)
	{
		for (; i < content_size; i += HTTP::CHUNK_SIZE)
		{
			sso << std::hex << HTTP::CHUNK_SIZE << HTTP_newline << content.substr(i, HTTP::CHUNK_SIZE) << HTTP_newline;
			socket->Send(sso.str());
		}
		if (i > content_size)
		{
			sso << std::hex << (i - content_size) << HTTP_newline << content.substr(i - content_size) << HTTP_newline;
			socket->Send(sso.str());
		} 
		socket->Send("0\r\n");
	}
	else
		socket->Send(content);
}

void	HTTPResponse::transferResponse(ActiveSocket *socket, bool write_headers, std::size_t content_length)
{
	std::size_t			content_size = 0;
	std::string			content = _body.str();

	Print(RED "\n*---------- Response ----------*" RESET);
	if (write_headers)
	{
		if (content_length > 0)
			content_size = content_length;
		else
			content_size = _body.tellp();
		if (content_size > 0)
			addHeader("Content-Length", content_size);
		closeHeaders();
		Print(_headers.str());
		socket->Send(_headers.str() + content);
	}
}

void	HTTPResponse::addFullHeader(const std::string content)
{
	_headers << content << HTTP_newline;
};
