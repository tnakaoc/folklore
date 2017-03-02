#pragma once
#include "include.hpp"
#include "spinlock.hpp"
//#include <atomic>

namespace folklore{ namespace connection
{
	struct fl_cn_base
	{
		struct server;
		struct client;
	};
	class connect_t
	{
	private:
	public:
	};
	template <typename Type>
	class server_base_t_
	{
	private:
		int sock_;
		bool condition_;
		static const size_t buffer_size = Type::size;
		char* buffer_;
		void init_impl(void)
		{
			condition_ = false;
			sock_=socket(AF_INET,SOCK_STREAM,0);
			if(sock_<0) return;
			{
				int on=1;
				setsockopt(sock_,SOL_SOCKET,SO_REUSEADDR,&on,sizeof(on));
				setsockopt(sock_,SOL_TCP   ,TCP_NODELAY ,&on,sizeof(on));
			}
			struct sockaddr_in saddr;
			memset(&saddr,0,sizeof(saddr));
			saddr.sin_port   = htons(Type::port);
			saddr.sin_family = AF_INET;
			saddr.sin_addr.s_addr=htonl(INADDR_ANY);
			{
				auto ret=bind(sock_,(struct sockaddr*)&saddr,sizeof(saddr));
				if(ret<0) return;
				ret = listen(sock_,5);
				if(ret<0) return;
			}
			condition_=true;
		}
		void init(void)
		{
			init_impl();
			buffer_=nullptr;
			if(!condition_)
			{
				perror();
				close(sock);
			}
			else
			{
				buffer_=(char*)malloc(buffer_size);
			}
		}
	public:
		server_base_t_()
		{
			init();
		}
		explicit operator bool() const noexcept
		{
			return condition_;
		}
		void run(void)
		{
			Type* server = new Type();
			server.set_socket(&sock_);
			server.set_buffer(&bufer_);
			server.run();
			delete server;
		}
	};
	template <typename T>
	class connection_pair_t
	{
		T       socket;
		connect_t conn;
	public:
		void spawn();
	};
	template <typename R>
	connection_pair_t
	
}}


connect_t conn("addr");

auto srv = connection::get<connection::tsukuyomi::server>(conn);
auto cli = connection::get<connection::tsukuyomi::client>(conn);

auto server = srv.spawn();
auto client = cli.spawn();
server.run();
client

cli.send(buf);
cli.recv(buf);

