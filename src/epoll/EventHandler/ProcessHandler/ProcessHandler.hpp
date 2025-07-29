/*
 * By: pedromar <pedromar@student.42madrid.com>
 * Created on: 2024-12-24
 * Last modified: 2024-12-24
 */

 #ifndef ProcessHandler_HPP
 # define ProcessHandler_HPP
 
 #include "IEventHandler.hpp"
 #include "Process.hpp"
 #include "ClientHandler.hpp"
 
 class Server;
 class ClientHandler;
 
 class ProcessHandler : public IEventHandler {
 private:
	Process*	  				_process;
	ActiveSocket*				_socket;
	std::vector<std::string>	_envp;
	std::vector<std::string>	_args;
	std::size_t					_bytes_read;
	std::string					_content;
	bool						_success;
	bool						_write_headers;
	bool						_is_chunked;
	std::size_t					_content_len;
 
 public:
 
	 ProcessHandler();
	 ProcessHandler(ActiveSocket *socket, int64_t timeout = DEF_TIMEOUT);

	 Process*	GetProcess(void) const;
	 void		run(Location* location);
	 void		clear(bool is_full_reset = true);
	 void		checkChunkedInput(bool is_chunked, const std::size_t content_len);
	 void handleWrite();   // Handle output event
	 void handleRead();    // Handle input event
	 void handleError();   // Handle error event
	 void handleHangUp();  // Handle disconnection event
	 void handleGeneric(); // Handle other event
	 bool hasTimeOut(); // Check for timeout
 
	 ~ProcessHandler();
 };
 
 #endif // ProcessHandler_HPP
 