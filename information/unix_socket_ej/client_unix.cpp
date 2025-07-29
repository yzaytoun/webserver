#include <iostream>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/un.h>
#include <unistd.h>


int main(int argc, char *argv[]) {
  /*
  *
  * Creacion del socket cliente
  *
  */
  struct sockaddr_un addr;
  addr.sun_family = AF_UNIX;
  strncpy(addr.sun_path, "/tmp/SoyUnSocket.socket", sizeof(addr.sun_path));
  int fds = socket(AF_UNIX, SOCK_STREAM, 0);
  if (fds < 0) {
    std::cout << "Error socket" << std::endl;
  } else {
    std::cout << "Socked fd " << fds << std::endl;
  }
  // assigns the address specified by addr to the socket referred to by the file
  // descriptor sockfd
  if (connect(fds, (struct sockaddr *)&addr, sizeof(addr)) == -1) { // <--------------- hace peticion al servidor.
    std::cout << "Error connect" << std::endl;
  } else {
    std::cout << "create dir sun_family " << addr.sun_family << " sun_path "
              << addr.sun_path << std::endl;
  }
  char buf[100];
  ssize_t numRead;
  while ((numRead = read(STDIN_FILENO, buf, 100)) > 0)
    if (write(fds, buf, numRead) != numRead)
      break ;
 if (numRead == -1)
    exit(1);
  return (0);
}
