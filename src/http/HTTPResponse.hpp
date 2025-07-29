/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   HTTPResponse.hpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: yzaytoun <yzaytoun@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/27 12:08:19 by yzaytoun          #+#    #+#             */
/*   Updated: 2025/03/22 20:41:39 by yzaytoun         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#pragma once

#include "Time.hpp"
#include "Directory.hpp"
#include <stdint.h>
#include <map>
#include <string>
#include <vector>
#include <sstream>
#include "HTTPElements.hpp"
#include "Location.hpp"
#include "Server.hpp"
#include "utils.h"
#include <cstdlib>
#include "HTTPCGI.hpp"

class Location;
class Server;
class HTTPRequest;

class HTTPResponse
{
private:
	std::string							_message;
	std::string							_port;
	std::string							_filename;
	Time								_time;

	std::ostringstream					_headers;
	std::ostringstream					_body;

	std::vector<std::string>			_cookies;
	bool								autoindex;
	
	void			handleGet(HTTPRequest& request, const std::string& path);
	void			handlePost(HTTPRequest& request, const std::string& path);
	void			handleDelete(HTTPRequest& request, const std::string& path);
	void			handlePut(HTTPRequest& request, const std::string& path);

	std::string		generateAutoindex(const HTTPRequest *request, const std::string& path);

	bool		isRedirection(const Location & location);
	void		handleRedirection(const std::string status_redirection, const std::string new_path);
	void		closeHeaders(void);

public:
	HTTPResponse();
	HTTPResponse(const std::string port);
	~HTTPResponse();
	HTTPResponse(const HTTPResponse& response);
	HTTPResponse&	operator=(const HTTPResponse& response);

	/* Setters */
	void			setMessage(const std::string& message);
	void			addAllowedMethods(Location& location);
	void			addBodyFromFile(const HTTPRequest *request, const std::string &path);
	void			addBody(const std::string content, bool add_newline = true);
	void			addFullHeader(const std::string header);
	void			handleError(std::map<int, std::string> error_pages);
	
	template <typename T>
	void			addHeader(const std::string key, const T val){
		_headers << key << ": " << val << HTTP_newline;
	};
	
	/* Getters */	
	std::string		getMessage(void) const;
	std::string		getCurrentTime(void) const;
	std::string		getCurrentTime(const int64_t offset, Time::Period period);

	/* Methods */
	void			generateFromRequest(HTTPRequest& request, Server* server);
	void			handleMethod(HTTPRequest& request, const std::string path);

	void			transferResponse(ActiveSocket *socket, bool write_headers = true, std::size_t content_length = 0);
	void			sendChunked(ActiveSocket *socket, std::string& content, std::size_t content_size);

};
