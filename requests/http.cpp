#include "http.h"
#include "utils.h"
#include <iostream>



namespace Requests
{
	request * get(std::string url, u_short port)
	{
		auto this_req = new request;
		char buffer[10000];
		WSADATA wsa_data;
		SOCKADDR_IN sock_addr;
		const auto get_request_raw = ReqUtils::populateUri(url);
		const auto request_host = ReqUtils::split(url, '/')[0];

		if (WSAStartup(MAKEWORD(2, 2), &wsa_data) != 0)
			throw std::invalid_argument("WSAStartup Failed");

		auto const Socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
		auto const host = gethostbyname(request_host.c_str());

		sock_addr.sin_port = htons(port);
		sock_addr.sin_family = AF_INET;
		sock_addr.sin_addr.s_addr = *reinterpret_cast<unsigned long *>(host->h_addr);

		//std::cout << get_request_raw << std::endl;

		auto get_http = "GET " + get_request_raw + " HTTP/1.1\r\nHost: " + request_host + "\r\nConnection: close\r\n\r\n";

		//std::cout << get_http << std::endl;

		if (connect(Socket, reinterpret_cast<SOCKADDR *>(&sock_addr), sizeof(sock_addr)) != 0)
			throw std::invalid_argument("could not connect");

		send(Socket, get_http.c_str(), strlen(get_http.c_str()), 0);

		int nDataLength;
		while ((nDataLength = recv(Socket, buffer, 10000, 0)) > 0) {}
		closesocket(Socket);
		WSACleanup();

		const auto rawBuff = std::string(buffer);
		this_req->content_length = atoi(ReqUtils::returnBetween(rawBuff, "Content-Length: ", "\r\n").c_str());
		this_req->status_code = ReqUtils::returnBetween(rawBuff, "HTTP/1.1 ", "\r\n");

		std::string reverse_buff;
		auto len = this_req->content_length;

		for (auto buffIteration = sizeof buffer / sizeof *buffer; buffIteration != 0; --buffIteration)
		{
			if (buffer[buffIteration] >= 32 || buffer[buffIteration] == '\n' || buffer[buffIteration] == '\r')
			{
				while (len != 0)
				{
					reverse_buff += buffer[buffIteration];
					buffIteration--;
					len--;
				}
			}
		}

		std::string response(reverse_buff);
		std::reverse(response.begin(), response.end());
		this_req->text = response;

		return this_req;
	}

}
