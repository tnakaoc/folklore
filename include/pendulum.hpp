#pragma once
#include <thread>
#include <time.h>
namespace folklore
{
	namespace pendulum
	{
		namespace mode
		{
			typedef decltype(CLOCK_REALTIME) mode_t;
			struct realtime
			{
				static const mode_t  mode=CLOCK_REALTIME;
			};
			struct process_cputime
			{
				static const mode_t mode=CLOCK_PROCESS_CPUTIME_ID;
			};
			struct monotonic
			{
				static const mode_t mode=CLOCK_MONOTONIC;
			};
			struct monotonic_raw
			{
				static const mode_t mode=CLOCK_MONOTONIC_RAW;
			};
		}
		namespace period
		{
			struct microsec
			{
				static const uint64_t resol=1;
			};
			struct msec
			{
				static const uint64_t resol=microsec::resol*1000;
			};
			struct sec
			{
				static const uint64_t resol=msec::resol*1000;
			};
			struct min
			{
				static const uint64_t resol=sec::resol*60;
			};
			struct hour
			{
				static const uint64_t resol=min::resol*60;
			};
			struct day
			{
				static const uint64_t resol=hour::resol*24;
			};
		}
		template <typename Resol_,typename Mode_=pendulum::mode::realtime>
		class pendulum
		{
		private:
			char flag;
			char stfl;
		public:
			pendulum()
				:flag(0),stfl(1)
			{
			}
			~pendulum()
			{
			}
			void sway(uint64_t& clock)
			{
				std::thread th([&](){
					struct timespec res;
					struct timespec start;
					clock=0;
					clock_gettime(Mode_::mode,&res);
					clock_gettime(Mode_::mode,&start);
					flag=0;
					while(flag==0)
					{
						clock_gettime(Mode_::mode,&res);
						clock=(uint64_t)(((res.tv_sec-start.tv_sec)*1000000000+(res.tv_nsec-start.tv_nsec))/Resol_::resol/1000);
						usleep(Resol_::resol);
					}
				});
				th.detach();
			}
			void stop(void)
			{
				flag=stfl;
			}
		};
	}
}
