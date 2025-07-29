/*
 * By: pedromar <pedromar@student.42madrid.com>
 * Created on: 2024-12-26
 * Last modified: 2024-12-26
 */

#ifndef CONNECTIONSOCKET_HPP
# define CONNECTIONSOCKET_HPP

#include "Socket.hpp"
#include "macros.h"

class ActiveSocket : public Socket
{
public:
  ActiveSocket ();
  ActiveSocket (std::string port);
  ActiveSocket (int fd, struct sockaddr addr, std::string port);
  ~ActiveSocket ();

 ssize_t Recv(std::string &msg);
 ssize_t Send(const std::string &msg);
// ssize_t Send(int fd, std::vector<std::string > msgs)
// ssize_t SendFile(inf fd, std::string name_file)
};

#endif // CONNECTIONSOCKET_HPP
