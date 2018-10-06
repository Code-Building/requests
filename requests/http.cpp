#include "http.h"
#include "utils.h"
#include <iostream>



namespace Requests
{
	request * get(std::string url, req_headers h_data, u_short port)
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

		auto get_http = "GET " + get_request_raw + " HTTP/1.1\r\nHost: " +
			request_host + "\r\n" +
			ReqUtils::parse_headers(h_data) + "\r\n\r\n";

		if (connect(Socket, reinterpret_cast<SOCKADDR *>(&sock_addr), sizeof(sock_addr)) != 0)
			throw std::exception("Could not connect through Socket.");

		send(Socket, get_http.c_str(), strlen(get_http.c_str()), 0);

		recv(Socket, buffer, 10000, 0);
		closesocket(Socket);
		WSACleanup();

		const auto rawBuff = std::string(buffer);
		this_req->content_length = atoi(ReqUtils::returnBetween(rawBuff, "Content-Length: ", "\r\n").c_str());
		this_req->status_code = ReqUtils::returnBetween(rawBuff, "HTTP/1.1 ", "\r\n");
		this_req->date = ReqUtils::returnBetween(rawBuff, "Date: ", "\r\n");
		this_req->server = ReqUtils::returnBetween(rawBuff, "Server: ", "\r\n");
		this_req->last_modified = ReqUtils::returnBetween(rawBuff, "Last-Modified: ", "\r\n");
		this_req->content_type = ReqUtils::returnBetween(rawBuff, "Content-Type: ", "\r\n");
		this_req->content_encoding = ReqUtils::returnBetween(rawBuff, "Content-Encoding: ", "\r\n");


		std::string reverse_buff;
		auto len = this_req->content_length;

		for (auto buff_iteration = sizeof buffer / sizeof *buffer; buff_iteration != 0; --buff_iteration)
		{
			if (buffer[buff_iteration] >= 32 || buffer[buff_iteration] == '\n' || buffer[buff_iteration] == '\r')
				while (len != 0)
				{
					reverse_buff += buffer[buff_iteration];
					buff_iteration--;
					len--;
				}
		}

		std::string response(reverse_buff);
		std::reverse(response.begin(), response.end());
		this_req->text = response;

		return this_req;
	}

	request* post(std::string url, post_data pdata, req_headers h_data, u_short port)
	{
		auto this_req = new request;
		char buffer[10000];
		WSADATA wsa_data;
		SOCKADDR_IN sock_addr;
		const auto requested_uri_raw = ReqUtils::populateUri(url);
		const auto request_host = ReqUtils::split(url, '/')[0];

		if (WSAStartup(MAKEWORD(2, 2), &wsa_data) != 0)
			throw std::invalid_argument("WSAStartup Failed");

		auto const Socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
		auto const host = gethostbyname(request_host.c_str());

		sock_addr.sin_port = htons(port);
		sock_addr.sin_family = AF_INET;
		sock_addr.sin_addr.s_addr = *reinterpret_cast<unsigned long *>(host->h_addr);

		auto post_data_str = ReqUtils::generatePost(pdata);

		auto post_http = "POST " + requested_uri_raw + " HTTP/1.1\r\nHost: " + request_host + "\r\n" +
			ReqUtils::parse_headers(h_data) + "Content-Length: " + std::to_string(
				post_data_str.length()) +
			"\r\n\r\n" + post_data_str + "\r\n\r\n";

		//std::cout << post_http << std::endl;

		if (connect(Socket, reinterpret_cast<SOCKADDR *>(&sock_addr), sizeof(sock_addr)) != 0)
			throw std::exception("Could not connect through Socket.");

		send(Socket, post_http.c_str(), strlen(post_http.c_str()), 0);

		recv(Socket, buffer, 10000, 0);
		closesocket(Socket);
		WSACleanup();

		const auto rawBuff = std::string(buffer);
		this_req->content_length = atoi(ReqUtils::returnBetween(rawBuff, "Content-Length: ", "\r\n").c_str());
		this_req->status_code = ReqUtils::returnBetween(rawBuff, "HTTP/1.1 ", "\r\n");
		this_req->date = ReqUtils::returnBetween(rawBuff, "Date: ", "\r\n");
		this_req->server = ReqUtils::returnBetween(rawBuff, "Server: ", "\r\n");
		this_req->last_modified = ReqUtils::returnBetween(rawBuff, "Last-Modified: ", "\r\n");
		this_req->content_type = ReqUtils::returnBetween(rawBuff, "Content-Type: ", "\r\n");


		std::string reverse_buff;
		auto len = this_req->content_length;

		for (auto buff_iteration = sizeof buffer / sizeof *buffer; buff_iteration != 0; --buff_iteration)
		{
			if (buffer[buff_iteration] >= 32 || buffer[buff_iteration] == '\n' || buffer[buff_iteration] == '\r')
			{
				while (len != 0)
				{
					reverse_buff += buffer[buff_iteration];
					buff_iteration--;
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