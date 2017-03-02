#pragma once
#include <aux/folklore_core.hpp>
#include <vector>
#include <algorithm>
#include <thread>
namespace folklore
{
	class kaguya
	{
	private:
		int                lcnt_;
		char*              fifo_;
		bool         alive{true};
		uint32_t            pos_;
		uint32_t            end_;
		std::vector<char*> bufl_;
		uint32_t size(void) const { return 1024*1024*128; }
		uint32_t thr(void)  const { return 1024*1024*96;  }
	public:
		kaguya()=delete;
		kaguya(const kaguya&)=delete;
		const kaguya& operator=(const kaguya&)=delete;
		kaguya(const char* message)
			:lcnt_(0),pos_(0),end_(1024*1024*128)
		{
			fprintf(stderr,"\t%s\n",message);
			fifo_=(char*)malloc(size());
			bufl_.reserve(32);
			std::thread sorter([&](){
				size_t sss=bufl_.size();
				while(alive)
				{
					size_t e=bufl_.size();
					if(sss!=e)
					{
						for(size_t i=sss;i<e;++i) fprintf(stderr,"[kaguya::sorter]: Additional buffer(%p) detected.\n",bufl_[i]);
						sss=bufl_.size();
					}
					for(auto it=bufl_.begin();it!=bufl_.end();++it)
					{
						char* l=*it;
						if(l[0]==1)
						{
							int s=0x0fffffff&(*reinterpret_cast<int*>(l+1));
							//int s=0x000fffff&(*reinterpret_cast<int*>(l+1));
							memcpy(fifo_+pos_,l+1,s);
							pos_+=s;
							if(pos_>thr())
							{
								end_=pos_;
								pos_=0;
							}
							l[0]=0;
						}
					}
					usleep(1);
				}
			});
			sorter.detach();
			std::thread send_([&](){
				typedef folklore::make_connection<folklore::karma::takeyabu()> connect_type;
				connect_type conn;
				conn.server_init();
				int dst=conn.server();
				fprintf(stderr,"[kaguya::sender]: Okina connected[%d]\n",dst);
				uint32_t sp(0);
				while(alive)
				{
					char fl(0);
					int ret=recv(dst,&fl,sizeof(char),MSG_WAITALL);
					if(ret==sizeof(char)&&fl==1)
					{
						uint32_t s=(pos_>=sp?pos_-sp:end_-sp);
						if(s==0)
						{
							send(dst,&s,sizeof(int),MSG_NOSIGNAL);
							continue;
						}
						int ret=send(dst,&s,sizeof(int),MSG_NOSIGNAL);
						if(ret==sizeof(int)) ret=send(dst,fifo_+sp,s,MSG_NOSIGNAL);
						if(ret<=0) break;
						sp+=s;
						if(sp==end_) sp=0;
					}
				}
				close(dst);
			});
			send_.detach();
		}
		~kaguya()
		{
			fprintf(stderr,"[kaguya]: shutdown signal reached\n");
			alive=false;
			sleep(1);
			if(fifo_) free(fifo_);
		}
		void record(void)
		{
			std::thread dp([&](){
				typedef folklore::make_connection<folklore::karma::mt_fuji()> connect_type;
				connect_type conn;
				conn.server_init();
				while(alive)
				{
					fprintf(stderr,"[kaguya::record]: Waiting call from Lunar.\n");
					int dst=conn.server();
					if(dst<0) continue;
					std::thread th([&](){
						int dst_=dst;
						++lcnt_;
						fprintf(stderr,"[kaguya::record]: Connection from Lunar[#%d].\n",dst_);
						char* buf=(char*)malloc(1024*1024*8);
						buf[0]=0;
						bufl_.push_back(buf);
						fprintf(stderr,"[kaguya::record]: A new buffer is assigned at %p\n",buf);
						int sz=0;
						while(alive)
						{
							if(buf[0]==0)
							{
								int s=recv(dst_,&sz,sizeof(int),MSG_WAITALL);
								if(s==sizeof(int)) s=recv(dst_,buf+1,sz,MSG_WAITALL);
								if(s<=0) break;
								buf[0]=1;
							}
							else
							{
								usleep(1);
							}
						}
						if(alive)
						{
							fprintf(stderr,"[kaguya::record]: Disconnect from Lunar [#%d]\n",dst_);
							fprintf(stderr,"[kaguya::record]: Free local readout buffer at %p\n",buf);
						}
						bufl_.erase(std::remove(bufl_.begin(),bufl_.end(),buf),bufl_.end());
						fprintf(stderr,"[kaguya::record]: Readout buffer number is %lu\n",bufl_.size());
						close(dst_);
						if(buf) free(buf);
						--lcnt_;
					});
					th.detach();
				}
			});
			dp.detach();
		}
	};
}
