#pragma once
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <winsock2.h>
#include <string>
#include <map>

#pragma comment(lib, "Ws2_32.lib")

namespace Requests
{

	struct request {
		//general
		std::string status_code;
		std::string date;
		std::string server;
		std::string last_modified;
		int content_length;
		std::string content_type;
		std::string text;
	};

	typedef std::map<std::string, std::string> post_data;


	request* get(std::string url, u_short port = 80);
	request* post(std::string url, post_data pdata,u_short port = 80);

}