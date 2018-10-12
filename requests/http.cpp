#include "http.h"
#include "utils.h"
#include <iostream>
#include <sstream>

namespace Requests
{
	request* execute(const req_headers& h_data, const u_short port, const std::string& headers,
		const std::string& request_host)
	{
		auto this_req = new request;
		char buffer[10000];
		WSADATA wsa_data;
		SOCKADDR_IN sock_addr;

		if (WSAStartup(MAKEWORD(2, 2), &wsa_data) != 0)
			throw std::exception("WSAStartup Failed");

		auto const Socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
		auto const host = gethostbyname(request_host.c_str());

		sock_addr.sin_port = htons(port);
		sock_addr.sin_family = AF_INET;
		sock_addr.sin_addr.s_addr = *reinterpret_cast<unsigned long *>(host->h_addr);


		if (connect(Socket, reinterpret_cast<SOCKADDR *>(&sock_addr), sizeof(sock_addr)) != 0)
			throw std::exception("Could not connect through Socket.");

		send(Socket, headers.c_str(), int(strlen(headers.c_str())), 0);

		recv(Socket, buffer, 10000, 0);
		closesocket(Socket);
		WSACleanup();

		auto raw_buff = std::string(buffer);


		std::stringstream hexstream;
		int dec_len;
		auto const headers_len = ReqUtils::string_index(raw_buff, "\r\n\r\n");
		auto const raw_headers = raw_buff.substr(0, headers_len);
		raw_buff = raw_buff.substr(headers_len + 4);

		auto res_headers_parsed = ReqUtils::parse_res_headers(raw_headers);

		const auto implicit_lentgh = !res_headers_parsed.count("Content-Length");


		if (implicit_lentgh)
		{
			hexstream << std::hex << ReqUtils::return_between(raw_buff, "\r\n\r\n", "\r\n");
			hexstream >> dec_len;
			hexstream.seekg(0, std::ios::end);
			const int hexstream_len = hexstream.tellg();
			hexstream.seekg(0, std::ios::beg);
			raw_buff = raw_buff.substr(
				/*size of content-lentgh*/ +hexstream_len +
				/*size of \r\n*/ 2
			);
			this_req->content_length = static_cast<int>(dec_len);
		}
		else
			this_req->content_length = atoi(res_headers_parsed["Content-Length"].c_str());

		auto const response(raw_buff.substr(0, this_req->content_length));
		this_req->headers = res_headers_parsed;
		this_req->text = response;
		this_req->status_code = res_headers_parsed["Status"];

		return this_req;
	}

	request* get(std::string url, const req_headers& h_data, const u_short port)
	{
		if (ReqUtils::starts_with(url, "http://"))
			url = url.substr(7);
		else if (ReqUtils::starts_with(url, "https://"))
			url = url.substr(8);

		if (url[url.length()] != '/' && ReqUtils::string_index(url, "/") == -1)
			url += "/";

		const auto get_request_raw = ReqUtils::populate_uri(url);
		const auto request_host = ReqUtils::split(url, '/')[0];

		const auto requested_headers = "GET " + get_request_raw + " HTTP/1.1\r\nHost: " +
			request_host + "\r\n" +
			ReqUtils::parse_headers(h_data) + "\r\n\r\n";

		return execute(h_data, port, requested_headers, request_host);
	}

	request* post(std::string url, const post_data& pdata, const req_headers& h_data, const u_short port)
	{
		if (ReqUtils::starts_with(url, "http://"))
			url = url.substr(7);
		else if (ReqUtils::starts_with(url, "https://"))
			url = url.substr(8);

		if (url[url.length()] != '/' && ReqUtils::string_index(url, "/") == -1)
			url += "/";

		const auto requested_uri_raw = ReqUtils::populate_uri(url);
		const auto request_host = ReqUtils::split(url, '/')[0];
		auto post_data_str = ReqUtils::generate_post(pdata);

		const auto requested_headers = "POST " + requested_uri_raw + " HTTP/1.1\r\nHost: " + request_host + "\r\n" +
			ReqUtils::parse_headers(h_data) + "Content-Length: " + std::to_string(
				post_data_str.length()) +
			"\r\n\r\n" + post_data_str + "\r\n\r\n";

		return execute(h_data, port, requested_headers, request_host);
	}
}