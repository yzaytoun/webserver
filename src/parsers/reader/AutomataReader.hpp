#ifndef AUTOMATAREADER_HPP
# define AUTOMATAREADER_HPP

# include "ConfigObject.hpp"
# include "Automata.hpp"
# include <vector>
# include <fstream>
# include <stdlib.h>
# include <string>
# include "utils.h"

class ConfigObject;

class AutomataReader : public Automata
{
	enum s_states
	{
		EMPTY,
		ERROR,
		CURLY_O,
		CURLY_C,
		SPACES_BTW,
		WORD,
		SEMICOLON,
		EOL,
	};

	private:
		void take_word();
		void error();
		void curly_openend();
		void add_resource();
		void curly_close();
		void move();
		void innit();
		void (AutomataReader::*fsa[20])( void );
		void (AutomataReader::*fta[20][20])( void );
		std::vector<ConfigObject*>	targetStack;
		std::vector<std::string>	wordsReaded;
		int lines;

	public:
		AutomataReader();
		void read_config_file(ConfigObject* master, const std::string& filename);
		virtual int evaluate();
};
#endif