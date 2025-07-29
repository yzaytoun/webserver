#ifndef REQUESTPARSER_HPP
# define REQUESTPARSER_HPP

#include <iostream>
#include <memory>
#include "RegexParser.hpp"
#include "HTTPRequest.hpp"
#include "utils.h"
#include <sstream>

class HTTPRequest;

/* RFC 3986 regex */
static const std::string pct_encoded_octects = "%[abcdefABCDEF]{2,2}";
static const std::string unreserved_octects = "\\w\\d-._~";
static const std::string sub_delims_octects = "!$&',;=\\(\\)\\*\\+";
static const std::string IPv4address_octects = "([\\d]{1,3}.){3,3}[\\d]{1,3}";
static const std::string reg_name_octects = "(" + pct_encoded_octects + "|[" + unreserved_octects + sub_delims_octects + "])+";
static const std::string pchar_octects = "(" + pct_encoded_octects + "|[" + unreserved_octects + sub_delims_octects + ":@])+";
static const std::string queryrgx_octects = "(" + pct_encoded_octects + "|[" + unreserved_octects + sub_delims_octects + ":@/\\?])+";
static const std::string end_line_octects = "\r\n";
/* RFC 3986 regex end */

class RequestParser {

    private:
        std::size_t                         _actions_index;
        std::string                         _pattern;
        std::vector<void(*)(std::string)>   _actions;
        RegexParser                         *_regex;
        static RequestParser                *instance;
         
        RequestParser();
        ~RequestParser();
        void    SetRegexParser();
        void    AddToRegex(std::string pattern, void(*action)(std::string), bool is_optional = false);

    public:
        HTTPRequest             *request;
        void                    processHeaders(std::string request_str, HTTPRequest& request);

        static RequestParser*   getInstance(void);
		static void	            destroyInstance(void);
};

#endif