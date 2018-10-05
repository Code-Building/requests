#pragma once
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <winsock2.h>
#include <string>

#pragma comment(lib, "Ws2_32.lib")

namespace Requests
{

	struct request {
		//general
		std::string tmp_buff;


		std::string status_code;
		std::string date;
		std::string server;
		std::string last_modified;
		int content_length;
		std::string content_type;
		std::string text;
	};


	request* get(std::string url, u_short port = 80);
}