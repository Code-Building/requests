#include "../requests/http.h"
#include <cassert>
#include "../requests/utils.h"

int main()
{
	const Requests::post_data data = {
		{"foo", "bar"}
	};

	const auto encrypted = ReqUtils::encrypt_str("httpbin.org/get");

	const auto g_res = Requests::get("httpbin.org/get");
	const auto p_res = Requests::post("httpbin.org/post", data);
	const auto nf_res = Requests::get("httpbin.org/404");
	const auto g_explicithttp_res = Requests::get("http://httpbin.org/get");
	const auto decrypturl_res = Requests::get(ReqUtils::decrypt_str(encrypted));

	assert(g_res->status_code == "200 OK");
	assert(p_res->status_code == "200 OK");
	assert(nf_res->status_code == "404 NOT FOUND");
	assert(g_explicithttp_res->status_code == "200 OK");
	assert(decrypturl_res->status_code == "200 OK");

	printf("That's all :D");
	getchar();
}
