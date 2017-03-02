#pragma once
#include <karma.hpp>
#include <future>
#include <utility>
namespace folklore{namespace lunar
{
#define FL_EC_CRTP(fun) static_cast<eclipse_t*>(this)->fun
	template <typename Eclipse>
	class shadow
	{
	public:
		typedef Eclipse eclipse_t;
		typedef folklore::karma::header_base_t<0> header_t;
		uint64_t* clk_;
		void life(uint64_t* l)
		{
			clk_=l;
		}
		size_t malloc_size(void) const
		{
			return 0;
		}
		int id(void) const
		{
			return 0;
		}
		inline void P1(void)
		{
			fprintf(stderr,"[shadow::initialize]: initializeing board#%d...\n",FL_EC_CRTP(id)());
			FL_EC_CRTP(initialize)();
			fprintf(stderr,"[shadow::initialize]: #%d Done!\n",FL_EC_CRTP(id)());
		}
		inline void P2(void)
		{
			FL_EC_CRTP(start)();
		}
		inline void total(int dst,bool& ok_)
		{
	//	/*
			std::thread th([&](){
				typename eclipse_t::header_t header;
				char* buf=nullptr;
				while(!ok_){usleep(1000);}
				fprintf(stderr,"[shadow::run]: start communication for kaguya#%d\n",dst);
				for(;;)
				{
					int s=FL_EC_CRTP(run)(buf);
					if(s==-1) break;
					if(s==0) continue;
					header.set_time(*clk_);
					usleep(10);
					s+=header.size();
					header.set_size(s);
					header.set_uid(FL_EC_CRTP(id)());
					memcpy(buf-header.size(),&header,header.size());
					if(send(dst,&s,sizeof(int),MSG_NOSIGNAL)==0) break;
					if(send(dst,buf-header.size(),s,MSG_NOSIGNAL)==0) break;
				}
				fprintf(stderr,"[shadow::run]: end communication for kaguya#%d\n",dst);
			});
			th.detach();
	//		*/
//			auto ret=std::async(std::launch::async,
//				[&,this](bool& fl){
//					typename eclipse_t::header_t header;
//					char* buf=nullptr;
//					while(!fl){usleep(100);}
//					fprintf(stderr,"[shadow::run]: start communication for kaguya#%d\n",dst);
//					for(;;)
//					{
//						int s=FL_EC_CRTP(run)(buf);
//						if(s==-1) break;
//						if(s==0) continue;
//						header.set_time(*clk_);
//						usleep(10);
//						s+=header.size();
//						header.set_size(s);
//						header.set_uid(FL_EC_CRTP(id)());
//						memcpy(buf-header.size(),&header,header.size());
//						if(send(dst,&s,sizeof(int),MSG_NOSIGNAL)==0) break;
//						if(send(dst,buf-header.size(),s,MSG_NOSIGNAL)==0) break;
//						usleep(1);
//					}
//					fprintf(stderr,"[shadow::run]: end communication for kaguya#%d\n",dst);
//				},
//				std::ref(ok_)
//			);
			usleep(10);
		}
		inline void P4(void)
		{
			FL_EC_CRTP(stop)();
		}
		inline void P5(void)
		{
			fprintf(stderr,"[shadow::finalize]: finalizeing board#%d...\n",FL_EC_CRTP(id)());
			FL_EC_CRTP(finalize)();
			fprintf(stderr,"[shadow::finalize]: #%d Done!\n",FL_EC_CRTP(id)());
		}
	};
}}
