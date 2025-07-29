#include "RegexState.hpp"

MultiCharState::MultiCharState(int lvl)
{
	negate = false;
	this->lvl = lvl;
}

MultiCharState::~MultiCharState()
{
	FreeVector(states);
}

void MultiCharState::AddChar(char c)
{
	chars += c;
}

void MultiCharState::AddState(RegexState* state)
{
	MultiCharState *mstate = static_cast<MultiCharState*>(state);
	if (mstate)
		chars += mstate->chars;
	states.push_back(state);
}

bool equalfind(std::string chars, char c)
{
	return chars.find(c) != std::string::npos;
}

bool notequalfind(std::string chars, char c)
{
	return chars.find(c) == std::string::npos;
}

void MultiCharState::PrintChild()
{
	std::cout << chars;
}

bool MultiCharState::Matches(const std::string& input, size_t& pos, std::vector<std::string>& matches)
{
	(void)matches;
	size_t cant = 0;
	bool (*f)(std::string, char) = negate ? &notequalfind : &equalfind;
	while (pos < input.size() && cant < max && f(chars, input[pos]))
	{
		pos++;
		cant++;
	}
	return cant >= min;
}

HTTP::HTTPResult MultiCharState::StaticMatch(const std::string& input, size_t& pos, std::map<RegexState*, CurrentState>& record)
{
	size_t start;
	size_t cstate;
	size_t cant = 0;
	GetProgress(record, pos, start, cstate, cant);

	bool (*f)(std::string, char) = negate ? &notequalfind : &equalfind;
	while (pos < input.size() && cant < max && f(chars, input[pos]))
	{
		pos++;
		cant++;
	}

	if (cant < max && !input[pos])
	{
		SetProgress(record, start, cstate, cant);
		return HTTP::REQUEST_CONTINUE;
	}
	return cant >= min ? HTTP::REQUEST_HEADERS_END : HTTP::REQUEST_ERROR;
}