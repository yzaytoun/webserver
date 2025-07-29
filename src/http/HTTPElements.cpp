#include "HTTPElements.hpp"

std::string	HTTP::StatusCodeToString(const StatusCode &status)
{
	switch (status)
	{
		#define XX(StatusCodeIdentifier, StatusCodeString, StatusCodeMessage) \
			case StatusCodeIdentifier: return #StatusCodeMessage;
			HTTP_STATUS(XX)
		#undef XX
		default:
			return "Unknown";
	}
}

static const std::map<std::string, HTTP::StatusCode> _FillStatusCodeToStr()
{
	std::map<std::string, HTTP::StatusCode> _tmp;

	#define XX(StatusCodeIdentifier, StatusCodeString, StatusCodeMessage) \
		_tmp[#StatusCodeIdentifier] = HTTP::StatusCodeString;
		HTTP_STATUS(XX)
	#undef XX
	return _tmp;
};

HTTP::StatusCode	HTTP::StrToStatusCode(const std::string &status)
{
	static const std::map<std::string, HTTP::StatusCode> status_to_str = _FillStatusCodeToStr();
	std::map<std::string, HTTP::StatusCode>::const_iterator	it = status_to_str.find(status);
	return (it != status_to_str.end() ? it->second : HTTP::INTERNAL_SERVER_ERROR);
}

std::string	HTTP::GetCodeMessage(const StatusCode &status)
{
	std::stringstream	oss;

	switch (status)
	{
		#define XX(StatusCodeIdentifier, StatusCodeString, StatusCodeMessage) \
		case StatusCodeIdentifier: oss << #StatusCodeIdentifier << " " << #StatusCodeMessage; break;
			HTTP_STATUS(XX)
		#undef XX
		default:
			return "Unknown";
	}
	return (oss.str());
}

static const std::map<std::string, HTTP::Method> _FillMethodToStr()
{
		std::map<std::string, HTTP::Method> _tmp;

		#define XX(MethodIdentifier, MethodString) \
			_tmp[#MethodString] = HTTP::MethodIdentifier;
			HTTP_METHOD(XX)
		#undef XX
		return _tmp;
};

HTTP::Method	HTTP::StrToMethodId(const std::string &protocol)
{
	static const std::map<std::string, HTTP::Method> MethodToStr = _FillMethodToStr();
	std::map<std::string, HTTP::Method>::const_iterator	it = MethodToStr.find(protocol);
	return (it != MethodToStr.end() ? it->second : HTTP::UnknownMethod);
}

std::string		HTTP::MethodIdToString(const HTTP::Method &protocol)
{
	switch (protocol)
	{
		#define XX(MethodIdentifier, MethodString) \
			case HTTP::MethodIdentifier: return #MethodString;
			HTTP_METHOD(XX)
		#undef XX
		default:
			return "UnknownMethod";
	}
}

static const std::map<std::string, HTTP::Protocol> _FillProtocolToStr()
{
		std::map<std::string, HTTP::Protocol> _tmp;

		#define XX(ProtocolIdentifier, ProtocolString) \
			_tmp[#ProtocolString] = HTTP::ProtocolIdentifier;
			HTTP_PROTOCOLS(XX)
		#undef XX
		return _tmp;
};

HTTP::Protocol	HTTP::StrToProtocolId(const std::string &protocol)
{
	static const std::map<std::string, HTTP::Protocol> ProtocolToStr = _FillProtocolToStr();
	std::map<std::string, HTTP::Protocol>::const_iterator	it = ProtocolToStr.find(protocol);
	return (it != ProtocolToStr.end() ? it->second : HTTP::UnknownProtocol);
}

std::string		HTTP::ProtocolIdToString(const HTTP::Protocol &protocol)
{
	switch (protocol)
	{
		#define XX(ProtocolIdentifier, ProtocolString) \
			case HTTP::ProtocolIdentifier: return #ProtocolString;
			HTTP_PROTOCOLS(XX)
		#undef XX
		default:
			return "UnknownProtocol";
	}
}

static const std::map<std::string, HTTP::RequestHeader> _FillRequestHeaderToStr()
{
		std::map<std::string, HTTP::RequestHeader> _tmp;

		#define XX(RequestHeaderIdentifier, RequestHeaderString) \
			_tmp[#RequestHeaderString] = HTTP::RequestHeaderIdentifier;
			HTTP_HEADERS(XX)
		#undef XX
		return _tmp;
};

HTTP::RequestHeader	HTTP::StrToRequestHeaderId(const std::string &reqHeader)
{
	static const std::map<std::string, HTTP::RequestHeader> RequestHeaderToStr = _FillRequestHeaderToStr();
	std::map<std::string, HTTP::RequestHeader>::const_iterator	it = RequestHeaderToStr.find(reqHeader);
	return (it != RequestHeaderToStr.end() ? it->second : HTTP::UnknownHeader);
}

std::string		HTTP::RequestHeaderIdToString(const HTTP::RequestHeader &reqHeader)
{
	switch (reqHeader)
	{
		#define XX(HeadersIdentifier, HeadersString) \
			case HTTP::HeadersIdentifier: return #HeadersString;
			HTTP_HEADERS(XX)
		#undef XX
		default:
			return "UnknownHeader";
	}
}

static const std::map<std::string, HTTP::LanguageTag> _FillLanguageTagToStr()
{
		std::map<std::string, HTTP::LanguageTag> _tmp;

		#define XX(LanguageTagIdentifier, LanguageTagString, LanguageTagMessage) \
			_tmp[#LanguageTagString] = HTTP::LanguageTagIdentifier;
			HTTP_LANGUAGE_TAG(XX)
		#undef XX
		return _tmp;
};

HTTP::LanguageTag	HTTP::StrToLanguageTagId(const std::string &LanguageTag)
{
	static const std::map<std::string, HTTP::LanguageTag> LanguageTagToStr = _FillLanguageTagToStr();
	std::map<std::string, HTTP::LanguageTag>::const_iterator	it = LanguageTagToStr.find(LanguageTag);
	return (it != LanguageTagToStr.end() ? it->second : HTTP::UnknownLanguageTag);
}

std::string		HTTP::LanguageTagIdToString(const HTTP::LanguageTag &LanguageTag)
{
	switch (LanguageTag)
	{
		#define XX(LanguageTagsIdentifier, LanguageTagsString, LanguageTagMessage) \
			case HTTP::LanguageTagsIdentifier: return #LanguageTagsString;
			HTTP_LANGUAGE_TAG(XX)
		#undef XX
		default:
			return "UnknownLanguageTag";
	}
}

static const std::map<std::string, HTTP::Encoding> _FillEncodingToStr()
{
		std::map<std::string, HTTP::Encoding> _tmp;

		#define XX(EncodingIdentifier, EncodingString) \
			_tmp[#EncodingString] = HTTP::EncodingIdentifier;
			HTTP_ENCODING(XX)
		#undef XX
		return _tmp;
};

HTTP::Encoding	HTTP::StrToEncodingId(const std::string &Encoding)
{
	static const std::map<std::string, HTTP::Encoding> EncodingToStr = _FillEncodingToStr();
	std::map<std::string, HTTP::Encoding>::const_iterator	it = EncodingToStr.find(Encoding);
	return (it != EncodingToStr.end() ? it->second : HTTP::UnknownEncoding);
}

std::string		HTTP::EncodingIdToString(const HTTP::Encoding &Encoding)
{
	switch (Encoding)
	{
		#define XX(EncodingsIdentifier, EncodingsString) \
			case HTTP::EncodingsIdentifier: return #EncodingsString; 
			HTTP_ENCODING(XX)
		#undef XX
		default:
			return "UnknownEncoding";
	}
}

static const std::map<std::string, HTTP::MimeType> _FillMimeTypeToStr()
{
		std::map<std::string, HTTP::MimeType> _tmp;

		#define XX(MimeTypeIdentifier, MimeTypeExtension, MimeTypeDef, MimeTypeString) \
			_tmp[#MimeTypeString] = HTTP::MimeTypeIdentifier;
			HTTP_MIME_TYPE(XX)
		#undef XX
		return _tmp;
};

HTTP::MimeType	HTTP::StrToMimeTypeId(const std::string &MimeType)
{
	static const std::map<std::string, HTTP::MimeType> MimeTypeToStr = _FillMimeTypeToStr();
	std::map<std::string, HTTP::MimeType>::const_iterator	it = MimeTypeToStr.find(MimeType);
	return (it != MimeTypeToStr.end() ? it->second : HTTP::UnknownMimeType);
}

static const std::map<std::string, HTTP::MimeType> _FillMimeTypeToExtension()
{
		std::map<std::string, HTTP::MimeType> _tmp;

		#define XX(MimeTypeIdentifier, MimeTypeExtension, MimeTypeDef, MimeTypeString) \
			_tmp[#MimeTypeExtension] = HTTP::MimeTypeIdentifier;
			HTTP_MIME_TYPE(XX)
		#undef XX
		return _tmp;
};

HTTP::MimeType	HTTP::ExtensionToMimeTypeId(const std::string &MimeStr)
{
	static const std::map<std::string, HTTP::MimeType> MimeTypeToExtension = _FillMimeTypeToExtension();
	std::map<std::string, HTTP::MimeType>::const_iterator	it = MimeTypeToExtension.find(MimeStr);
	return (it != MimeTypeToExtension.end() ? it->second : HTTP::UnknownMimeType);
}

std::string		HTTP::MimeTypeIdToString(const HTTP::MimeType &MimeType)
{
	switch (MimeType)
	{
		#define XX(MimeTypeIdentifier, MimeTypeExtension, MimeTypeDef, MimeTypeString) \
			case HTTP::MimeTypeIdentifier: return #MimeTypeString; 
			HTTP_MIME_TYPE(XX)
		#undef XX
		default:
			return "UnknownMimeType";
	}
}

std::string		HTTP::MimeTypeIdToExtension(const HTTP::MimeType &MimeType)
{
	switch (MimeType)
	{
		#define XX(MimeTypeIdentifier, MimeTypeExtension, MimeTypeDef, MimeTypeString) \
			case HTTP::MimeTypeIdentifier: return #MimeTypeExtension; 
			HTTP_MIME_TYPE(XX)
		#undef XX
		default:
			return "UnknownMimeType";
	}
}

static const std::map<std::string, HTTP::CgiVarible> _FillCGIVariables()
{
	std::map<std::string, HTTP::CgiVarible> _tmp;

	#define XX(CgiVaribleIdentifier, CgiVaribleString) \
		_tmp[#CgiVaribleString] = HTTP::CgiVaribleIdentifier;
		HTTP_CGI_VARIABLE(XX)
	#undef XX
	return _tmp;
};

HTTP::CgiVarible	HTTP::StrToCGIVariable(const std::string &cgi_variable)
{
	static const std::map<std::string, HTTP::CgiVarible> CGITOEnum = _FillCGIVariables();
	std::map<std::string, HTTP::CgiVarible>::const_iterator	it = CGITOEnum.find(cgi_variable);
	return (it != CGITOEnum.end() ? it->second : HTTP::UnknownCgiVarible);
}


std::string		HTTP::CGIVariableToString(const HTTP::CgiVarible &cgi_id)
{
	switch (cgi_id)
	{
		#define XX(CgiVaribleIdentifier, CgiVaribleString) \
			case CgiVaribleIdentifier: return #CgiVaribleString;
			HTTP_CGI_VARIABLE(XX)
		#undef XX
		default:
			return "UnknownCGIVariable";
	}
}