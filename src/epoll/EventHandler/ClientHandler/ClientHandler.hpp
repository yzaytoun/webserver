/*
 * By: pedromar <pedromar@student.42madrid.com>
 * Created on: 2024-12-24
 * Last modified: 2024-12-24
 */

#ifndef CLIENTHANDLER_HPP
# define CLIENTHANDLER_HPP

#include "IEventHandler.hpp"
#include "Server.hpp"
#include "ActiveSocket.hpp"
#include "HTTPRequest.hpp"
#include "RequestParser.hpp"
#include "HTTPResponse.hpp"
#include "HTTPElements.hpp"
#include "ProcessHandler.hpp"

class Server;
class HTTPRequest;

class ClientHandler : public IEventHandler {
private:
	Server      		*_server;
	ActiveSocket		*_socket;
	HTTPRequest			*_request;
	std::string			_error_message;

public:
	ClientHandler();
	ClientHandler(Server *server, ActiveSocket* clientsocket, uint64_t timeout = DEF_TIMEOUT);

	void handleWrite();   // Handle output event
	void handleRead();    // Handle input event
	void handleError();   // Handle error event
	void handleHangUp();  // Handle disconnection event
	void handleGeneric(); // Handle other event
	bool hasTimeOut(); // Check for timeout

	~ClientHandler();
};

#endif // CLIENTHANDLER_HPP
