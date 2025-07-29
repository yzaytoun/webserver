/*
 * By: pedromar <pedromar@student.42madrid.com>
 * Created on: 2024-12-24
 * Last modified: 2024-12-24
 */

#ifndef SOCKETHANDLER_HPP
# define SOCKETHANDLER_HPP

#include "IEventHandler.hpp"
#include "PassiveSocket.hpp"
#include "ClientHandler.hpp"

class Server;

class SocketHandler : public IEventHandler {
private:
    PassiveSocket*	_socket;
    Server*			_server;
public:

    SocketHandler();
	SocketHandler(PassiveSocket *socket, Server *server);

    PassiveSocket*	GetSocket(void) const;
	void handleWrite();   // Handle output event
    void handleRead();    // Handle input event
    void handleError();   // Handle error event
    void handleHangUp();  // Handle disconnection event
    void handleGeneric(); // Handle other event
    bool hasTimeOut(); // Check for timeout

    ~SocketHandler();
};

#endif // SOCKETHANDLER_HPP
