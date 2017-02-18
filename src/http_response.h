#ifndef _HTTP_H
#define _HTTP_H
#include <string>
#include <unordered_map>
#include <stdint.h>
#include <vector>
#include <iterator>




class HttpResponse
{
	typedef uint16_t  HTTPstate ;
	//static const HTTPstate HTTP_200 = 200;
	//static const HTTPstate http_404 = 404;
	public:
	static std::unordered_map<HTTPstate,std::string> _stateTable;
//	static const char* const LFCR = "\r\n";
#define  LFCR "\r\n"


	public:

	HttpResponse(HTTPstate state):_state(state)
	{
	
	}

	HttpResponse&  header_add(const std::string &key,const std::string &value)
	{
		_htt_pre[key] = value;
		return *this; 
	}


	void Body_add(const std::string &body)
	{
	//	_body.clean();

		//std::copy(body.begin(),body.end(),_body.begin());
		//迭代器适配器
		std::copy(body.begin(),
						body.end(),
						std::back_inserter(_body));

	}

	std::string getPackage()//const  //"+P"
	{
		std::string package;
		package.append("HTTP/1.0 ");
		package.append(_stateTable[_state]);
		package.append(LFCR);
		//headers k:v
		for(auto e: _htt_pre)
		{
			package.append(e.first);
			package.append(":");
			package.append(e.second);
			package.append(LFCR);
		}
		package.append(LFCR);
		package.append(_body.begin(),_body.end());

		return package;
	}


	private:

		HTTPstate _state;
		std::unordered_map<std::string,std::string> _htt_pre;
		std::vector<uint16_t> _body;

};



#endif
