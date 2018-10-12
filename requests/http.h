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
	typedef std::map<std::string, std::string> res_headers;

	const req_headers DEFAULT_HEADERS = {{"Connection", "close"}, {"Accept-Charset", "utf-8"}};
	const req_headers DEFAULT_POST_HEADERS = {
		{"Content-Type", "application/x-www-form-urlencoded"}, {"Accept-Charset", "utf-8"}
	};

	struct request
	{
		std::string status_code;
		int content_length{};
		std::string text;
		res_headers headers;
	};


	request* execute(const req_headers& h_data, u_short port, const std::string& headers,
	                 const std::string& request_host);

	request* get(std::string url, const req_headers& h_data = DEFAULT_HEADERS, u_short port = 80);
	request* post(std::string url, const post_data& pdata, const req_headers& h_data = DEFAULT_POST_HEADERS,
	              u_short port = 80);
}
#endif
