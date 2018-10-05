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

	std::string populateUri(const std::string content)
	{
		std::string uri;
		auto array = split(content, '/');
		for (auto i = 1; array.size() > i; i++)
			uri += "/" + array[i];
		uri += "/";
		uri.pop_back();
		return uri;
	}

	std::string generatePost(Requests::post_data pdata_map)
	{
		std::string generated;
			for (auto& it : pdata_map)
				generated += it.first + "=" + it.second + "&";
		generated.pop_back();
		return generated;
	}

	std::string returnBetween(const std::string& s, const std::string& start_delim, const std::string& stop_delim)
	{
		unsigned first_delim_pos = s.find(start_delim);
		unsigned end_pos_of_first_delim = first_delim_pos + start_delim.length();
		std::string fixed_next_search = s.substr(end_pos_of_first_delim);
		unsigned last_delim_pos = fixed_next_search.find(stop_delim);

		return fixed_next_search.substr(0,
			last_delim_pos);
	}
}
