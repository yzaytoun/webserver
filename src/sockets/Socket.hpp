/*
 * By: pedromar <pedromar@student.42madrid.com>
 * Created on: 2024-12-26
 * Last modified: 2024-12-26
 */

#ifndef SOCKET_HPP
#define SOCKET_HPP

#include "Timer.hpp"

#include <stdexcept>

#include <cstring>

#include <string>
#include <vector>

// system
#include <fcntl.h>
#include <unistd.h>

// socket
#include <netinet/in.h>
#include <sys/socket.h>

// Protocol
#include <netdb.h>

#define RECV_BUFFER_SIZE 1024

typedef enum
{
    SOCKET_OK,
    SOCKET_ERROR,
    SOCKET_EMPTY,
    SOCKET_SEND_SUCCESS,
    SOCKET_SEND_ERROR,
    SOCKET_RECV_SUCCESS,
    SOCKET_RECV_ERROR
} ScoketStatus;

class Socket
{
  public:
    Socket();
    virtual ~Socket();

    void Close();
  
    /*
    * Get  Address
    */
    struct sockaddr *GetAddress();

    /*
    * Get port
    */
    std::string GetPort() const;
  
    /*
    * Get host
    */
    std::string getHost(void) const;

    /*
     * Get Socket File descriptor
     */
    int GetSocketDescriptor();

    /*
     * Set Blonking
     */
    bool SetBlocking();

    /*
     * Set Non-Blonking
     */
    bool SetNonBlocking();

    /*
     * return true if socket is Non-Blocking
     */
    bool IsNonblocking();

    /*
     * Set recv Timeout
     */
    bool SetRecvTimeout(int64_t nRecvTimeoutSec, int64_t nRecvTimeoutUsec);

    /*
     * Get recv Timeout in millisecond
     */
    int64_t GetRecvTimeout();

    /*
     * Set send Timeout
     */
    bool SetSendTimeout(int64_t nSendTimeoutSec, int64_t nSendTimeoutUsec);

    /*
     * Get send Timeout in millisecond
    */
    int64_t GetSendTimeout();
    
    /*
     * Set socket canonical name
    */
    void  SetCanonicalName(const std::string& name);

    /*
     *  make socket reuse
     */
    bool SetOptionReuseAddr();

    ScoketStatus GetSocketStatus();

  protected:
 
    /* Socket File descriptor */
    int _sockfd;

    /*Socket Status*/
    ScoketStatus _status;
  
    /* Host name or IP address */
    std::string _host;

    /* Service or port */
    std::string _port;

    struct addrinfo _addrinfo;
    struct sockaddr _sockinfo;

    /* Socket Blocking status */
    bool _IsNonBlocking;

    /* receive timeout */
    struct timeval _RecvTimeout;

    /* send timeout */
    struct timeval _SendTimeout;

    /* Internal timer */
    Timer _timer;

    ScoketStatus SetFileDescriptor(struct addrinfo *addr);
    ScoketStatus SetAddrInfo(int ip_type, int socktype, int flags);
};

#endif // SOCKET_HPP