#include "HTTPException.hpp"
#include <iostream>
HTTPException::HTTPException(HTTP::StatusCode code)
{
	std::stringstream	oss;
	std::string			code_message = HTTP::StatusCodeToString(code);

	oss << code << " " << code_message;
	_message = oss.str();
}

HTTPException::~HTTPException() throw() {}

