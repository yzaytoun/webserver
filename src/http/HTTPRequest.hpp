#ifndef HTTPREQUEST_HPP
# define HTTPREQUEST_HPP

#include <iostream>
#include <map>
#include <sstream>
#include "RegexState.hpp"
#include "macros.h"
#include "utils.h"
#include "HTTPElements.hpp"
#include "RequestParser.hpp"
#include "HTTPException.hpp"
#include "Location.hpp"

class Server;
class Location;

class HTTPRequest
{
	private:
		std::vector<std::string>            enviorment_variables;
		std::vector<std::string>            arguments;
		HTTP::HTTPResult                    _state;
		Timer                               _timer;
		std::size_t							_max_body_size;
		Location							*_locationptr;
		Server								*_server;
		HTTP::RequestBody					_bodytype;
		std::size_t							_header_sep_pos;
		std::size_t							_chunk_size;
		
		HTTP::RequestBody					getBodyType(void);
		
		public:
		// Params
		std::string							params[HTTP::NUM_PARAMS];

		// Method
		HTTP::Method                        method;
		
        // URI
        HTTP::Schema                        scheme;
        HTTP::HostType                      host_type;
		bool								flag[HTTP::REQUEST_FLAG_NUM];
		
		// Protocol
		HTTP::Protocol                      protocol;
		
		// Headers
		std::vector<std::string>           	headers[HTTP::NUM_HEADERS];

		HTTP::RequestHeader                 header_index;
		HTTPRequest();
		~HTTPRequest();

		/* Methods */
		void    					setState(HTTP::HTTPResult state);
		void						setMaxBody(std::size_t max_body_size);
		void						setTimer(void);
		void						addLocationPointer(Location* location);
		void						addServer(Server* server);
		bool						parseBody(void);

		std::string					getHeaderVal(HTTP::RequestHeader header_id, const int idx = 0);
		Location*					getLocationPointer(void);
		Server*						getServer(void);
		std::string					getTime(void);
		HTTP::HTTPResult			getState(void) const;
		std::string                 getLocation(void) const;
		std::vector<std::string>    getArguments(void) const;
		std::vector<std::string>    getEnvironmentVariables(void) const;
		void						clear(void);
		void						append(const std::string new_str);

		bool						isLargePayLoad(void);
		bool						isLargePayLoad(const std::string str);
		HTTP::Encoding				getEncoding(void);
		bool						hasHeaderSeparator(void);
		bool						parseBodyChunk(void);
};

std::ostream& operator<<(std::ostream& os, const HTTPRequest& request);

namespace SetterHTTPRequest
{
	// Default Setter
	void setDefaultSetter(std::string str);

	// Method
#define XX(MethodIdentifier, MethodString) \
	void setMethodName##MethodIdentifier(std::string str);
	HTTP_METHOD(XX)
#undef XX

	void setNotImplementedMethod(std::string str);

	// URI
	//// Schema
	void setSchemaUri(std::string str);
	//// UserInfo
	void setUserInfoUri(std::string str);
	//// host
	void setHostUri(std::string str);
	//// port
	void setPortUri(std::string str);
	//// Authority
	void setAuthorityUri(std::string str);
	//// Path
	void setPathUri(std::string str);
	//// Query
	void setQueryUri(std::string str);
	//// Fragment
	void setFragmentUri(std::string str);

	// Protocol
	void firstLineChecker(std::string str);

	// Header Name
#define XX(HeadersIdentifier, HeadersString) \
	void setHeaderName##HeadersIdentifier(std::string str);
	HTTP_HEADERS(XX)
#undef XX
	void setHeaderNameUnknown(std::string str);

	// Header Value
	void setHeaderValue(std::string str);

	// End Request 
	void setEndRequest(std::string str);
}

#endif
