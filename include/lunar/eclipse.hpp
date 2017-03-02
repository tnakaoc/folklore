#pragma once
#include <lunar/shadow.hpp>
namespace utility
{
	int atoiX(char* s)
	{
		int v;
		sscanf(s,s[1]=='x'?"%x":"%d",&v);
		return v;
	}
}
#include <lunar/dummy.hpp>
