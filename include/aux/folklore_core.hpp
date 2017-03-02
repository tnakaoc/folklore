#pragma once
#include "include.hpp"
#include "spinlock.hpp"
//#include <atomic>
namespace folklore
{
	class server_type
	{
	private:
		int port;
		int sock;
	public:
		server_type(int p)
			:port(p)
		{
			struct sockaddr_in saddr;
			saddr.sin_port=htons(port);
			saddr.sin_family=PF_INET;
			saddr.sin_addr.s_addr=htonl(INADDR_ANY);
			sock=socket(AF_INET,SOCK_STREAM,0);
			bind(sock,(struct sockaddr*)&saddr,sizeof(saddr));
			listen(sock,5);
		}
		int get_socket(void)
		{
			return accept(sock,0,0);
		}
	};
	class client_type
	{
	private:
		int port;
		int sock;
		int conn;
	public:
		client_type(int p)
			:port(p)
		{
		}
		bool is_connect()
		{
			return conn>=0;
		}
		int get(const char* ip)
		{
			struct sockaddr_in addr_src;
			addr_src.sin_port   = htons(port);
			addr_src.sin_family = AF_INET;
			addr_src.sin_addr.s_addr=inet_addr(ip);
			sock=socket(AF_INET,SOCK_STREAM,0);
			conn=connect(sock,(struct sockaddr*)&addr_src,sizeof(addr_src));
			return sock;
		}
	};
	template <int Port>
	struct connection_t
	{
	};
	template <int Port>
	struct make_connection
	{
		int sock;
		int server_init()
		{
			sock=socket(AF_INET,SOCK_STREAM,0);
			int on=1;
		//	{
		//		int val;
		//		socklen_t len=sizeof(val);
		//		getsockopt(sock,SOL_TCP,TCP_NODELAY,&val,&len);
		//		printf("1:TCP NODELAY = %d\n",val);
		//	}
			setsockopt(sock,SOL_SOCKET,SO_REUSEADDR,&on,sizeof(on));
			setsockopt(sock,SOL_TCP   ,TCP_NODELAY ,&on,sizeof(on));
			struct sockaddr_in saddr;
			memset(&saddr,0,sizeof(saddr));
			saddr.sin_port   = htons(Port);
			saddr.sin_family = AF_INET;
			saddr.sin_addr.s_addr=htonl(INADDR_ANY);
			bind(sock,(struct sockaddr*)&saddr,sizeof(saddr));
			listen(sock,5);
			return sock;
		}
		int server()
		{
			return accept(sock,0,0);
		}
		int client(const char* ip,int& sock)
		{
			sock=socket(AF_INET,SOCK_STREAM,0);
			int on=1;
			setsockopt(sock,SOL_SOCKET,SO_REUSEADDR,&on,sizeof(on));
			setsockopt(sock,SOL_TCP   ,TCP_NODELAY ,&on,sizeof(on));
			struct sockaddr_in addr_src;
			addr_src.sin_port   = htons(Port);
			addr_src.sin_family = AF_INET;
			addr_src.sin_addr.s_addr=inet_addr(ip);
			return connect(sock,(struct sockaddr*)&addr_src,sizeof(addr_src));
		}
		make_connection()
			:sock{0}
		{}
		~make_connection(void)
		{
			close(sock);
		}
	};
}
