
# include "AutomataReader.hpp"

void AutomataReader::innit()
{
	const std::string strings[] = {"{", "}", ";", " \t", "\n"};
	const size_t size = sizeof(strings) / sizeof(std::string);

	for (int i = 0; i < 20; i++)
		fsa[i] = NULL;
	for (int i = 0; i < 20; i++)
		for (int j = 0; j < 20; j++)
			fta[i][j] = NULL;

	state = 0;
	ostate = 0;
	fsa[ERROR] = &AutomataReader::error;
	fsa[CURLY_O] = &AutomataReader::curly_openend;
	fsa[SEMICOLON] = &AutomataReader::add_resource;
	fsa[CURLY_C] = &AutomataReader::curly_close; 
	fta[WORD][SEMICOLON] = &AutomataReader::take_word;
	fta[WORD][SPACES_BTW] = &AutomataReader::take_word;
	fta[WORD][CURLY_O] = &AutomataReader::take_word;
	fta[WORD][CURLY_C] = &AutomataReader::take_word;
	fta[EMPTY][WORD] = &AutomataReader::move;

	set_alphabet(size, strings);
}

int getstate(int i, int j)
{
	int state[][7] = {
	//   {   }   ;  \s  \n   ^
		{1,  3,  0,  0,  7,  5}, // empty			0
		{1,  1,  1,  1,  1,  1}, // error			1
		{2,  3,  0,  0,  7,  5}, // curly open		2
		{2,  3,  0,  0,  7,  5}, // curly close		3
		{2,  3,  6,  4,  1,  5}, // spaces btw 		4
		{2,  3,  6,  4,  1,  5}, // word	 		5
		{2,  3,  0,  0,  7,  5}, // semicolon		6
		{1,  3,  0,  0,  7,  5}, // line			7
	};
	return (state[i][j]);
}

AutomataReader::AutomataReader() : Automata(getstate)
{
	////std::cout << "Automata constructor called" << std::endl;
	innit();
}

bool    read_from(std::ifstream& input_file, const std::string& filename)
{
	input_file.open(filename.c_str());
    if (!input_file.is_open())
    {
        std::cerr << "Error: unable to open input file " << std::endl;
        return false;
    }
    return true;
}

void AutomataReader::take_word()
{
	wordsReaded.push_back(trim(str.substr(o_idx, c_idx - o_idx)));
	// //std::cout << "Taking Word: " << str.substr(o_idx, c_idx - o_idx) << std::endl;
	o_idx = c_idx;
}

void AutomataReader::error()
{
	std::cout << str << std::endl;
	std::cout << "o_state: " <<  ostate << " state: " << state << std::endl;
	std::cout << "An error have been found at line: " << lines << " index: " << c_idx << std::endl;
	exit(1);
}

void AutomataReader::add_resource()
{
	targetStack.back()->AddAttribute(wordsReaded);
	wordsReaded.clear();
}

void AutomataReader::curly_openend()
{
	ConfigObject *a = targetStack.back()->AddObject(wordsReaded);
	if (a)
	{
		//std::cout << "Cambiando objetivo de recursos a: " << a->getType() << std::endl;
		targetStack.push_back(a);					// Se convierte el paso actual en el objetivo de los recursos
	}
	wordsReaded.clear();
}

void AutomataReader::curly_close()
{
	//std::cout << "Quitando objetivo de recursos de: " << targetStack.back()->getType() << std::endl;
	targetStack.pop_back();
}

void AutomataReader::move()
{
	o_idx = c_idx;
}

void AutomataReader::read_config_file(ConfigObject* master, const std::string& filename)
{
	std::ifstream input_file;
	targetStack.push_back(master);
    
    if (!read_from(input_file, filename))
        return ;

    std::string iline;
	while (std::getline(input_file, iline))
    {
		set_string(iline + "\n");
		lines++;
		if (!startsWith(trim(iline), "#"))
			evaluate();
    }

    input_file.close();
}

int AutomataReader::evaluate()
{
	this->o_idx = 0;
	this->c_idx = -1;

	while (++c_idx < (int)str.length())
	{
		this->state = this->get_state(this->state, alphabet_idx(this->alphabet, str[this->c_idx]));
		/// std::cout << "State: " << this->ostate << " to: " << this->state << " at: " << str[this->c_idx] << " char: " << (int)str[this->c_idx] << std::endl;
		if (fta[ostate][state] != NULL)
			(*this.*fta[ostate][state])();
        if (fsa[state] != NULL)
			(*this.*fsa[state])();
		this->ostate = this->state;
	}
	return (this->state);
}