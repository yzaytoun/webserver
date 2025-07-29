

#include "Process.hpp"

Process::Process() : pid(-1)
{
	if (pipe(pipe_in) == -1)
	{
		pipe_in[0] = -1;
		pipe_in[1] = -1;
	}
	if (pipe(pipe_out) == -1)
	{
		close(pipe_in[0]);
		close(pipe_in[1]);
		pipe_out[0] = -1;
		pipe_out[1] = -1;
	}
}

Process::~Process()
{
	close(pipe_in[1]);
	close(pipe_out[0]);
	if (pid != -1)
		KillProcess(pid, 9);
}

pid_t Process::ForkProcess()
{
	pid = fork();
	if (pid < 0)
	{
		throw std::runtime_error("Fork error");
	}
	return pid;
}

bool Process::ExecProcess(std::string program, std::vector<std::string> argv, std::vector<std::string> envp)
{
	char *_program = const_cast<char*>(program.c_str());
	char **_argv = VecStringToArrChars(argv);
	char **_envp = VecStringToArrChars(envp);
	return ExecProcess(_program, _argv, _envp);
}

bool Process::ExecProcess(const char *program, char *const args[], char *const envp[])
{
	ForkProcess();
	if (pid == 0)
	{
		RedirectPipe();
		//WaitChild();
		if (execve(program, args, envp) == -1)
		{
			freeCharArray(args);
			freeCharArray(envp);
			throw std::runtime_error(strerror(errno));
		}
	}
	else
	{
		RedirectPipe();
		freeCharArray(args);
		freeCharArray(envp);
		
	} 
	return true;
}
 
void	Process::RedirectPipe()
{
	if (pid == 0) 
	{
		dup2(pipe_in[0], STDIN_FILENO);
		close(pipe_in[0]);

		dup2(pipe_out[1], STDOUT_FILENO);
		close(pipe_out[1]);

		close(pipe_in[1]);
		close(pipe_out[0]);
	}
	else
	{
		// Not used
        close(pipe_in[0]);
		close(pipe_out[1]);
		
		// Set as fd's non blocking
		int flags_in = fcntl(pipe_in[1], F_GETFL);
		int flags_out = fcntl(pipe_out[0], F_GETFL);
		if (flags_in == -1 || flags_out == -1)
		{
			close(pipe_in[1]);
			close(pipe_out[0]);
			kill(pid, 9);
			throw std::runtime_error("Error");
		}
		flags_in |= O_NONBLOCK;
		flags_out |= O_NONBLOCK;
		if (fcntl(pipe_in[1], F_SETFL, flags_in) == -1 || fcntl(pipe_out[0], F_SETFL, flags_out) == -1)
		{
			close(pipe_in[1]);
			close(pipe_out[0]);
			kill(pid, 9);
			throw std::runtime_error("Error");
		}
	}
}

bool Process::KillProcess(pid_t pid, int signum)
{
	return (kill (pid, signum) < 0);
}

bool Process::KillProcess(void)
{
	return (kill (pid, 9) < 0);
}

int     Process::GetInputFd()
{
	return pipe_in[1];
}

int     Process::GetOutFd()
{
	return pipe_out[0];
}

void Process::WaitChild()
{
	char	buf;
	if (pid == 0)
	{
		read(STDIN_FILENO, &buf, 1);
	}
}

void Process::RunCommand()
{
	if (pid > 0)
	{
		write(pipe_in[0], " ", 1);
	}
}

int Process::GetStatus()
{
	if (pid > 0)
	{
		waitpid(pid, status_error, WNOHANG);
		if (WIFEXITED(*status_error))
			*status_error = WEXITSTATUS(*status_error);
	}
	return (*status_error);
}

ssize_t Process::ReadOutput(std::string &msg)
{
    char buffer[BUFFER_SIZE_PIPE];
    ssize_t bytes_received = 0;

    while (true)
    {
		bytes_received = read(pipe_out[0], buffer, BUFFER_SIZE_PIPE - 1);
        if (bytes_received == -1)
           break;
		buffer[bytes_received] = '\0';
		msg.append(buffer, bytes_received);
		if (bytes_received == 0 || (bytes_received < BUFFER_SIZE_PIPE - 1))
			break;
    }
    return (msg.length());
}

ssize_t Process::SendInput(const std::string &msg)
{
	std::size_t	len = 0;

	if (msg.empty())
		len = write(pipe_in[1], "1", 1);
	else
		len = write(pipe_in[1], msg.c_str(), msg.length());
	return (len);
}
