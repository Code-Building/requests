#pragma once
#include <string>
#include <vector>

namespace ReqUtils
{
	std::vector<std::string> split(const std::string& str, char seperator);
	std::string populateUri(const std::string content);
	std::string returnBetween(const std::string& s, const std::string& start_delim, const std::string& stop_delim);
}
