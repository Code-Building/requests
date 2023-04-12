#pragma once

#include <string>
#include <vector>
#include "http.h"

namespace req_utils
{
	//converters
	LPCWSTR to_lpcwstr(const char* text);

	//generators
	std::string populate_uri(const std::string& content);
	std::string generate_post(requests::post_data pdata_map);
	std::map<std::string, std::string> parse_headers(std::map<std::string, std::string> h_map, DWORD verb);
	std::map<std::string, std::string> parse_res_headers(std::string raw_headers);


	//text parsing-related
	std::vector<std::string> split(const std::string& str, char separator);
	std::string return_between(const std::string& s, const std::string& start_delim, const std::string& stop_delim);
	bool starts_with(const std::string& str, const std::string& who);
	int string_index(const std::string& str, const std::string& who);

	//simple-crypt
	std::string encrypt_str(std::string vulstr);
	std::string decrypt_str(std::string vulstr);
}