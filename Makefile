########################################################################################
#      WEBSERVER
########################################################################################

NAME        :=  webserv

vpath %.hpp src : src/http  : src/parsers/reader/Regex : src/parsers/reader : src/parsers/automata\
			: src/parsers/configobject : src/parsers/data : src/sockets : src/server \
			: src/utils : src/epoll : src/epoll/EventHandler\
			: src/epoll/EventHandler/ClientHandler : src/epoll/EventHandler/SocketHandler : src/epoll/EventHandler/ProcessHandler
vpath %.cpp src : src/http : src/parsers/reader/Regex : src/parsers/reader : src/parsers/automata\
			: src/parsers/configobject : src/parsers/data : src/sockets : src/server \
			: src/utils : src/epoll : src/epoll/EventHandler\
			: src/epoll/EventHandler/ClientHandler : src/epoll/EventHandler/SocketHandler : src/epoll/EventHandler/ProcessHandler
vpath %.h	src/server : src/utils

INCLUDE_DIRS := http parsers/reader parsers/reader/Regex parsers/reader\
				parsers/automata parsers/configobject parsers/data\
				sockets server utils epoll epoll/EventHandler\
				epoll/EventHandler/ClientHandler epoll/EventHandler/SocketHandler epoll/EventHandler/ProcessHandler

# CPP FILES ----------------------

REGEX		= AndRState.cpp CharRState.cpp GroupRState.cpp MultiCharState.cpp OrRState.cpp RegexParser.cpp RegexState.cpp

PARSER		= ConfigObject.cpp Automata.cpp AutomataReader.cpp Server.cpp Master.cpp Location.cpp ListenData.cpp

HTTP		= HTTPRequest.cpp HTTPResponse.cpp HTTPException.cpp RequestParser.cpp HTTPElements.cpp

SOCKETS		= Socket.cpp PassiveSocket.cpp ActiveSocket.cpp

EPOLL       = EpollManager.cpp

EVENTS      = IEventHandler.cpp ClientHandler.cpp SocketHandler.cpp ProcessHandler.cpp

UTILS		= stringfun.cpp debug.cpp Process.cpp Timer.cpp Directory.cpp Time.cpp containersfun.cpp

SRCS 		=  main.cpp $(PARSER) $(SOCKETS) $(UTILS) $(REGEX) $(EPOLL) $(EVENTS) $(HTTP)

# --------------------------------

OBJDIR		= .obj
OBJS		:= $(SRCS:%.cpp=$(OBJDIR)/%.o)
DEBUG_FLAG	= #-fno-limit-debug-info

CURRENT_DIR	:= $(shell pwd)
CXX         := c++
CXXFLAGS    := -Wall -Wextra -std=c++98 -g $(DEBUG_FLAG) -D WEBSERV_HOME=\"$(CURRENT_DIR)\" 
SANITIZER	:= -fsanitize=address

CXXFLAGS    += $(addprefix -Isrc/, $(INCLUDE_DIRS))

RM			:= rm -fr
MAKEFLAGS   += --no-print-directory


all: $(NAME)

$(NAME): $(OBJS)
	@echo Creating $(NAME)
	@$(CXX) $(OBJS) -o $(NAME) #$(SANITIZER)
	@echo "\n----------- $(NAME) Ready ------------\n"

$(OBJDIR)/%.o:%.cpp
	@echo Compiling $<
	@mkdir -p $(@D)
	@$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	@echo "\nCleaning Object files"
	@$(RM) $(OBJS) $(OBJDIR)

fclean: clean
	@$(RM) $(NAME)
	@echo "Cleaning" $(NAME)
	@echo "------------ DONE ------------\n"

re: fclean all

.PHONY: re fclean all
