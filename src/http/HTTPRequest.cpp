#include "HTTPRequest.hpp"

HTTPRequest::HTTPRequest() : _state(HTTP::REQUEST_CONTINUE),
	_max_body_size(RECV_BUFFER_SIZE), _locationptr(NULL), _server(NULL), _bodytype(HTTP::NUM_REQUEST_BODY), _header_sep_pos(STRING_NULL), _chunk_size(STRING_NULL)
{
	for (int i = 0; i < HTTP::REQUEST_FLAG_NUM; i++)
		flag[i] = false;
}

HTTPRequest::~HTTPRequest() {}

std::ostream& operator<<(std::ostream& os, const HTTPRequest& request)
{
	std::cout << GREEN "\n*--------* Request *--------*\n" RESET << std::endl;
	os << "HTTP Request:" << std::endl;
	os << "  Method: " << HTTP::MethodIdToString(request.method) << std::endl;
	os << "  URI: " << "|" << request.getLocation() << "|" << std::endl;
	
	int	i = 0;
	std::vector<std::string>::const_iterator it;
	os << "  Headers:" << std::endl;
	while (i < HTTP::NUM_HEADERS)
	{
		for (it = request.headers[i].begin(); it != request.headers[i].end(); it++)
		{
			std::cout << "\t" << HTTP::RequestHeaderIdToString(static_cast<HTTP::RequestHeader>(i)) << "\t" << *it << std::endl;
		}
		++i;
	}
	if (!request.params[HTTP::BODY_STRING].empty() && !request.flag[HTTP::IS_CHUNKED])
		os << " Body: \n" << request.params[HTTP::BODY_STRING] << std::endl;
	else if (!request.params[HTTP::CONTENT].empty())
		os << " Chunked Body of size: " << request.params[HTTP::CONTENT].length() << std::endl;
	else
		os << " No Body " << std::endl;
	return os;
}

std::string  HTTPRequest::getLocation(void) const
{
	std::string	location;

	if ((method == HTTP::MethodPUT || method == HTTP::MethodPOST) && !params[HTTP::DIR_PATH].empty())
		location = params[HTTP::DIR_PATH];
	else
		location = params[HTTP::PATH];
	return location;
}

std::vector<std::string>  HTTPRequest::getArguments(void) const
{
	return arguments;
}

std::vector<std::string>  HTTPRequest::getEnvironmentVariables(void) const
{
	return enviorment_variables;
}

void    HTTPRequest::setState(HTTP::HTTPResult state)
{
	_state = state;
}

HTTP::HTTPResult HTTPRequest::getState(void) const
{
	return (_state);
}

void	HTTPRequest::setTimer(void)
{
	_timer.Initialize();
	_timer.SetStartTime();
}

std::string	HTTPRequest::getTime(void)
{
	std::ostringstream	stream;

	_timer.SetEndTime();
	stream << "Time" << _timer.GetMicroSeconds();

	return (stream.str());
}

void	HTTPRequest::setMaxBody(std::size_t max_body_size)
{
	_max_body_size = max_body_size;
}

void	HTTPRequest::addLocationPointer(Location* location)
{
	flag[HTTP::IS_CGI] = location->isCGI(params[HTTP::FULL_PATH]);
	_locationptr = location;
}

void	HTTPRequest::addServer(Server* server)
{
	_server = server;
}

HTTP::Encoding	HTTPRequest::getEncoding(void)
{
	HTTP::Encoding	encoding = HTTP::NUM_HTTP_ENCODING;

	if (!headers[HTTP::TransferEncoding].empty())
		encoding = HTTP::StrToEncodingId(headers[HTTP::TransferEncoding].at(0));
	return (encoding);
}

std::string	HTTPRequest::getHeaderVal(HTTP::RequestHeader header_id, const int idx)
{
	std::string	val;
	std::vector<std::string> header;

	if (inRange(header_id, 0, HTTP::NUM_HEADERS - 1))
	{
		header = headers[header_id];
		if (!header.empty() && inRange(idx, 0, header.size()))
			val = header.at(idx);
	}
	return (val);
}

/*
	******************* RegexParsr Setters *********************** 
*/

// Default Setter
void SetterHTTPRequest::setDefaultSetter(std::string str)
{
	(void)str;
	throw std::domain_error("Imposible action");
}

// Method
#define XX(MethodIdentifier, MethodString) \
void SetterHTTPRequest::setMethodName##MethodIdentifier(std::string str) {(void)str; RequestParser::getInstance()->request->method = HTTP::MethodIdentifier; }
	HTTP_METHOD(XX)
#undef XX

void SetterHTTPRequest::setNotImplementedMethod(std::string str)
{
	(void)str;
	throw HTTPException(HTTP::NOT_IMPLEMENTED);
}

// URI
//// Schema
void SetterHTTPRequest::setSchemaUri(std::string str)
{
	if (str != "http")
		throw HTTPException(HTTP::NOT_IMPLEMENTED);
	RequestParser::getInstance()->request->scheme = HTTP::SchemaHttp;

	// default port
	RequestParser::getInstance()->request->params[HTTP::PORT] = "80";
}

//// UserInfo
void SetterHTTPRequest::setUserInfoUri(std::string str)
{
	RequestParser::getInstance()->request->params[HTTP::USER_INFO] = str;
}

//// host
void SetterHTTPRequest::setHostUri(std::string str)
{
	RequestParser::getInstance()->request->params[HTTP::HOST_PARAM] = str;
}

//// port
void SetterHTTPRequest::setPortUri(std::string str)
{
	unsigned int port = std::atoi(str.c_str());
	if (port > 665535)
		throw HTTPException(HTTP::BAD_REQUEST);
	RequestParser::getInstance()->request->params[HTTP::PORT] = str;
}

//// Authority
void SetterHTTPRequest::setAuthorityUri(std::string str)
{
	RequestParser::getInstance()->request->params[HTTP::AUTHORITY] = str;
}

//// Path
void SetterHTTPRequest::setPathUri(std::string str)
{
	std::size_t	pos = str.find_last_of('/');
	
	RequestParser::getInstance()->request->params[HTTP::PATH] = str;
	if (pos != STRING_NULL)
	{
		RequestParser::getInstance()->request->params[HTTP::DIR_PATH] = str.substr(0, pos + 1);
		RequestParser::getInstance()->request->params[HTTP::FILE_PATH] = str.substr(pos + 1);
	}
}

//// Query
void SetterHTTPRequest::setQueryUri(std::string str)
{
	RequestParser::getInstance()->request->params[HTTP::QUERY] = str;
}

//// Fragment
void SetterHTTPRequest::setFragmentUri(std::string str)
{
	RequestParser::getInstance()->request->params[HTTP::FRAGMENT] = str;
}

// Protocol
void SetterHTTPRequest::firstLineChecker(std::string str)
{
	(void)str;
	HTTPRequest*	request = RequestParser::getInstance()->request;
	Server*			server = request->getServer();
	Location*		locationptr = NULL;
	
	if (server)
	{
		locationptr = server->findLocation(request->getLocation(), request->method);
		if (locationptr == NULL)
			throw HTTPException(HTTP::NOT_FOUND);
		request->params[HTTP::FULL_PATH] = locationptr->getRealPath(request->getLocation());
		if (!Directory::pathExists(request->params[HTTP::FULL_PATH]))
			throw HTTPException(HTTP::NOT_FOUND);
		request->addLocationPointer(locationptr);
		if (locationptr->checkMethod() && !locationptr->isMethodAllowed(request->method))
		{
			request->flag[HTTP::IS_METHOD_NOT_ALLOWED] = true;
			throw HTTPException(HTTP::METHOD_NOT_ALLOWED);
		}
	}
}


#define XX(HeadersIdentifier, HeadersString) \
void SetterHTTPRequest::setHeaderName##HeadersIdentifier(std::string str) {(void)str; RequestParser::getInstance()->request->header_index = HTTP::HeadersIdentifier; }
	HTTP_HEADERS(XX)
#undef XX

void SetterHTTPRequest::setHeaderNameUnknown(std::string str) {(void)str;  
	RequestParser::getInstance()->request->header_index = HTTP::UnknownHeader; }

void SetterHTTPRequest::setHeaderValue(std::string str)
{
	HTTPRequest *Req = RequestParser::getInstance()->request;

	if (inRange(Req->header_index, 0, HTTP::NUM_HEADERS) || Req->header_index == HTTP::UnknownHeader)
	{
		switch (Req->header_index)
		{
		// Ignore Unknown Headers
		case HTTP::UnknownHeader:
			break;
		// Save the value of the last occurrence
		default:
			Req->headers[Req->header_index].push_back(str);
			break;
		}
	}
}

// End Request
void SetterHTTPRequest::setEndRequest(std::string str)
{
	(void)str;
	HTTPRequest	*request = RequestParser::getInstance()->request;
	Server		*server = request->getServer();
	bool		contains_chunked = vecSearch(request->headers[HTTP::TransferEncoding], HTTP::EncodingIdToString(HTTP::chunked));

	/* Body Checkers */
	if (!request->headers[HTTP::ContentLength].empty() && !request->headers[HTTP::TransferEncoding].empty())
		throw HTTPException(HTTP::BAD_REQUEST);
	else if (!request->headers[HTTP::ContentLength].empty() || !request->headers[HTTP::TransferEncoding].empty())
	{
		request->flag[HTTP::HAS_BODY] = true;
		if (!request->headers[HTTP::TransferEncoding].empty() && contains_chunked)
			request->flag[HTTP::IS_CHUNKED] = true;
		else if (!isValidNum(request->getHeaderVal(HTTP::ContentLength)))
		{
			request->flag[HTTP::CLOSE_CONNECTION] = true;
			throw HTTPException(HTTP::BAD_REQUEST);
		}
	}

	if (request->method == HTTP::MethodPOST)
	{
		if (request->headers[HTTP::ContentLength].empty() && request->getEncoding() != HTTP::chunked)
			throw HTTPException(HTTP::LENGTH_REQUIRED);
		else if (request->headers[HTTP::ContentType].empty())
			throw HTTPException(HTTP::BAD_REQUEST);
		else if (request->isLargePayLoad())
			throw HTTPException(HTTP::PAYLOAD_TOO_LARGE);
	}
	if (server && request->flag[HTTP::CHECK_MEMTYPE])
	{
		if (!request->headers[HTTP::ContentType].empty()
			&& !server->isMimeTypeAccepted(request->headers[HTTP::ContentType]))
			throw HTTPException(HTTP::UNSUPPORTED_MEDIA_TYPE);
	}
	if (request->getHeaderVal(HTTP::Connection) == "close")
		request->flag[HTTP::CLOSE_CONNECTION] = true;
}

void	HTTPRequest::clear(void)
{
	int	i = 0;
	std::vector<std::string>::const_iterator it;

	if (!flag[HTTP::HAS_BODY])
	{
		_state = HTTP::REQUEST_CONTINUE;
		while (i < HTTP::NUM_HEADERS)
		{
			this->headers[i].clear();
			++i;
		}
		i = 0;
		while (i < HTTP::NUM_PARAMS)
		{
			if (!params[i].empty())
				params[i].clear();
			++i;
		}
		headers->clear();
		for (int i = 0; i < HTTP::REQUEST_FLAG_NUM; i++)
			flag[i] = false;
		if (_locationptr)
		{
			_locationptr->useCGI(false);
			_locationptr->skipCheckMethod(false);
			_locationptr = NULL;
		}
	}
	flag[HTTP::IS_COMPLETE_CHUNK] = false;
}

Location	*HTTPRequest::getLocationPointer(void)
{
	return (_locationptr);
}

Server	*HTTPRequest::getServer(void)
{
	return (_server);
}

/*
	************************* Checkers *********************************
*/
bool	HTTPRequest::isLargePayLoad(void)
{
	std::vector<std::string> content_len = headers[HTTP::ContentLength];
	bool	large_pay_load = false;

	if (_max_body_size > 0)
	{
		if (!content_len.empty() && (stringToUnsignedLong(content_len.at(0)) > _max_body_size))
			large_pay_load = true;
		else if (!params[HTTP::BODY_STRING].empty() && params[HTTP::BODY_STRING].length() > _max_body_size)
			large_pay_load = true;
	}
	return (large_pay_load);
}

bool	HTTPRequest::isLargePayLoad(const std::string str)
{
	bool	large_pay_load = false;

	if (_max_body_size > 0)
	{
		if (!str.empty() && str.length() > _max_body_size)
			large_pay_load = true;
	}
	return (large_pay_load);
}

static bool	reachedContentLength(const std::string body, const std::size_t content_length)
{
	return (content_length != STRING_NULL && !body.empty() && (body.length() == content_length));
}

bool	HTTPRequest::hasHeaderSeparator(void)
{
	bool	has_separator = false;

	if (!params[HTTP::HEADERS_STRING].empty())
	{
		_header_sep_pos = params[HTTP::HEADERS_STRING].find(HTTP_double_newline);
		if (_header_sep_pos != STRING_NULL)
		{
			has_separator = true;
			params[HTTP::BODY_STRING] = params[HTTP::HEADERS_STRING].substr(_header_sep_pos + HTTP_double_newline.length());
		}
	}
	return (has_separator);
}

HTTP::RequestBody	HTTPRequest::getBodyType(void)
{
	std::string			boundary = ExtractString(getHeaderVal(HTTP::ContentType), "boundary=", HTTP_newline);
	HTTP::RequestBody	type = HTTP::NUM_REQUEST_BODY;

	if (flag[HTTP::IS_CHUNKED])
		type = HTTP::CHUNKED_BODY;
	else if (!boundary.empty())
	{
		type = HTTP::WITH_BOUNDARY_BODY;
		params[HTTP::BODY_BOUNDARY] = boundary;
	}
	else
		type = HTTP::RAW_BODY;
	return (type);
}

/*
	************************* Body *********************************
*/

static bool	parseRawBody(const std::string& request_body, std::string& content, const std::string content_length)
{
	bool		success = false;
	std::size_t	pos = 0;
	
	pos = request_body.find(HTTP_newline);
	if (pos != STRING_NULL)
		content = request_body.substr(0, pos);
	else if (reachedContentLength(request_body, std::strtoul(content_length.c_str(), NULL, 10)))
		content = request_body;
	else
		content = request_body;
	if (!content.empty())
		success = true;
	return (success);
}

bool	HTTPRequest::parseBodyChunk(void)
{
	bool		success = false;
	std::size_t	digit_sep = 0;
	std::size_t	pos = 0;

	/* Process first chunk */
	if (_chunk_size == STRING_NULL)
	{
		digit_sep = params[HTTP::BODY_STRING].find(HTTP_newline);
		if (digit_sep != STRING_NULL)
		{
			_chunk_size = std::strtoul(params[HTTP::BODY_STRING].substr(0, digit_sep).c_str(), NULL, 16);
			params[HTTP::BODY_STRING].erase(0, digit_sep + HTTP_newline.length());
		}
	}
	if (_chunk_size != STRING_NULL)
	{
		if (!_chunk_size)
		{
			flag[HTTP::IS_LAST_CHUNK] = true;
			success = true;
		}
		pos = params[HTTP::BODY_STRING].find(HTTP_newline);
		if (pos != STRING_NULL && pos == _chunk_size)
		{
			flag[HTTP::IS_COMPLETE_CHUNK] = true;
			params[HTTP::CONTENT] = params[HTTP::BODY_STRING].substr(0, pos); 
			params[HTTP::BODY_STRING] = params[HTTP::BODY_STRING].substr(pos + HTTP_newline.length());
			params[HTTP::FULL_CHUNKED_BODY] += params[HTTP::CONTENT];
			std::cout << "\rParsing chunks: full body size -> " << params[HTTP::FULL_CHUNKED_BODY].length() << std::flush;
			_chunk_size = STRING_NULL;
			success = true;
		}
	}
	return (success);
}

static bool	parseBodyWithBoundary(const std::string& request_body, std::string& content, const std::string& boundary)
{
	std::size_t	bodybegin = 0;
	std::size_t	bodyend = 0;
	bool		success = false;
	std::string	buffer = boundary;

	buffer.erase(0, 9);
	bodybegin = request_body.find(HTTP_double_newline);
	bodyend = request_body.find(buffer,  buffer.length() + 1);
	if (bodyend != STRING_NULL && bodybegin != STRING_NULL && endsWith(request_body.substr(bodyend), HTTP_newline))
	{
		content = request_body.substr(bodybegin, bodyend - bodybegin);
		ReplaceSubString(content, HTTP_newline, "");
		if (!ReplaceSubString(content, "--", ""))
			throw HTTPException(HTTP::BAD_REQUEST);
		success = true;
	}
	return (success);
}

/*
	Try to parse request body
	@returns false if the body is incomplete
*/
bool	HTTPRequest::parseBody(void)
{
	bool		success = false;

	if (_bodytype == HTTP::NUM_REQUEST_BODY)
		_bodytype = getBodyType();
	if (!params[HTTP::BODY_STRING].empty())
	{
		switch (_bodytype)
		{
		case HTTP::WITH_BOUNDARY_BODY:
			success = parseBodyWithBoundary(params[HTTP::BODY_STRING], params[HTTP::CONTENT], params[HTTP::BODY_BOUNDARY]);
			break;
		case HTTP::RAW_BODY:
			success = parseRawBody(params[HTTP::BODY_STRING], params[HTTP::CONTENT], getHeaderVal(HTTP::ContentLength));
			break;
		case HTTP::CHUNKED_BODY:
			success = this->parseBodyChunk();
			break;
		default:
			break;
		}
		params[HTTP::FILE_NAME] = ExtractString(params[HTTP::BODY_STRING], "filename=", HTTP_newline);
		if (params[HTTP::FILE_NAME].empty())
			params[HTTP::FILE_NAME] = ExtractString(getHeaderVal(HTTP::ContentDisposition), "filename=", HTTP_newline);
		if (!params[HTTP::FILE_NAME].empty())
		{
			params[HTTP::FILE_NAME].erase(0,9);
			RemoveChar(params[HTTP::FILE_NAME], is_dquote);
		}
	}
	return (success);
}

void	HTTPRequest::append(const std::string new_str)
{
	if (_state == HTTP::REQUEST_HEADERS_END)
		params[HTTP::BODY_STRING] += new_str;
	else
		params[HTTP::HEADERS_STRING] += new_str;
}
