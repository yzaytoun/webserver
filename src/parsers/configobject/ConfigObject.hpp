
#pragma once

# include "AutomataReader.hpp"
# include <stdexcept>
# include <iostream>
# include <iomanip>
# include <utility>
# include <vector>
# include <string>
# include <map>
# include <sstream>
# include "macros.h"
# include "utils.h"
# include "HTTPElements.hpp"

class ConfigObject
{
	public:
		ConfigObject();
		virtual	~ConfigObject();
		std::vector<ConfigObject*> resources;
		virtual void AddAttribute(std::vector<std::string>& info) = 0;
		virtual ConfigObject* AddObject(std::vector<std::string>& info) = 0;
		virtual void Print_Content() = 0;
		virtual std::string getType() = 0;
		void Print_Str_Vector(std::string name, const std::vector<std::string>& vector);
		void Print_Map(std::string name, std::map<int, std::string>& map, std::string indentation);
		void Print_StrMap(std::string name, std::map<std::string, std::string>& map, std::string indent);
		void Handle_Int(int& target, std::vector<std::string>& info);
		void Handle_unSignedInt(std::size_t& target, std::vector<std::string>& info);
		void Handle_Str(std::string& target, std::vector<std::string>& info);
		void Handle_Str(std::vector<std::string>& target, std::vector<std::string>& info);
		void Handle_MultiStrings(std::vector<std::string> &vector, std::vector<std::string> &info);
		void Handle_Map(std::map<int, std::string>& target, std::vector<std::string>& info);
		void Handle_StrMap(std::map<std::string, std::string>& target, std::vector<std::string>& info);
		void FillAcceptMethod(bool *vector, std::vector<std::string> info);
		void FillCGIVariable(std::string *vector, std::vector<std::string> info);
		void Throw_Att_Error(std::string err);
		void Throw_Obj_Error(std::string err);
		void ThrowCustomError(const char* err);
		void Dispose(void);
		virtual void Mount(ConfigObject& obj) = 0;
		virtual void CheckConfig(ConfigObject& obj) = 0;
		virtual void SetDefaultConfig(ConfigObject& obj) = 0;

	class InvalidAttribute: public std::exception
	{
		public:
			const char *what() const throw()
			{
				return("Invalid Attribute");
			}
	};

	class InvalidObject: public std::exception
	{
		public:
			const char *what() const throw()
			{
				return("Invalid Object");
			}
	};
};

int		stringToInt(const std::string& str);

std::vector<std::string>	GetDefautlLocations(void);
std::vector<std::string>	GetDefautlServerNames(void);


/* ConfigObject */