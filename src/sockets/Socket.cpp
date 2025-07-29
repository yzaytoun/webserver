
/*
 * By: pedromar <pedromar@student.42madrid.com>
 * Created on: 2024-12-26
 * Last modified: 2024-12-26
 */

#include "Socket.hpp"
#include "Timer.hpp"

Socket::Socket() : _sockfd(-1), _status(SOCKET_EMPTY), _host(), _port(), _IsNonBlocking(false), _timer()
{
    memset(&_RecvTimeout, 0, sizeof(timeval));
    memset(&_SendTimeout, 0, sizeof(timeval));
    memset(&_addrinfo, 0, sizeof(_addrinfo));
    memset(&_sockinfo, 0, sizeof(_sockinfo));
}

#include <iostream>
Socket::~Socket()
{    
    Close();
}

void Socket::Close(void)
{
    if (_sockfd == -1)
        return;
    close(_sockfd);
    memset(&_sockinfo, 0, sizeof(_sockinfo));
    _sockfd = -1;
}

int Socket::GetSocketDescriptor()
{
    return _sockfd;
}

std::string Socket::GetPort() const
{
    return _port;
}

std::string Socket::getHost() const
{
    return _host;
}

void    Socket::SetCanonicalName(const std::string& name)
{
    _addrinfo.ai_canonname = (char *)name.c_str();
}

bool Socket::SetNonBlocking()
{
    int flags = fcntl(_sockfd, F_GETFL, 0);

    if (flags < -1)
    {
        return (false);
    }
    if (fcntl(_sockfd, F_SETFL, flags | O_NONBLOCK) == -1)
    {
        return (false);
    }
    _IsNonBlocking = true;
    return (true);
}

bool Socket::SetBlocking()
{
    int flags = fcntl(_sockfd, F_GETFL, 0);

    if (flags < -1)
    {
        return (false);
    }
    if (fcntl(_sockfd, F_SETFL, flags & ~O_NONBLOCK) == -1)
    {
        return (false);
    }
    _IsNonBlocking = false;
    return (true);
}

bool Socket::IsNonblocking()
{
    return _IsNonBlocking;
}

ScoketStatus Socket::GetSocketStatus()
{
    return _status;
}

ScoketStatus Socket::SetFileDescriptor(struct addrinfo *addr)
{
    _sockfd = socket(addr->ai_family, addr->ai_socktype, addr->ai_protocol);
    if (_sockfd == -1)
    {
        _status = SOCKET_ERROR;
    }
    int flags = fcntl(_sockfd, F_GETFL, 0);
    if (flags < -1)
    {
        _status = SOCKET_ERROR;
    }
    if (fcntl(_sockfd, F_SETFL, flags & FD_CLOEXEC) == -1)
    {
        _status = SOCKET_ERROR;
    }
	_status = SOCKET_OK;
    return _status;
}

struct sockaddr *Socket::GetAddress()
{
    if (_sockinfo.sa_family == AF_INET)
    {
        return reinterpret_cast<struct sockaddr *>(&(_sockinfo));
    }
    else if (_sockinfo.sa_family == AF_INET6)
    {
        return reinterpret_cast<struct sockaddr *>(&(_sockinfo));
    }
    return NULL;
}

ScoketStatus Socket::SetAddrInfo(int ip_type, int socktype, int flags)
{
    memset(&_addrinfo, 0, sizeof(_addrinfo));
    _addrinfo.ai_family = ip_type;
    _addrinfo.ai_flags = flags;
    _addrinfo.ai_socktype = socktype;
	_status = SOCKET_OK;
    return _status;
}

bool Socket::SetRecvTimeout(int64_t RecvTimeoutSec, int64_t RecvTimeoutUsec)
{
    memset(&_RecvTimeout, 0, sizeof(struct timeval));

    _RecvTimeout.tv_sec = RecvTimeoutSec;
    _RecvTimeout.tv_usec = RecvTimeoutUsec;

    if (setsockopt(_sockfd, SOL_SOCKET, SO_RCVTIMEO, &_RecvTimeout, sizeof(struct timeval)) == -1)
    {
        _status = SOCKET_ERROR;
    }
	_status = SOCKET_OK;
    return (_status);
}

int64_t Socket::GetRecvTimeout()
{
    return timvalToMilliSeconds(_RecvTimeout);
}

bool Socket::SetSendTimeout(int64_t SendTimeoutSec, int64_t SendTimeoutUsec)
{
    memset(&_SendTimeout, 0, sizeof(struct timeval));

    _SendTimeout.tv_sec = SendTimeoutSec;
    _SendTimeout.tv_usec = SendTimeoutUsec;

    if (setsockopt(_sockfd, SOL_SOCKET, SO_RCVTIMEO, &_SendTimeout, sizeof(struct timeval)) == -1)
    {
        _status = SOCKET_ERROR;
    }
	_status = SOCKET_OK;
    return (_status);
}

int64_t Socket::GetSendTimeout()
{
    return timvalToMilliSeconds(_SendTimeout);
}

bool Socket::SetOptionReuseAddr()
{
    // Allows other sockets to bind() to this port, unless there is an active
    // listening socket bound to the port already. This enables you to get around
    // those “Address already in use” error messages when you try to restart your
    // server after a crash.
    int opt_resuseaddr = 1;
    if (setsockopt(_sockfd, SOL_SOCKET, SO_REUSEADDR, &opt_resuseaddr, sizeof(int)) == -1)
    {
        _status = SOCKET_ERROR;
    }
	_status = SOCKET_OK;
    return _status;
}
