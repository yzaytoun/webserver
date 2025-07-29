/*
 * By: pedromar <pedromar@student.42madrid.com>
 * Created on: 2024-12-24
 * Last modified: 2024-12-24
 */

#include "SocketHandler.hpp"

SocketHandler::SocketHandler() : _socket(NULL), _server(NULL)
{
	this->has_timeout = false;
	this->deleted = false;
	this->RemoveHandler = false;
	this->_timeout = DEF_TIMEOUT;
}

SocketHandler::SocketHandler(PassiveSocket *socket, Server *server)
	: _socket(socket), _server(server)
{
	this->RemoveHandler = false;
	this->has_timeout = false;
	this->deleted = false;
	this->_timeout = DEF_TIMEOUT;
	this->fileDescriptor = socket->GetSocketDescriptor();
}

PassiveSocket*  SocketHandler::GetSocket(void) const
{
	return (_socket);
}

SocketHandler::~SocketHandler()
{
	//delete _socket;
}

void SocketHandler::handleWrite() {
    std::cout << "handleSocket write" << std::endl;
}

void SocketHandler::handleRead()
{
	//std::cout << "handleSocket read" << std::endl;
	try
	{
		ActiveSocket	*client = _socket->Accept();
		client->SetNonBlocking();
		client->SetRecvTimeout(RECV_TIMEOUT_SEC, RECV_TIMEOUT_USEC);
		client->SetSendTimeout(SEND_TIMEOUT_SEC, SEND_TIMEOUT_USEC);
		ClientHandler	*handler = new ClientHandler(_server, client, _server->getTimeOut());
		if (handler->fileDescriptor > 0)
			EpollManager::getEpollManager()->addEventHandler(handler, EPOLLIN | EPOLLHUP | EPOLLERR);
		else
			delete handler;
	}
	catch(const std::exception& e)
	{
		std::cerr << e.what() << '\n';
	}
};

void SocketHandler::handleError() {
	std::cout << "->"<< ":" << this->fileDescriptor << std::endl;
	//std::cout << "handleSocket err" << std::endl;
	this->RemoveHandler = true;
};

void SocketHandler::handleHangUp() {
	std::cout << "->"<< ":" << this->fileDescriptor << std::endl;
	//std::cout << "handleSocket han" << std::endl;
	this->RemoveHandler = true;
};

void SocketHandler::handleGeneric() {
};

bool SocketHandler::hasTimeOut()
{
	return (false);
}
