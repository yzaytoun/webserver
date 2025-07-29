#ifndef HTTPCGI_H
# define HTTPCGI_H

# include <string>

static std::string CONTENT_LENGTH = "CONTENT_LENGTH=";
static std::string CONTENT_TYPE = "CONTENT_TYPE=";
static std::string GATEWAY_INTERFACE = "GATEWAY_INTERFACE=CGI/1.1";
static std::string PATH_INFO = "PATH_INFO=";
static std::string PATH_TRANSLATED = "PATH_TRANSLATED=";
static std::string QUERY_STRING = "QUERY_STRING=";
static std::string REMOTE_HOST = "REMOTE_HOST=";
static std::string REQUEST_METHOD = "REQUEST_METHOD=";
static std::string SCRIPT_NAME = "SCRIPT_NAME=";
static std::string SERVER_NAME = "SERVER_NAME=";
static std::string SERVER_PORT = "SERVER_PORT=";
static std::string SERVER_PROTOCOL = "SERVER_PROTOCOL=HTTP/1.1";

#endif // HTTPCGI_H
