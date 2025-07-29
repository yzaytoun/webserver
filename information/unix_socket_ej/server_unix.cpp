#include <iostream>
#include <string.h>
#include <strings.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/un.h>
#include <unistd.h>


  /*
  *
  * Echo Server: Lee lo que manda el cliente y lo escribe en pantalla . El cliente imprime por pantalla 
  *
  */

int main(int argc, char *argv[]) {
  /*
  *
  * Creacion del socket servidor
  *
  */

  //  creates an endpoint for communication and returns a file descriptor that
  //  refers to that endpoint.
  struct sockaddr_un  addr;
  addr.sun_family =  AF_UNIX;
  strncpy(addr.sun_path, "/tmp/SoyUnSocket.socket", sizeof(addr.sun_path));
  int                 fds = socket(AF_UNIX, SOCK_STREAM, 0);
  if (fds < 0) {
    std::cout << "Error socket" << std::endl;
  } else {
    std::cout << "Socked fd " << fds << std::endl;
  }
  // assigns the address specified by addr to the socket referred to by the file
  // descriptor sockfd
  if (bind(fds, (struct sockaddr *)&addr, sizeof(addr)) == -1) { //<--------------- Al crear el socker se le da un punto de entrada.
    std::cout << "Error bind" << std::endl;
  } else {
    std::cout << "create dir sun_family " << addr.sun_family << " sun_path "
              << addr.sun_path << std::endl;
  }
  // marks  the socket referred to by sockfd as a passive socket.
  //	20 = max en queue of pending message
  if (listen(fds, 20)) { // <--------------- Aplicación servidor escucha
    std::cout << "Error listen" << std::endl;
  } else {
    std::cout << "OkOk  " << std::endl;
  }

  /*
  *
  * Main loop. Comunicación
  *
  */

  int cfd;
  char buf[100];
  ssize_t numRead;
  for (;;) {
    /* Accept a connection. The connection is returned on a new socket, 'cfd';
     the listening socket ('sfd') remains open and can be used to accept further connections. */
    cfd = accept(fds, NULL, NULL);// <--------------- accepta conexion
    std::cout << "se han connectado" << std::endl;
    if (cfd == -1)
      exit(1);
  /* Transfer data from connected socket to stdout until EOF */
    while ((numRead = read(cfd, buf, 100)) > 0)
      if (write(STDOUT_FILENO, buf, numRead) != numRead)
        break ;
    if (numRead == -1)
      exit(1);
    if (close(cfd) == -1)
      exit(1);
  }
  return 0;
}
