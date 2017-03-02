#pragma once
#include <stdio.h>
#include <string.h>
#include <aux/folklore_core.hpp>
//#include <vector>
#include <karma.hpp>
#include <pendulum.hpp>
#include <lunar/eclipse.hpp>
#include <lunar/yue.hpp>
namespace folklore{ namespace lunar
{
	class lunar
	{
	private:
		char* ip_;
		bool updated_;
		bool running_;
		bool closeok_;
		int    thcnt_;
		int32_t chfl_;
		int32_t okfl_;
		folklore::spin_lock lock;
		unsigned char flag[folklore::karma::aux_::tsukuyomi_size()];
		uint64_t clk_;
		typedef folklore::pendulum::pendulum<folklore::pendulum::period::microsec> pendulum_t;
		void fatal(void)
		{
			folklore::console_state cons;
			cons.device(stderr);
			cons.fg_color("red");
			cons.bold();
			fprintf(stderr,"[lunar::fatal]:\n");
			fprintf(stderr,"[lunar::fatal]: No route to server.\n");
			fprintf(stderr,"[lunar::fatal]: Please check your TERRA server condition.\n");
			fprintf(stderr,"[lunar::fatal]:\n");
			cons.reset();
		}
	public:
		lunar()=delete;
		lunar(const lunar&)=delete;
		lunar& operator=(const lunar&)=delete;
		lunar(const char* ip)
			:updated_(true),running_(true),closeok_(false),thcnt_(0),chfl_(0),okfl_(0),clk_(0)
		{
			ip_=(char*)malloc(strlen(ip)+1);
			memset(flag,0,sizeof(flag));
			strcpy(ip_,ip);
			std::thread flagupdate([&](){
				int dst(0);
				int ret=folklore::make_connection<folklore::karma::fullmoon()>().client(&ip_[0],dst);
				if(ret<0 || dst<0)
				{
					fprintf(stdout,"[lunar::control]: cannot connect tsukuyomi server.\n");
					fprintf(stdout,"[lunar::control]: please check terra server condtion.\n");
					closeok_=true;
					return;
				}
				fprintf(stdout,"[lunar::control]: connected to tsukuyomi server sucessfully. %d\n",dst);
				std::thread th1([&,dst](){
					int  dst__=dst;
					fprintf(stderr,"[lunar::control]: socket#=%d.\n",dst__);
					uint16_t f=1;
					int      s=0;
					int      siz =folklore::karma::aux_::tsukuyomi_size();
					while(running_)
					{
						if(!updated_)
						{
							f=2;
							s=send(dst__,&f,sizeof(f),MSG_NOSIGNAL);
							if(s==sizeof(f)) s=recv(dst__,flag,f,MSG_WAITALL);
							if(s<=0) break;
						}
						else
						{
							f=0;
							s=send(dst__,&f,sizeof(f),MSG_NOSIGNAL);
							if(s==sizeof(f)) s=send(dst__,flag,siz,MSG_NOSIGNAL);
							if(s<=0) break;
							updated_=false;
						}
						usleep(10000);
					}
					f=siz+1;
					send(dst__,&f,sizeof(f),MSG_NOSIGNAL);
					close(dst__);
					fprintf(stderr,"[lunar::control]: disconencted to tsukuyomi server.\n");
					closeok_=true;
				});
				th1.detach();
			});
			flagupdate.detach();
			std::thread state([&](){
				unsigned char c=flag[0];
				for(;;)
				{
					if(c!=flag[0]&&thcnt_!=0)
					{
						while(chfl_!=okfl_)
						{
							usleep(10000);
						}
						flag[0]|=1<<7;
						c=flag[0];
						fprintf(stderr,"[lunar::state]: %d flags are updated\n",thcnt_);
						fprintf(stderr,"[lunar::state]: flag[0]=%d\n",flag[0]);
						updated_=true;
						chfl_=0;
						usleep(100);
					}
					else
					{
						usleep(10000);
					}
					if(closeok_) break;
				}
			});
			state.detach();
			std::thread clock([&](){
				pendulum_t pendulum;
				while(running_)
				{
					if(flag[0]==folklore::karma::start_ok()||flag[0]==folklore::karma::nssta_ok())
					{
						pendulum.sway(clk_);
						for(;;)
						{
							if(flag[0]==folklore::karma::stop_ok()) break;
							usleep(100);
						}
						pendulum.stop();
						clk_=0;
					}
					else
					{
						usleep(1000);
					}
				}
			});
			clock.detach();
		}
		~lunar()
		{
			if(ip_) free(ip_);
			fprintf(stderr,"[lunar::system]: shutdown signal reached at lunar object.\n");
			usleep(1000);
			fprintf(stderr,"[lunar::system]: ready to shutdown.\n");
			fprintf(stderr,"[lunar::system]: By!\n");
		}
		template <typename Eclipse>
		bool observe(Eclipse* e)
		{
			typedef folklore::make_connection<folklore::karma::mt_fuji()> connect_type;
			connect_type conn;
			int dst(0);
			int ret(0);
			while(dst<=0)
			{
				ret = conn.client(ip_,dst);
				usleep(1000);
			}
			bool thok=false;
			if(ret==-1)
			{
				fprintf(stderr,"[lunar::observe]: id:%d connection failed\n",e->id());
				fatal();
				flag[1]=1;
				return false;
			}
			std::thread th([&,dst](){
				Eclipse eclipse=*e;
				eclipse.life(&clk_);
				fprintf(stderr,"[lunar::observe]: initializing board%d...\n",eclipse.id());
				eclipse.P1();
				fprintf(stderr,"[lunar::observe]: Done!\n");
				int ldst=dst;
				fprintf(stderr,"[lunar::observe]: connection for kaguya#%d eclipse#%d\n",ldst,eclipse.id());
				unsigned char c=~flag[0];
				int thid=thcnt_;
				okfl_|=1<<thcnt_;
				++thcnt_;
				thok=true;
				eclipse.total(ldst,updated_);
				for(;;)
				{
					if(c!=flag[0])
					{
						switch(flag[0])
						{
							case folklore::karma::start():
							case folklore::karma::nssta():
								lock.lock();
								fprintf(stderr,"[lunar::observe]: thread#%d kaguya#%d start signal reached.\n",thid,ldst);
								eclipse.P2();
								lock.unlock();
//								eclipse.total(ldst,updated_);
								break;
							case folklore::karma::stop():
								lock.lock();
								fprintf(stderr,"[lunar::observe]: thread#%d kaguya#%d stop signal reached.\n",thid,ldst);
								eclipse.P4();
								lock.unlock();
								break;
							default:
								usleep(1);
								continue;
						}
						for(;;)
						{
							char tmp=chfl_;
							tmp|=1<<thid;
							if((chfl_|=1<<thid)==tmp) break;
						}
						c=flag[0];
						usleep(1);
					}
					else if(flag[1]!=0)
					{
						break;
					}
					else
					{
						usleep(10000);
					}
					if(lock) lock.unlock();
				}
				close(ldst);
				eclipse.P5();
			});
			th.detach();
			fprintf(stderr,"[lunar::observe[%d]]: Waiting for thread initialization...\n",__LINE__);
			while(!thok) usleep(1000);
			fprintf(stderr,"[lunar::observe[%d]]: Done!\n",__LINE__);
			return true;
		}
		void console(void)
		{
			while(flag[1]==0)
			{
				usleep(100000);
			}
		}
	};
}}
