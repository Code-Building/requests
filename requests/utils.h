#ifndef UTILS_H
#define UTILS_H


#include <string>
#include <vector>
#include "http.h"

namespace ReqUtils
{
	std::string parse_headers(std::map<std::string,std::string> hmap);


	//generators
	std::string populateUri(const std::string content);
	std::string generatePost(Requests::post_data pdata_map);


	//text parsing-related
	std::vector<std::string> split(const std::string& str, char seperator);
	std::string returnBetween(const std::string& s, const std::string& start_delim, const std::string& stop_delim);
}
#endif