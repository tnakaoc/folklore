#pragma once
#include <stdio.h>
namespace folklore{ namespace lunar
{
	class dummy_eclipse_t
		:public folklore::lunar::shadow< dummy_eclipse_t >
	{
	private:
		bool is_start;
		char* lbuf;
		int    siz;
		FILE* fp;
	public:
		dummy_eclipse_t()
			:is_start(false)
		{
			siz=1024*1024;
			lbuf=(char*)malloc(siz);
		}
		int id(void) const
		{
			return 0;
		}
		bool initialize(void)
		{
			sleep(1);
			return true;
		}
		bool finalize(void) const
		{
			return true;
		}
		void start(void)
		{
			fp=fopen("/dev/urandom","rb");
			fread(lbuf,siz,1,fp);
			fclose(fp);
			is_start=true;
		}
		void stop(void)
		{
			is_start=false;
		}
		inline int run(char*& buf)
		{
			buf=lbuf;
			usleep(100000);
			if(is_start)
			{
				return siz;
			}
			return 0;
		}
	};
}}
