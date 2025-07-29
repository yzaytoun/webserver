/*
 * By: pedromar <pedromar@student.42madrid.com>
 * Created on: 2024-09-14
 * Last modified: 2024-09-14
 */

#include "Master.hpp"

Master*	GetServerMaster(const char * config_file)
{
	Master*	master = NULL;

	try
	{
		master = new Master(config_file);
	}
	catch(const std::exception& e)
	{
		std::cerr << e.what() << std::endl;
	}
	return (master);
}

int main(int argc, char **argv)
{
	Master 	*master;

	if (argc > 2)
	{
		std::cout << RED "Se debe introducir un solo argumento o sin argumentos" RESET << std::endl;
		return (EXIT_FAILURE);
	}
	if (argc == 1)
		master = GetServerMaster("");
	else
		master = GetServerMaster(argv[1]);
	if (master)
	{
		try
		{
			master->Mount(*master);
			master->Run();
		}
		catch(const std::exception& e)
		{
			std::cerr << e.what() << std::endl;
			master->Dispose();
		}
		delete master;
	}
	return (EXIT_SUCCESS);
}
