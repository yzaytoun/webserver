/*
 * By: pedromar <pedromar@student.42madrid.com>
 * Created on: 2024-12-24
 * Last modified: 2024-12-24
 */

 #ifndef PROCESS_HPP
 # define PROCESS_HPP
 
 #include <utils.h>
 #include <stdexcept>
 #include <iostream>
 #include <unistd.h>
 #include <sys/wait.h>
 #include <fcntl.h>
 #include <cstring>
 #include <vector>
 #include <errno.h>
 #include <cstdio>
 
#define BUFFER_SIZE_PIPE 1024

 class Process
 {
 private:
     pid_t pid;

     int pipe_in[2]; // parent -> child
     int pipe_out[2]; // child -> parent
 
     int     *status_error;
 
     void    WaitChild();
     void    RedirectPipe();
 public:
     Process();
     ~Process();
 
     int     GetInputFd();
     int     GetOutFd();
 
     int     GetStatus();
 
     pid_t   ForkProcess();
     bool    ExecProcess(std::string program, std::vector<std::string> argv, std::vector<std::string> envp);
     bool    ExecProcess(const char* program, char* const args[], char *const envp[]);
 
     static  bool KillProcess (pid_t pid, int signum);
     bool   KillProcess(void);
 
     void    RunCommand();
    
     ssize_t ReadOutput(std::string &msg);
     ssize_t SendInput(const std::string &msg);
 };
 
 #endif // PROCESS_HPP