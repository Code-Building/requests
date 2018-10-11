#include "utils.h"

namespace ReqUtils
{
	std::vector<std::string> split(const std::string& str, const char seperator)
	{
		std::vector<std::string> output;
		std::string::size_type prev_pos = 0, pos = 0;

		while ((pos = str.find(seperator, pos)) != std::string::npos)
		{
			std::string substring(str.substr(prev_pos, pos - prev_pos));
			output.push_back(substring);
			prev_pos = ++pos;
		}

		output.push_back(str.substr(prev_pos, pos - prev_pos));
		return output;
	}

	std::string parse_headers(std::map<std::string, std::string> h_map)
	{
		std::string headers;
		for (auto& it : h_map)
			headers += it.first + ": " + it.second + "\r\n";
		return headers;
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
	std::string encryptStr(std::string vulstr)
	{
		std::string vStr = vulstr;
		uint8_t doinc = 0;
		for (size_t i = 0; i < vulstr.length(); i++)
		{
			if (doinc == 5)
				doinc = 0;
			*(uint8_t*)(&vStr[i]) = *(uint8_t*)(&vulstr[i]) + doinc;
			doinc++;
		}
		return vStr;
	}

	std::string decryptStr(std::string vulstr)
	{
		std::string vStr = vulstr;
		int doinc = 0;
		for (size_t i = 0; i < vulstr.length(); i++)
		{
			if (doinc == 5)
				doinc = 0;
			*(uint8_t*)(&vStr[i]) = *(uint8_t*)(&vulstr[i]) - doinc;
			doinc++;
		}
		return vStr;
	}
}
