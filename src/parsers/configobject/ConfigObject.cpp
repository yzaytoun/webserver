# include "ConfigObject.hpp"

void ConfigObject::Throw_Att_Error(std::string attName)
{
	std::cout << "El atributo: <" + attName + "> no puede ser procesado por " + getType() << std::endl;
	this->Dispose();
	throw InvalidAttribute();
}

void ConfigObject::ThrowCustomError(const char* err)
{
	this->Dispose();
	throw std::runtime_error(err);
}

void ConfigObject::Throw_Obj_Error(std::string objName)
{
	std::cout << "El objeto: <" + objName + "> no puede ser procesado por " + getType() << std::endl;
	this->Dispose();
	throw InvalidObject();
}

void ConfigObject::Print_Str_Vector(std::string name, const std::vector<std::string>& vector)
{
	std::cout << name + ": ";
	int i = -1;
	while (++i < (int)vector.size())
		std::cout << vector[i] << " ";
	std::cout << std::endl;
}

void ConfigObject::Print_Map(std::string name, std::map<int, std::string>& map, std::string indentation)
{
	std::map<int, std::string>::iterator it;

	std::cout << name + ": \n";
    for (it = map.begin(); it != map.end(); ++it) {
        std::cout << indentation + "Código de error: " << it->first << ", Valor: " << it->second << std::endl;}
}

void ConfigObject::Print_StrMap(std::string name, std::map<std::string, std::string>& map, std::string indent)
{
	std::map<std::string, std::string>::iterator it;

	std::cout << name + ": \n";
    for (it = map.begin(); it != map.end(); ++it) {
        std::cout << indent + "Host: " << it->first << ", Port: " << it->second << std::endl;}
}

ConfigObject::ConfigObject() { }

ConfigObject::~ConfigObject() { }

void ConfigObject::Handle_MultiStrings(std::vector<std::string>& vector, std::vector<std::string>& info)
{
	if (info.size() < 2)
		Throw_Att_Error(info.at(0));
	int i = 0;
	while (++i < (int)info.size())
		vector.push_back(info.at(i));
}

void	ConfigObject::FillAcceptMethod(bool *vector, std::vector<std::string> info)
{
	if (info.size() < 2)
		Throw_Att_Error(info.at(0));
	for (size_t i = 1; i < info.size(); i++) {
		vector[HTTP::StrToMethodId(info.at(i))] = true;
	}
}

void	ConfigObject::FillCGIVariable(std::string *vector, std::vector<std::string> info)
{
	if (info.size() != 3)
		Throw_Att_Error(info.at(0));
	vector[HTTP::StrToCGIVariable(info.at(1))] = info.at(2);
}

int	NumFormat(int ch)
{
	int	val;

	switch (ch)
	{
		case 'K': val = ONE_KILOBYTE;	break;
		case 'M': val = ONE_MEGABYTE;	break;
		case 'G': val = ONE_GIGABYTE;	break;
		default: val = 1; break;
	}
	return (val);
}

void ConfigObject::Handle_Int(int& target, std::vector<std::string>& info)
{
	int value;

	if (info.size() != 2)
		Throw_Att_Error(info.at(0));
	value = stringToInt(info.at(1));
	if (info.at(1).length() == 2 && isalpha(info.at(1).at(1)))
		value *= NumFormat(info.at(1).at(1));
	target = value;
}

void ConfigObject::Handle_unSignedInt(std::size_t& target, std::vector<std::string>& info)
{
	std::size_t	value;

	if (info.size() != 2)
		Throw_Att_Error(info.at(0));
	value = stringToUnsignedLong(info.at(1));
	if (info.at(1).length() == 2 && isalpha(info.at(1).at(1)))
		value *= NumFormat(info.at(1).at(1));
	target = value;
}

void ConfigObject::Handle_Str(std::string& target, std::vector<std::string>& info)
{
	if (info.size() != 2)
		Throw_Att_Error(info.at(0));
	target = info.at(1);
}

void ConfigObject::Handle_Str(std::vector<std::string>& target, std::vector<std::string>& info)
{
	if (info.size() != 2)
		Throw_Att_Error(info.at(0));
	target.push_back(info.at(1));
}

void ConfigObject::Handle_Map(std::map<int, std::string>& target, std::vector<std::string>& info)
{
	int i = 0;

	if (info.size() <= 2)
		Throw_Att_Error(info.at(0));
	while (++i < (int)info.size() - 1)
	{
		int val = stringToInt(info.at(i));
		target[val] = info.at(info.size() - 1);
	}
}

void ConfigObject::Handle_StrMap(std::map<std::string, std::string>& target, std::vector<std::string>& info)
{
	size_t	i = 1;

	if (info.size() < 2)
		Throw_Att_Error(info.at(0));
	while (++i < info.size())
		target[info.at(1)] = info.at(i);
}

void ConfigObject::Dispose(void)
{
	unsigned int	i = 0;
	unsigned int	size = resources.size();

	while (i < size)
	{
		if (resources[i])
		{
			resources[i]->Dispose();
			delete resources[i];
		}
		++i;
	}
	resources.clear();
}

/*---------------    Default    ----------------*/
/*
	@returns default paths
*/
std::vector<std::string>		GetDefautlLocations(void)
{
	std::vector<std::string>	locations;

	locations.push_back(DOCROOT);
	locations.push_back(DEF_LOCATION_PATH);
	locations.push_back(DEF_ROOT);
	locations.push_back(DEF_ERROR_ROOT);
	locations.push_back(DEF_CGI_ROOT);
	locations.push_back(DEF_UPLOADS_ROOT);
	return (locations);
}

/*
	@returns default server names
*/
std::vector<std::string>	GetDefautlServerNames(void)
{
	std::vector<std::string> server_names;

	server_names.push_back("webserve.com");
	server_names.push_back("www.webserve.com");

	return (server_names);
}

