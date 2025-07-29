
#pragma once
#include <string>
#include <sstream>
#include "HTTPElements.hpp"

class HTTPException : public std::exception
{
private:
	std::string		_message;

public:
	HTTPException(HTTP::StatusCode code);
	virtual ~HTTPException() throw();

	const char*	what() const throw()
	{
		return _message.c_str();
	}
};
