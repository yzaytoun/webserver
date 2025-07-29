/*
 * By: pedromar <pedromar@student.42madrid.com>
 * Created on: 2024-12-26
 * Last modified: 2024-12-26
 */

#ifndef PASSIVESOCKET_HPP
#define PASSIVESOCKET_HPP

#include "ActiveSocket.hpp"
#include "Socket.hpp"
#include "macros.h"

class PassiveSocket : public Socket
{
  public:
    PassiveSocket();
    PassiveSocket(std::string host, std::string port);
    ~PassiveSocket();

    int Listen(int backlog = 5);
    ActiveSocket *Accept(bool is_non_block = false);

  private:
    ScoketStatus Bind(struct addrinfo *addr);
};

#endif // PASSIVESOCKET_HPP