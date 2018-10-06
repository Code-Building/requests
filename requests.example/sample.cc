#include "../requests/http.h"
#include <cassert>

int main()
{
	const Requests::post_data data = {
		{"foo","bar"}
	};

	const auto g_res = Requests::get("httpbin.org/get");
	const auto p_res = Requests::post("httpbin.org/post", data);
	const auto nf_res = Requests::get("httpbin.org/404");

	assert(g_res->status_code == "200 OK");
	assert(p_res->status_code == "200 OK");
	assert(nf_res->status_code == "404 NOT FOUND");

	printf("That's all :D");

}
