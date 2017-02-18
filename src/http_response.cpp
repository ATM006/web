#include "http_response.h"



std::unordered_map<HttpResponse::HTTPstate,std::string> HttpResponse::_stateTable = {
	{200,"OK"},
	{404,"NOT FOND"}
};


