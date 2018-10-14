// This file is part of the "requests" project, https://github.com/Code-Building/requests
//   (c) 2018 Code Building https://codebuilding.org
//
// Licensed under the MIT License (the "License"); you may not use this
// file except in compliance with the License. You may obtain a copy of
// the License at: http://opensource.org/licenses/MIT


#include "utils.h"


/*This namespace is responsible of utilities on processing sockets HTTP responses and their data*/
namespace ReqUtils
{
	// Split function will receive a string and a char. The character will be responsible by 
	// the separator between strings of the given string.
	// the first step is to loop a function while a given character exists in the given string
	// then, in that function, if the char was found, it assigns a index value to it.
	// with that value, we can use substring to get the value between the last found index and
	// actual found index - last found index. 
	// (last found index will be actual found index+1 at the iteration end)
	// after that loop, we also need to return the value between last found index and actual 
	// found index - last found index to ensure that strings like
	// a:b with the separator ':' return a & b.
	// we can visualize that with:
	// std::string x = "Hi:How:Are:You"; const char separator = ':';
	// first loop iteration, found separator at index 2, the substr will be substr(0, (2-0));
	// resulting in "Hi" pushed onto output vector
	// second loop iteration, found separator at index 6, the substr will be substr(3, (6-3));
	// resulting in "How" pushed onto output vector
	// the loop will remains until all separators got consumed.
	// in this case, at "Are" since "You" don't have ':' at the end.
	// as we need to cover "You" also, we do a last substring bewteen last ':' index and string end.
	// and push it onto vector.
	std::vector<std::string> split(const std::string& str, const char seperator)
	{
		std::vector<std::string> output;
		std::string::size_type prev_pos = 0, pos = 0;

		while ((pos = str.find(seperator, pos)) != std::string::npos)
		{
			auto substring(str.substr(prev_pos, pos - prev_pos));
			output.push_back(substring);
			prev_pos = ++pos;
		}

		output.push_back(str.substr(prev_pos, pos - prev_pos));
		return output;
	}

	// Parse Headers function will recieve a map of two std::string's and return 
	// the HTTP-style request headers, -> Name: Value CLRF <-
	// this is solved by looping through all given map keys and values and formatting
	// then like -> Name: Value CLRF <-.
	// so if the given map has 1 index and it is "User-Agent" the return will be
	// User-Agent: h_map["User-Agent"]\r\n
	std::map<std::string, std::string> parse_headers(std::map<std::string, std::string> h_map, const DWORD verb)
	{
		if (!h_map.count("Content-Type") && verb == 2)
		{
			h_map["Content-Type"] = "application/x-www-form-urlencoded";
		}
		if (!h_map.count("User-Agent"))
		{
			h_map["User-Agent"] = "Requests 2.0";
		}
		return h_map;
	}

	// Parse Response Headers function will recieve a raw std::string of response headers
	// a valid http response should start with
	// HTTP/N.N STATUS CODE
	// so let's deal with it.
	// we simply do a 8 characters -to- the first CLRF index substring to retrieve
	// the response code.
	// then, we can just apply the split function logic to get all headers between CLRF
	std::map<std::string, std::string> parse_res_headers(std::string raw_headers)
	{
		std::map<std::string, std::string> formatted;
		const auto x = string_index(raw_headers, "\r\n");
		auto http_res = raw_headers.substr(8, x - 8);
		http_res.erase(0, 1);
		formatted["Status"] = http_res;
		raw_headers = raw_headers.substr(x);

		while (string_index(raw_headers, "\r\n") != -1)
		{
			raw_headers = raw_headers.substr(string_index(raw_headers, "\r\n") + 2);
			const auto delimeter = string_index(raw_headers, ":");
			auto value = return_between(raw_headers, ":", "\r\n");
			value.erase(0, 1);
			const auto header = raw_headers.substr(0, delimeter);
			formatted[header] = value;
		}

		return formatted;
	}

	LPCWSTR to_lpcwstr(const char* text)
	{
		const auto size = strlen(text) + 1;
		auto* wtext = new wchar_t[size];
		size_t out_size;
		mbstowcs_s(&out_size, wtext, size, text, size - 1);
		return wtext;
	}

	std::string populate_uri(const std::string& content)
	{
		std::string uri;
		auto array = split(content, '/');
		for (auto i = 1; array.size() > i; i++)
			uri += "/" + array[i];
		return uri;
	}

	std::string generate_post(Requests::post_data pdata_map)
	{
		std::string generated;
		for (auto& it : pdata_map)
			generated += it.first + "=" + it.second + "&";
		generated.pop_back();
		return generated;
	}

	std::string return_between(const std::string& s, const std::string& start_delim, const std::string& stop_delim)
	{
		const auto first_delim_pos = s.find(start_delim);
		const auto end_pos_of_first_delim = first_delim_pos + start_delim.length();
		auto fixed_next_search = s.substr(end_pos_of_first_delim);
		const auto last_delim_pos = fixed_next_search.find(stop_delim);

		return fixed_next_search.substr(0,
		                                last_delim_pos);
	}

	bool starts_with(const std::string& str, const std::string& who)
	{
		return str.rfind(who, 0) == 0;
	}

	int string_index(const std::string& str, const std::string& who)
	{
		return str.find(who);
	}

	std::string encrypt_str(std::string vulstr)
	{
		auto v_str = vulstr;
		uint8_t doinc = 0;
		for (size_t i = 0; i < vulstr.length(); i++)
		{
			if (doinc == 5) doinc = 0;
			*reinterpret_cast<uint8_t*>(&v_str[i]) = *reinterpret_cast<uint8_t*>(&vulstr[i]) + doinc;
			doinc++;
		}
		return v_str;
	}

	std::string decrypt_str(std::string vulstr)
	{
		auto v_str = vulstr;
		uint8_t doinc = 0;
		for (size_t i = 0; i < vulstr.length(); i++)
		{
			if (doinc == 5) doinc = 0;
			*reinterpret_cast<uint8_t*>(&v_str[i]) = *reinterpret_cast<uint8_t*>(&vulstr[i]) - doinc;
			doinc++;
		}
		return v_str;
	}
}
