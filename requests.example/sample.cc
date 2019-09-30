#include "../requests/http.h"
#include <cassert>
#include "../requests/utils.h"

int main()
{
	const requests::req_headers headers = {
		{"Custom-Header", "Custom Value :D"}
	};

	const requests::post_data pdata = {
		{"foo", "bar"},
		{"fizz", "liz"}
	};
	
	const auto it = requests::get("https://www.google.com/",headers);
	const auto it2 = requests::post("httpbin.org/post", pdata, headers);
	const auto encrypted = req_utils::encrypt_str("httpbin.org/get");
	const auto g_res = requests::get("httpbin.org/get");
	const auto p_res = requests::post("httpbin.org/post", pdata);
	const auto nf_res = requests::get("httpbin.org/404");
	const auto g_explicithttp_res = requests::get("http://httpbin.org/get");
	const auto decrypturl_res = requests::get(req_utils::decrypt_str(encrypted));

	assert(g_res->status_code == "200 OK");
	assert(p_res->status_code == "200 OK");
	assert(nf_res->status_code == "404 NOT FOUND");
	assert(g_explicithttp_res->status_code == "200 OK");
	assert(decrypturl_res->status_code == "200 OK");
	assert(it->status_code == "200 OK");
	assert(it2->status_code == "200 OK");
	printf("That's all :D");
	auto dispose = getchar();
}
