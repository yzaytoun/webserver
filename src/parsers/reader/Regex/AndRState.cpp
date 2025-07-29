#include "RegexState.hpp"

void AndRState::AddState(RegexState* state)
{
	states.push_back(state);
}

void AndRState::AddAction(void(*f)(std::string))
{
	states.back()->AddAction(f);
}

void AndRState::AddAction(int index)
{
	states.back()->AddAction(index);
}

void AndRState::AddQuantity(int min, int max)
{
	states.back()->SetQuantity(min, max);
}

void AndRState::SetQuantity(int min, int max)
{
	states.back()->SetQuantity(min, max);
}

AndRState::AndRState(int lvl)
{
	this->lvl = lvl;
	this->print = true;
}

AndRState::AndRState(AndRState& other)
{
	this->print = other.print;
	this->states = std::vector<RegexState*>(other.states);
}

AndRState::~AndRState()
{
	FreeVector(states);
}

void AndRState::AddChar(char c)
{
	if (!states.empty()
		&& dynamic_cast<OrRState*>(states.back()))
		states.back()->AddChar(c);
	else
		states.push_back(new CharRState(c, lvl + 1));
}

void AndRState::PrintChild()
{
	std::cout << std::endl << "AndRState: ";
	for (std::vector<RegexState*>::const_iterator state = states.begin(); state != states.end(); ++state)
	{
		(*state)->PrintChild();
	}
}

bool AndRState::Matches(const std::string& input, size_t& pos, std::vector<std::string>& matches)
{
	size_t start = pos;
	for (std::vector<RegexState*>::const_iterator state = states.begin(); state != states.end(); ++state)
	{
		bool result = (*state)->Matches(input, start, matches);

		if (!result)
			return false;
	}
	pos = start;
	return true;
}

HTTP::HTTPResult AndRState::StaticMatch(const std::string& input, size_t& pos, std::map<RegexState*, CurrentState>& record)
{
	size_t start;
	size_t none;
	GetProgress(record, pos, start, cstate, none);

	while (cstate < states.size())
	{
		HTTP::HTTPResult result = states[cstate]->StaticMatch(input, start, record);
		switch (result)
		{
			case HTTP::REQUEST_CONTINUE:
			{
				SetProgress(record, start, cstate, none);
				return HTTP::REQUEST_CONTINUE;
			}
			case HTTP::REQUEST_HEADERS_END: break;
			case HTTP::REQUEST_ERROR: return HTTP::REQUEST_ERROR;
		}

		cstate++;
		if (result == HTTP::REQUEST_HEADERS_END && cstate != states.size() && input[start] == '\0')
		{
			SetProgress(record, start, cstate, none);
			return HTTP::REQUEST_CONTINUE;
		}
	}
	pos = start;
	return HTTP::REQUEST_HEADERS_END;
}

//bool AndRState::StaticMatch(const std::string& input, size_t& pos, std::map<RegexState *, CurrentState&> &record)
//bool AndRState::StaticMatch(const std::string& input, size_t& pos, std::map<RegexState*, CurrentState>& record)

RegexState* AndRState::Simplify(std::vector<RegexState*>& toBeFree)
{
	if (states.size() == 1)
	{
		toBeFree.push_back(this);
		// Se evita que se liberen el resto de nodos al terminar la simplificación.
		RegexState *child = states[0];
		states.clear();
		return child->Simplify(toBeFree);
	}
	else
	{
		for (size_t i = 0; i < states.size(); i++)
			states[i] = states[i]->Simplify(toBeFree);
	}
	return this;
}