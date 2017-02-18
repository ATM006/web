

#http 请求报文

GET / HTTP/1.1
Host: 127.0.0.1:8080
User-Agent: Mozilla/5.0 (X11; Ubuntu; Linux x86_64; rv:50.0) Gecko/20100101 Firefox/50.0
Accept: text/html,application/xhtml+xml,application/xml;q=0.9,*/*;q=0.8
Accept-Language: en-US,en;q=0.5
Accept-Encoding: gzip, deflate
Connection: keep-alive
Upgrade-Insecure-Requests: 1


#请求
#
#读取数据
#读取行
#解析请求头
#解析body
#

class HttpRequest
{
	enum METHOD{GET=0,POST};

public:
	
	void read();
	void read_line();
	void x_head();
	void X_body();
private:
	METHOD m_method;
	char *m_url;
	char *m_version;
	char *m_host;
	char *m_address;
	int m_ip;

};





#**********************************************************
web: main.cpp  

I suspect this is a C++ ABI issue. The ABI for std::string has changed in GCC 5 (related to C++11 requirements, but it applies even if you aren't using C++11). See:

https://gcc.gnu.org/gcc-5/changes.html#libstdcxx

If libprotobuf was built with GCC 4.x or prior, but your app is built with GCC 5, then you will see problems, because libprotobuf uses std::string in its interface. You have two options:

  1.  Rebuild libprotobuf with GCC 5 (but now any apps built with GCC 4 won't work with the new version of libprotobuf).
  2.  Build you app with  
	 #-D_GLIBCXX_USE_CXX11_ABI=0
	 as described at the above link. This will force GCC to use the old ABI version.
#*****************************************************************
