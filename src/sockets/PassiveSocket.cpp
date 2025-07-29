
/*
 * By: pedromar <pedromar@student.42madrid.com>
 * Created on: 2024-12-26
 * Last modified: 2024-12-26
 */

#include "PassiveSocket.hpp"

PassiveSocket::PassiveSocket()
{
}

PassiveSocket::PassiveSocket(std::string host, std::string port)
{
    struct addrinfo *servinfo;

    _port = port;
    _host = host;
    SetAddrInfo(AF_UNSPEC, SOCK_STREAM, AI_PASSIVE);
    int status = getaddrinfo(_host.c_str(), _port.c_str(), &_addrinfo, &servinfo);
    if (status != 0)
    {
        throw std::runtime_error(RED "Error en getaddrinfo: " RESET + std::string(gai_strerror(status)));
    }
    for (struct addrinfo *p = servinfo; p != NULL; p = p->ai_next)
    {
        if (SetFileDescriptor(p) == SOCKET_ERROR)
        {
            continue;
        }
        if (SetOptionReuseAddr() == SOCKET_ERROR)
        {
            continue;
        }
        if (Bind(p) == SOCKET_ERROR)
        {
            continue;
        }
        // constructor  Success
        size_t size_servinfo = sizeof(_sockinfo);
        getsockname(_sockfd, &_sockinfo, (socklen_t *)&size_servinfo);
        freeaddrinfo(servinfo);
        return;
    }
    // constructor Error
    freeaddrinfo(servinfo);
    throw std::runtime_error(RED "PassiveSocket Constructor Error" RESET);
}

PassiveSocket::~PassiveSocket() {}

int PassiveSocket::Listen(int backlog)
{
    int rc = 0;
	 if (_sockfd < 0 || _status != SOCKET_OK) {
      throw std::runtime_error(RED "Uninitialized socket" RESET);
    }
    else if ((rc = listen(_sockfd, backlog)) < 0) {
      throw std::runtime_error(RED "Listen error" RESET);
    }
    return (rc);
}

ActiveSocket *PassiveSocket::Accept(bool is_non_block)
{
    ActiveSocket *ClientSocket = NULL;

    struct sockaddr addr;
    socklen_t addr_len = sizeof(addr);
    int fd = accept(_sockfd, &addr, &addr_len);
    if (fd == -1)
    {
        throw std::runtime_error(RED "Accept Error" RESET);
    }
    ClientSocket = new ActiveSocket(fd, addr, _port);
    if (is_non_block) 
    {
        ClientSocket->SetNonBlocking();
    }
    return ClientSocket;
}

ScoketStatus PassiveSocket::Bind(struct addrinfo *addr)
{
    if (bind(_sockfd, addr->ai_addr, addr->ai_addrlen) == -1)
    {
        close(_sockfd);
		_status = SOCKET_ERROR;
        throw std::runtime_error(RED "Bind Error" RESET);
    }
	_status = SOCKET_OK;
    return _status;
}