#include <stdio.h>
#include <stdlib.h>
#include <strings.h>
#include <sys/types.h>	       /* See NOTES */
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <vector>
#include "http_response.h"
#include "http_request.h"
#include <string>


#define	IP "127.0.0.1"			/*  */
#define	PORT 8080			/*  */

//socket server
// socket()
// bind
// listen(socket, 5) //状态，　主动，被动
// accept(socket,
int main() {

  int listenFd = socket(AF_INET,
                        SOCK_STREAM,
                        0);
  if (listenFd == -1) {
    perror("socket failed");
    exit(1);
  }

  struct sockaddr_in listenAddr;

  bzero(&listenAddr, sizeof(listenAddr));
  listenAddr.sin_family = AF_INET;
  //ip 
  inet_pton(AF_INET, 
            IP,
            &(listenAddr.sin_addr.s_addr));

  //port
  listenAddr.sin_port = htons(PORT);

  //支持可扩展类型
  int ret = bind(listenFd, 
                 (struct sockaddr *) &listenAddr,
                 sizeof(listenAddr));

  if (ret == -1) {
    perror("bind failed");
    exit(1);
  }
  printf("bind successed %s:%d\n", IP, PORT);

  ret = listen(listenFd, 5);
  if (ret == -1) {
    perror("listen failed");
    exit(1);
  }
  printf("listen successed %s:%d\n", IP, PORT);

  //char buff[1024]={0};
  int clientFd = -1;
  while (1) {
    clientFd = accept(listenFd, NULL, 0);
    printf("a new connect is establishing\n");
	//请求
	/* 
	 int ret = recv(clientFd,buff,sizeof(buff),0);
	 if(ret == -1)		//-1 是否整整发生错误
	 {
	 	perror("recv");
		close(clientFd);
		
	 }
	*/
	//读取数据，判读数据是否完整
	 HttpRequest req(clientFd);
	 req.read();
	 req.start();
	

	
	HttpResponse  rep(200);
   
	rep.header_add("Accept-Ranges","bytes")
		 .header_add("Accept-Ranges", "bytes")
		 .header_add("Last-Modified",
				       "Fri, 13 Jan 2017 03:13:14 GMT")
		 .header_add("Content-Type",
	                "text/html; charset=UTF-8")
		 .header_add("Content-Length", "5")
		 .header_add("Date", 
                   "Fri, 10 Feb 2017 03:55:49 GMT");
	 rep.Body_add("hello");
	 
	 std::string package = rep.getPackage();


	 //printf("%s\n",buff);
    // do HTTP 
    
	 // 
	
	 //printf("%s\n",package.c_str());
//	 send(clientFd,package.data(),package.length(),0);
	

	 close(clientFd);
    clientFd = -1;
  }




}
