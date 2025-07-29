/*
 * By: pedromar <pedromar@student.42madrid.com>
 * Created on: 2024-12-26
 * Last modified: 2024-12-26
 */

#include "ActiveSocket.hpp"

ActiveSocket::ActiveSocket() {};
ActiveSocket::~ActiveSocket() {};

ActiveSocket::ActiveSocket(std::string port)
{
    struct addrinfo *servinfo;

    _host = "";
    _port = port;
    SetAddrInfo(AF_UNSPEC, SOCK_STREAM, 0);
    int status = getaddrinfo(_host.c_str(), _port.c_str(), &_addrinfo, &servinfo);
    if (status != 0)
    {
        throw std::runtime_error("Error en getaddrinfo: " + std::string(gai_strerror(status)));
    }
    for (struct addrinfo *p = servinfo; p != NULL; p = p->ai_next)
    {
        if (SetFileDescriptor(p) == SOCKET_ERROR)
        {
            continue;
        }
        if (connect(_sockfd, p->ai_addr, p->ai_addrlen) == -1)
        {
            close(_sockfd);
            throw std::runtime_error("Connect error");
        }
        // constructor  Success
        _status = SOCKET_OK;
        size_t size_servinfo = sizeof(_sockinfo);
        getsockname(_sockfd, &_sockinfo, (socklen_t *)&size_servinfo);
        freeaddrinfo(servinfo);
        return;
    }
    // constructor Error
    freeaddrinfo(servinfo);
    throw std::runtime_error("ActiveSocket Constructor Error");
}

ActiveSocket::ActiveSocket(int fd, struct sockaddr addr, std::string port)
{
    _sockfd = fd;
    _port = port;
    memcpy(&_addrinfo, &addr, sizeof(_addrinfo));
}

ssize_t ActiveSocket::Recv(std::string &msg)
{
    char buffer[RECV_BUFFER_SIZE];
    std::vector<char> message;
    ssize_t bytes_received = 0;

    while (true)
    {
        bytes_received = recv(_sockfd, buffer, RECV_BUFFER_SIZE - 1, 0);
        if (bytes_received == -1)
           break;
		buffer[bytes_received] = '\0';
        msg.append(buffer, bytes_received);
		if (bytes_received == 0 || (bytes_received < RECV_BUFFER_SIZE - 1))
			break;
    }
    return (bytes_received);
}

ssize_t ActiveSocket::Send(const std::string &msg)
{
    ssize_t bytes_sent = send(_sockfd, msg.c_str(), msg.length(), 0);
    if (bytes_sent == -1)
    {
        close(_sockfd);
        return SOCKET_SEND_ERROR;
    }
    return SOCKET_SEND_SUCCESS;
}
