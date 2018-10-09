#ifndef HTTP_H
#define HTTP_H
#pragma once
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <winsock2.h>
#include <string>
#include <map>

#pragma comment(lib, "Ws2_32.lib")

namespace Requests
{
	typedef std::map<std::string, std::string> post_data;
	typedef std::map<std::string, std::string> req_headers;

	const req_headers DEFAULT_HEADERS = { {"Connection", "close"} , {"Accept-Charset", "utf-8" } };
	const req_headers DEFAULT_POST_HEADERS = { {"Content-Type", "application/x-www-form-urlencoded"}, {"Accept-Charset", "utf-8" } };

	struct request {
		std::string status_code;
		std::string date;
		std::string server;
		std::string last_modified;
		int content_length{};
		std::string content_type;
		std::string content_encoding;
		std::string text;
	};


	request* get(std::string url, const req_headers& h_data = DEFAULT_HEADERS, u_short port = 80);
	request* post(std::string url, const post_data& pdata, const req_headers& h_data = DEFAULT_POST_HEADERS, u_short port = 80);

}
#endif