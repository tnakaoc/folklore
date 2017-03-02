#pragma once
#include <aux/folklore_core.hpp>
#include <karma.hpp>
#include <thread>
#include <atomic>
#include <stdio.h>
namespace folklore
{
	class tsukuyomi
	{
	private:
		//unsigned char flag[folklore::karma::aux_::tsukuyomi_size()];
		std::atomic_uchar flag[folklore::karma::aux_::tsukuyomi_size()];
		bool alive;
		bool verbose;
		void printinfo(void)
		{
			fprintf(stderr,"\tT S U K U Y O M I\tversion 2.00\n");
		}
	public:
		tsukuyomi():verbose(false){printinfo();}
		tsukuyomi(bool v_flag):verbose(v_flag){printinfo();}
		tsukuyomi(const tsukuyomi&)=delete;
		const tsukuyomi& operator=(const tsukuyomi&)=delete;
		size_t size(void) const
		{
			return sizeof(flag);
		}
		static tsukuyomi* get_instance(void)
		{
			static tsukuyomi ti;
			return &ti;
		}
		//unsigned char* recorder()
		std::atomic_uchar* recorder()
		{
			return flag;
		}
		void run(void)
		{
			alive=true;
			fprintf(stderr,"[tsukuyomi::run]: initializing tsukuyomi server.\n");
			std::thread fl_th([&](){ // Thread for DAQ control flag update server
				// [data] <--> [tsukuyomi]
				fprintf(stderr,"[tsukuyomi::control]: Initialized.\n");
				typedef folklore::make_connection<folklore::karma::fullmoon()> connect_type;
				connect_type conn;
				conn.server_init();
				while(alive)
				{
					if(verbose) fprintf(stderr,"[tsukuyomi::control]: Waiting for DAQ control connection request.\n");
					int dst=conn.server();
					if(dst<0) continue;
					if(verbose) fprintf(stderr,"[tsukuyomi::control]: Ready to connect socket #%d.\n",dst);
					std::thread server([&,dst](){
						if(verbose) fprintf(stderr,"[tsukuyomi::control]: Created new connection.\n");
						uint16_t fl;
						int dst_=dst;
						const int sz=folklore::karma::aux_::tsukuyomi_size();
						//unsigned char tflag[sz];
						std::atomic_uchar tflag[sz];
						//const int sz=1;
						while(alive)
						{
							int s=recv(dst_,&fl,sizeof(fl),MSG_WAITALL);
							if(s==sizeof(fl))
							{
								if(fl==0)
								{
									s=recv(dst_,flag[2]==0?flag:tflag,sz,MSG_WAITALL);
								}
								else if(fl<=sz)
								{
									s=send(dst_,flag,fl,MSG_NOSIGNAL);
								}
								else
								{
									break;
								}
							}
							if(s<=0) break;
						}
						if(verbose) fprintf(stderr,"[tsukuyomi::control]: Socket #%d disconnect.\n",dst_);
						close(dst_);
					});
					server.detach();
				}
				fprintf(stderr,"[tsukuyomi::control]: Close tsukuyomi DAQ control server.\n");
			});
			fl_th.detach();
		}
		void console(void)
		{
			while(true)
			{
				usleep(100000);
				if(flag[1]==1) break;
			}
			shutdown();
		}
		void shutdown(void)
		{
			alive=false;
			fprintf(stderr,"[tsukuyomi::shutdown]: try to shutdown tsukuyomi server.\n");
			sleep(1);
		}
		explicit operator bool() const
		{
			return alive;
		}
	};
}
