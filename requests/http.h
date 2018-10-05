#pragma once
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <winsock2.h>
#include <string>

#define GET = "GET";
#define POST = "POST";

#pragma comment(lib, "Ws2_32.lib")

namespace Requests
{

	struct request {
		int content_length;
		std::string text;
		std::string status_code;
	};


	request* get(std::string url, u_short port = 80);
}