	#ifndef REGEXSTATE_HPP
	#define REGEXSTATE_HPP

	# include <stdexcept>
	# include <iostream>
	# include <iomanip>
	# include <utility>
	# include <vector>
	# include <string>
	# include <map>
	# include <stack>
	# include <memory>
	# include <stdint.h>
	# include <cstdlib>
	# include "HTTPElements.hpp"

	/// @brief 
	class AData
	{
		public:
			AData() {};
			std::vector<void (AData::*)(std::string)> Actions;
	};

	struct CurrentState
	{
		int fd;
		size_t cstate;
		size_t start;
		size_t cant;
	};

	class
	RegexState
	{
		public:
			int lvl;
			size_t min;
			size_t max;
			virtual ~RegexState();
			virtual RegexState* Simplify(std::vector<RegexState*> &) { return this; };
			virtual void PrintChild() { std::cout << "Nada" << std::endl; };
			virtual void AddChar(char c) { (void)c; };
			virtual void AddAction(void (*f)(std::string)) { (void)f; };
			virtual void AddAction(int index) { (void)index; };
			virtual void AddState(RegexState *state) { (void)state; };
			virtual void AddQuantity(int min, int max);
			virtual void SetQuantity(int min, int max);
			virtual bool Matches(const std::string& input, size_t& pos, std::vector<std::string>& matches);
			virtual HTTP::HTTPResult StaticMatch(const std::string& input, size_t& pos, std::map<RegexState*, CurrentState>& record) { (void)input; (void)pos;  (void)record; return HTTP::REQUEST_ERROR; };
			virtual void GetProgress(std::map<RegexState*, CurrentState>& record, int pos, size_t& start, size_t& cstate, size_t& cant);
			virtual void SetProgress(std::map<RegexState*, CurrentState>& record, size_t& start, size_t& cstate, size_t& cant);
			void FreeVector(std::vector<RegexState*> &);

		protected:
			RegexState();
	};

	class AndRState : public RegexState
	{
		public:
			bool print;
			size_t cstate;
			std::vector<RegexState*> states;
			AndRState(int lvl);
			AndRState(AndRState& other);
			~AndRState();
			RegexState *Simplify(std::vector<RegexState*>& toBeFree);
			void AddChar(char c);
			void AddState(RegexState* state);
			void AddAction(void (*f)(std::string));
			void AddAction(int index);
			void AddQuantity(int min, int max);
			void SetQuantity(int min, int max);
			bool Matches(const std::string& input, size_t& pos, std::vector<std::string>& matches);
			HTTP::HTTPResult StaticMatch(const std::string& input, size_t& pos, std::map<RegexState*, CurrentState>& record);
			void PrintChild();
	};

	class CharRState : public RegexState
	{
		public:
			CharRState(char c, int lvl);
			~CharRState();
			char character;
			bool Matches(const std::string& input, size_t& pos, std::vector<std::string>& matches);
			HTTP::HTTPResult StaticMatch(const std::string& input, size_t& pos, std::map<RegexState*, CurrentState>& record);
			void PrintChild();
	};

	// Estado inicial del ResX, dentro siempre se añade un AndRState, si termina teniendo solamente un
	//	estado se comporta como un AndRState
	class OrRState : public RegexState
	{
		public:
			OrRState(int lvl);
			~OrRState();
			bool print;
			size_t cstate;
			std::vector<RegexState*> states;
			void AddChar(char c);
			void AddState(RegexState* state);
			void AddAction(void (*f)(std::string));
			void AddAction(int index);
			void AddQuantity(int min, int max);
			void SetQuantity(int min, int max);
			bool Matches(const std::string& input, size_t& pos, std::vector<std::string>& matches);
			HTTP::HTTPResult StaticMatch(const std::string& input, size_t& pos, std::map<RegexState*, CurrentState>& record);
			RegexState* Simplify(std::vector<RegexState*>& toBeFree);
			void PrintChild();
		};
		
		class MultiCharState : public RegexState
		{
			public:
			bool		negate;
			std::string chars;
			std::vector<RegexState*> states;
			MultiCharState(int lvl);
			~MultiCharState();
			void AddChar(char c);
			void AddState(RegexState* state);
			bool Matches(const std::string& input, size_t& pos, std::vector<std::string>& matches);
			HTTP::HTTPResult StaticMatch(const std::string& input, size_t& pos, std::map<RegexState*, CurrentState>& record);
			void PrintChild();
	};

	class GroupRState : public RegexState
	{
		private:
			bool hasDataIndex;
			int dataIndex;
			AData *data;
			void (*OnValidate)(std::string);
		public:
			RegexState *conditions;
			GroupRState(AData* data = NULL);
			~GroupRState();
			void AddChar(char c);
			void AddAction(void (*f)(std::string));
			void AddAction(int index);
			void AddQuantity(int min, int max);
			void AddState(RegexState* state);
			bool Matches(const std::string& input, size_t& pos, std::vector<std::string>& matches);
			HTTP::HTTPResult StaticMatch(const std::string& input, size_t& pos, std::map<RegexState*, CurrentState>& record);
			RegexState* Simplify(std::vector<RegexState*>& toBeFree);
			void PrintChild();
	};
	#endif