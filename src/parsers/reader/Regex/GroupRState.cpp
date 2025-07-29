#include "RegexState.hpp"

GroupRState::GroupRState(AData* data)
{
	this->data = data;
	dataIndex = 0;
	hasDataIndex = false;
	conditions = new OrRState(lvl + 1);
	OnValidate = NULL;
}

GroupRState::~GroupRState()
{
	delete conditions;
	conditions = NULL;
}

void GroupRState::AddChar(char c)
{
	conditions->AddChar(c);
}

void GroupRState::AddAction(void (*f)(std::string))
{
	OnValidate = f;
}

void GroupRState::AddAction(int index)
{
	hasDataIndex = true;
	dataIndex = index;
}

void GroupRState::AddQuantity(int min, int max)
{
	conditions->AddQuantity(min, max);
}

void GroupRState::AddState(RegexState* state)
{
	conditions->AddState(state);
}

void GroupRState::PrintChild()
{
	std::cout << "Group: ";
	conditions->PrintChild();
	std::cout << std::endl << "Group End ";
}

bool GroupRState::Matches(const std::string& input, size_t& pos, std::vector<std::string>& matches)
{
	size_t start = pos;
	size_t start2 = pos;
	bool result = conditions->Matches(input, start, matches);
	// std::cout << std::endl << "<<<<Grupo>>>>>" << std::endl;
	// PrintChild();
	// std::cout << "<<<<Grupo fin>>>>>" << std::endl;
	size_t cant = 0;
	while (result && cant < max)
	{
		start2 = start;
		cant++;
		if (cant < max)
			result = conditions->Matches(input, start, matches);
	}
	if (cant >= min)
	{
		std::string matching = input.substr(pos, start2 - pos);
		if (OnValidate && start != pos)
			OnValidate(matching);
		if (hasDataIndex && data != NULL)
			(data->*(data->Actions[dataIndex]))(matching);
		matches.push_back(matching);
		pos = start;
	}
	return cant >= min;
}

HTTP::HTTPResult GroupRState::StaticMatch(const std::string& input, size_t& pos, std::map<RegexState*, CurrentState>& record)
{
	size_t start = 0;
	size_t cant = 0;
	size_t none = 0;
	GetProgress(record, pos, start, none, cant);
	size_t start2 = start;

	HTTP::HTTPResult result = conditions->StaticMatch(input, start, record);
	while (result != HTTP::REQUEST_ERROR && cant < max)
	{
		if (result == HTTP::REQUEST_CONTINUE)
		{
			SetProgress(record, start, none, cant);
			return HTTP::REQUEST_CONTINUE;
		}
		start2 = start;
		cant++;
		if (cant < max)
			result = conditions->StaticMatch(input, start, record);
	}

	if (cant >= min)
	{
		std::string matching = input.substr(pos, start2 - pos);
		if (OnValidate && start != pos)
			OnValidate(matching);
		if (hasDataIndex && data != NULL)
			(data->*(data->Actions[dataIndex]))(matching);
		pos = start;
		return HTTP::REQUEST_HEADERS_END;
	}
	return HTTP::REQUEST_ERROR;
}

RegexState* GroupRState::Simplify(std::vector<RegexState*>& toBeFree)
{
	RegexState* simplified = conditions->Simplify(toBeFree);

	if (simplified != conditions)
        conditions = simplified;
	return this;
}