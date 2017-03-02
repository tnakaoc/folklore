#pragma once
#include <aux/folklore_core.hpp>
#include <thread>
#include <stdio.h>
namespace folklore
{
	class susanoho
	{
	private:
		char* ip_;
	public:
		susanoho()=delete;
		susanoho(const susanoho&)=delete;
		const susanoho& operator=(const susanoho&)=delete;
		susanoho(const char* ip)
		{
			const int siz=strlen(ip);
			if(siz!=0)
			{
				ip_=(char*)malloc(siz);
				strcpy(ip_,ip);
			}
		}
		
		
	};
}
