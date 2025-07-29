#ifndef REGEXPARSER_HPP
# define REGEXPARSER_HPP

# include "RegexState.hpp"
# include "Automata.hpp"
# include "HTTPElements.hpp"
# include <vector>
# include <stack>

class RegexParser : public Automata
{
	private:
		GroupRState* LastGroup;
		RegexState* root;
		std::vector<RegexState*> groups;
		std::map<int, std::map<RegexState*, CurrentState> > records;
		void innit();
		void character();
		void errorfound();
		void orfound();
		void setstart();
		void setend();
		void star();
		void plus();
		void question();
		void groupstart();
		void groupending();
		void minmaxlength();
		void backslash();
		void negate();
		void move();
		void action();
		void (RegexParser::* fsa[22])(void);
		void (RegexParser::* fta[22][22])(void);
		void Simplify();

	public:
		std::vector<void (*)(std::string)> Actions;
		AData *data;
		bool Match(std::string str);
		HTTP::HTTPResult StaticMatch(std::string str, int fd);
		std::vector<std::string> Search(std::string str);
		RegexParser(std::string regex, std::vector<void(*)(std::string)> actions);
		RegexParser(std::string regex);
		RegexParser(AData* data, std::string regex);
		int evaluate();
		virtual	~RegexParser();
};
#endif