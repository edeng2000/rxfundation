/*
 * Copyright (C) Alex Nekipelov (alex@nekipelov.net)
 * License: MIT
 */

#ifndef HTTPPARSER_RESPONSE_H
#define HTTPPARSER_RESPONSE_H

#include <string>
#include <vector>
#include <sstream>
#include <map>

#include "buffer.h"

using namespace std;

namespace httpparser
{
	static std::map<int, std::string> http_status_code = {
	{100, "Continue"},
	{101, "Switching Protocols"},
	{200, "OK"},
	{201, "Created"},
	{202, "Accepted"},
	{203, "Non-Authoritative Information"},
	{204, "No Content"},
	{205, "Reset Content"},
	{206,  "Partial Content"},
	{300, "Multiple Choices"},
	{301, "Moved Permanently"},
	{302, "Found"},
	{303, "See Other"},
	{304, "Not Modified"},
	{305, "Use Proxy"},
	{307, "Temporary Redirect"},
	{400, "Bad Request"},
	{401, "Unauthorized"},
	{402, "Payment Required"},
	{403, "Forbidden"},
	{404, "Not Found"},
	{405, "Method Not Allowed"},
	{406, "Not Acceptable"},
	{407, "Proxy Authentication Required"},
	{408, "Request Time-out"},
	{409, "Conflict"},
	{410, "Gone"},
	{411, "Length Required"},
	{412, "Precondition Failed"},
	{413, "Request Entity Too Large"},
	{414, "Request-URI Too Large"},
	{415, "Unsupported Media Type"},
	{416, "Requested range not satisfiable"},
	{417, "Expectation Failed"},
	{500, "Internal Server Error"},
	{501, "Not Implemented"},
	{502, "Bad Gateway"},
	{503, "Service Unavailable"},
	{504, "Gateway Time-out"},
	{505,  "HTTP Version not supported"},
	{808,  "UnKnown"}  //private
	};



struct Response {
    Response()       
    {
		versionMajor = (0);
		versionMinor = (0);
		keepAlive = (false);
		statusCode = (0);
		chunked_ = (false);
	}
    
    struct HeaderItem
    {
        std::string name;
        std::string value;
    };

    int versionMajor;
    int versionMinor;
    std::vector<HeaderItem> headers;
    std::vector<char> content;
    bool keepAlive;
	bool chunked_;
    unsigned int statusCode;
    std::string status;

    std::string inspect() const
    {
        std::stringstream stream;
        stream << "HTTP/" << versionMajor << "." << versionMinor
               << " " << statusCode << " " << status << "\n";

        for(std::vector<Response::HeaderItem>::const_iterator it = headers.begin();
            it != headers.end(); ++it)
        {
            stream << it->name << ": " << it->value << "\n";
        }

        std::string data(content.begin(), content.end());
        stream << data << "\n";
        return stream.str();
    }
	inline void add_date(Buffer& buf) {
		char date[50];
#ifndef WIN32
		struct tm cur;
#endif
		struct tm* cur_p;
		time_t t = time(NULL);
#ifdef WIN32
		cur_p = gmtime(&t);
#else
		gmtime_r(&t, &cur);
		cur_p = &cur;
#endif
		if (strftime(date, sizeof(date),
			"Date:%a, %d %b %Y %H:%M:%S GMT\r\n", cur_p) != 0) {
			buf.Append(date, strlen(date));
		}
	}

	inline void add_content_len(const int64_t size, Buffer& buf) {
		char len[42];
		snprintf(len, sizeof(len), "Content-Length:" RXFMT_U64 "\r\n",(long long unsigned int) size);
		buf.Append(len, strlen(len));
	}
	inline bool need_body(const int response_code) {
		return (response_code != 204 && response_code != 304 && (response_code < 100 || response_code >= 200));
	}
	inline void MakeHttpResponse(const int response_code, const int64_t body_size, const std::map<std::string, std::string>& header_field_value, Buffer& buf)
	{
		//HTTP/%d.%d code reson\r\n
		auto response_code_iter = http_status_code.find(response_code);
		if (response_code_iter == http_status_code.end()) {
			response_code_iter = http_status_code.find(808);
		}
		char status[16];
		snprintf(status, sizeof status, "HTTP/%d.%d %d ", versionMajor, versionMinor, response_code_iter->first);
		buf.Append(status);
		buf.Append(response_code_iter->second);
		buf.Append("\r\n");
		if (response_code == 400) { //Bad request
			buf.Append("\r\n");
			return;
		}
		if (versionMajor == 1) {
			if (versionMinor >= 1 && header_field_value.find("Date") == header_field_value.end()) {
				add_date(buf);
			}
			auto chunk = header_field_value.find("Transfer-Encoding");
			if (chunk != header_field_value.end() && chunk->second.compare("chunked") == 0) {
				chunked_ = true;
			}
			if ((versionMinor == 0 && !keepAlive)) {
				buf.Append("Connection:close\r\n");
				
			}
			else {
				if (!chunked_) {
					add_content_len(body_size, buf);
				}
			}
		}
		if (need_body(response_code) && header_field_value.find("Content-Type") == header_field_value.end()) {
			buf.Append("Content-Type:text/html; charset=ISO-8859-1\r\n");
		}
		for (auto& it : header_field_value) {
			buf.Append(it.first);
			buf.Append(":");
			buf.Append(it.second);
			buf.Append("\r\n");
		}
		buf.Append("\r\n");
	}
};

} // namespace httpparser

#endif // HTTPPARSER_RESPONSE_H

