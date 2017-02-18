/*************************************************************************
    * File Name: http_request.h
    * Author   : ATM
    * Mail     : 1489308201@qq.com
    * Time     : 2017年02月15日 星期三 20时44分45秒
 ************************************************************************/

#ifndef _HTTP_SRQUEST_H
#include <iostream>	
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>


class HttpRequest
{
public:
	 static const int FILENAME_LEN = 200;
    static const int READ_BUFFER_SIZE = 2048;
    static const int WRITE_BUFFER_SIZE = 1024;
    enum METHOD { GET = 0, POST, HEAD, PUT, DELETE,
		 				TRACE, OPTIONS, CONNECT, PATCH };
    enum CHECK_STATE { CHECK_STATE_REQUESTLINE = 0, 
		 				CHECK_STATE_HEADER, CHECK_STATE_CONTENT };
    enum HTTP_CODE { NO_REQUEST, GET_REQUEST, BAD_REQUEST, 
		 					NO_RESOURCE, FORBIDDEN_REQUEST, 
							FILE_REQUEST, INTERNAL_ERROR, CLOSED_CONNECTION };
    enum LINE_STATUS { LINE_OK = 0, LINE_BAD, LINE_OPEN };

public:	
	 HttpRequest(int sockfd):m_sockfd(sockfd)
	 {
		init();
	 }
	 bool read();	   //read the request
	 //HTTP_CODE write();
	 void start()
	 {
	 	HTTP_CODE read_ret = parse_read();
		if(read_ret == NO_REQUEST)
		{
			return ;
		}


	 }
private:
	 void init();
	 HTTP_CODE parse_read();
	 HTTP_CODE parse_request_line( char* text ); //解析行
    HTTP_CODE parse_headers( char* text );		//解析头
    HTTP_CODE parse_content( char* text );		//解析消息体
    HTTP_CODE do_request();
    char* get_line() { return m_read_buf + m_start_line; }
    LINE_STATUS parse_line();


private:	
	 int m_sockfd;
    //sockaddr_in m_address;

    char m_read_buf[ READ_BUFFER_SIZE ];
    int m_read_idx;
    int m_checked_idx;
    int m_start_line;
    //char m_write_buf[ WRITE_BUFFER_SIZE ];
    //int m_write_idx;

    CHECK_STATE m_check_state;
    METHOD m_method;

    char m_real_file[ FILENAME_LEN ];
    char* m_url;		//客户端请求目录
    char* m_version;
    char* m_host;
    int m_content_length;
    bool m_linger;


};


#endif
