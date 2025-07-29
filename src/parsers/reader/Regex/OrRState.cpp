#include "RegexState.hpp"

OrRState::OrRState(int lvl)
{
	this->lvl = lvl;
	states.push_back(new AndRState(lvl + 1));
}

OrRState::~OrRState()
{
	FreeVector(states);
}

void OrRState::AddChar(char c)
{
	states.back()->AddChar(c);
}

void OrRState::AddState(RegexState* state)
{
	if (dynamic_cast<AndRState*>(state))
		states.push_back(state);
	else
		states.back()->AddState(state);
}

void OrRState::AddAction(void(*f)(std::string))
{
	states.back()->AddAction(f);
}

void OrRState::AddAction(int index)
{
	states.back()->AddAction(index);
}

void OrRState::AddQuantity(int min, int max)
{
	states.back()->AddQuantity(min, max);
}

void OrRState::SetQuantity(int min, int max)
{
	states.back()->AddQuantity(min, max);
}

void OrRState::PrintChild()
{
	std::cout << "OrRState: ";
	for (std::vector<RegexState*>::const_iterator state = states.begin(); state != states.end(); ++state)
	{
		(*state)->PrintChild();
	}
	std::cout << std::endl << "Or End";
}

bool OrRState::Matches(const std::string& input, size_t& pos, std::vector<std::string>& matches)
{
	size_t start = pos;
	for (std::vector<RegexState*>::const_iterator state = states.begin(); state != states.end(); ++state)
	{
		bool result = (*state)->Matches(input, start, matches);
		if (result)
		{
			pos = start;
			return true;
		}
	}
	return false;
}

HTTP::HTTPResult OrRState::StaticMatch(const std::string& input, size_t& pos, std::map<RegexState*, CurrentState>& record)
{
	size_t start;
	size_t none;
	GetProgress(record, pos, start, cstate, none);

	while (cstate < states.size())
	{
		switch (states[cstate]->StaticMatch(input, start, record))
		{
			case HTTP::REQUEST_CONTINUE:
			{
				SetProgress(record, start, cstate, none);
				return HTTP::REQUEST_CONTINUE;
			}
			case HTTP::REQUEST_HEADERS_END:
			{
				pos = start;
				return HTTP::REQUEST_HEADERS_END;
			}
			case HTTP::REQUEST_ERROR: break;
		}
		cstate++;
	}
	return HTTP::REQUEST_ERROR;
}

RegexState* OrRState::Simplify(std::vector<RegexState*>& toBeFree)
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
