
#include "RequestParser.hpp"

RequestParser* RequestParser::instance = NULL;

RequestParser::RequestParser() : _actions_index(0), _pattern(""), _actions(), _regex(NULL)
{

	// Init Method
	AddToRegex("(", NULL);

	{
		#define XX(MethodIdentifier, MethodString) \
			AddToRegex("("#MethodString " )", SetterHTTPRequest::setMethodName##MethodIdentifier); AddToRegex("|", NULL);
			HTTP_METHOD(XX)
		#undef XX
		AddToRegex("([^ ]| )", SetterHTTPRequest::setNotImplementedMethod);
	}

	// End Method
	AddToRegex(")", NULL);

	// Init URI
	{
		// Schema
		AddToRegex("([\\w][" + unreserved_octects + "]*:)", SetterHTTPRequest::setSchemaUri, true);

		//Init authorithy (UserInfo + host + port)
		AddToRegex("(//", NULL);

		{
			// UserInfo
			AddToRegex("([" + unreserved_octects + sub_delims_octects + "]+@)", SetterHTTPRequest::setUserInfoUri, true); 

			// host
			AddToRegex("(" + reg_name_octects + ")", SetterHTTPRequest::setHostUri, true);

			// port
			AddToRegex("(:[\\d]+)", SetterHTTPRequest::setPortUri);
		}

		// End authorithy
		AddToRegex(")", SetterHTTPRequest::setAuthorityUri, true);

		// path
		AddToRegex("([^\\?# ]+)", SetterHTTPRequest::setPathUri);		// Es un poco ambiguo como se maneja el path...

		// query
		AddToRegex("(\\?" + queryrgx_octects + ")", SetterHTTPRequest::setQueryUri, true); 
		
		// fragment
		AddToRegex("(#" + queryrgx_octects + ")", SetterHTTPRequest::setFragmentUri, true);
	}

	// protocol
	std::stringstream protocol;
	AddToRegex("( HTTP/1.1" + end_line_octects + ")", SetterHTTPRequest::firstLineChecker);

	//Init header name
	AddToRegex("(((", NULL);

	{
		#define XX(HeadersIdentifier, HeadersString) \
			AddToRegex("("#HeadersString": )", SetterHTTPRequest::setHeaderName##HeadersIdentifier); AddToRegex("|", NULL);
			HTTP_HEADERS(XX)
		#undef XX
		//// UnknownHeader
		AddToRegex("([" + unreserved_octects + "]+: )",SetterHTTPRequest::setHeaderNameUnknown, true);
	}

	// End header Name
	AddToRegex("))", NULL);
	
	// header value
	AddToRegex("([^"+ end_line_octects +"]+)", SetterHTTPRequest::setHeaderValue);
	AddToRegex(end_line_octects + ")*", NULL);
	
	// End Reques 
	AddToRegex("(" + end_line_octects + ")", SetterHTTPRequest::setEndRequest);
	// End Reques 
	
	SetRegexParser();
}

RequestParser*	RequestParser::getInstance(void)
{
    if (!RequestParser::instance)
        RequestParser::instance = new RequestParser();
    return RequestParser::instance;
}

void	RequestParser::destroyInstance(void)
{
	if (RequestParser::instance)
	{
		delete RequestParser::instance;
		instance = NULL;
	}
}

RequestParser::~RequestParser()
{
	if (_regex)
	{
		delete _regex;
		_regex = NULL;
	}
}

void	RequestParser::processHeaders(std::string request_str, HTTPRequest& request)
{
	this->request = &request;

	request.setTimer();
	request.setState(_regex->StaticMatch(request_str, 0));
	if (request.getState() == HTTP::REQUEST_ERROR)
		throw HTTPException(HTTP::BAD_REQUEST);
}

void	RequestParser::AddToRegex(std::string pattern, void(*action)(std::string), bool is_optional)
{
	if (action == NULL)
	{
		_pattern += pattern;
		return ;
	}

	std::stringstream pattern_aux;
	pattern_aux << pattern << "~" << _actions_index++;
	if (is_optional)
		pattern_aux << "?";
	_pattern += pattern_aux.str();
	_actions.push_back(action);

}

void    RequestParser::SetRegexParser()
{
	_regex = new RegexParser(_pattern, _actions);
}