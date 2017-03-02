#pragma once
#include <stdio.h>
namespace folklore
{
	struct console_state
	{
		decltype(stdout) fd;
		console_state()
			:fd(stdout)
		{
		}
		void device(decltype(stdout) fda)
		{
			fd=fda;
		}
		void char_color(const std::string& col) const
		{
			if(col=="black")   { fprintf(fd,"\x1b[30m"); return; }
			if(col=="red")     { fprintf(fd,"\x1b[31m"); return; }
			if(col=="green")   { fprintf(fd,"\x1b[32m"); return; }
			if(col=="yellow")  { fprintf(fd,"\x1b[33m"); return; }
			if(col=="blue")    { fprintf(fd,"\x1b[34m"); return; }
			if(col=="mazenta") { fprintf(fd,"\x1b[35m"); return; }
			if(col=="cyan")    { fprintf(fd,"\x1b[36m"); return; }
			if(col=="white")   { fprintf(fd,"\x1b[37m"); return; }
			if(col=="reset")   { fprintf(fd,"\x1b[39m"); return; }
		}
		void char_color(const char* col) const
		{
			char_color(std::string(col));
		}
		void fg_color(const char* col) const
		{
			char_color(std::string(col));
		}
		void fg_color(const std::string& col) const
		{
			char_color(std::string(col));
		}
		//
		void bg_color(const std::string& col) const
		{
			if(col=="black")   { fprintf(fd,"\x1b[40m"); return; }
			if(col=="red")     { fprintf(fd,"\x1b[41m"); return; }
			if(col=="green")   { fprintf(fd,"\x1b[42m"); return; }
			if(col=="yellow")  { fprintf(fd,"\x1b[43m"); return; }
			if(col=="blue")    { fprintf(fd,"\x1b[44m"); return; }
			if(col=="mazenta") { fprintf(fd,"\x1b[45m"); return; }
			if(col=="cyan")    { fprintf(fd,"\x1b[46m"); return; }
			if(col=="white")   { fprintf(fd,"\x1b[47m"); return; }
			if(col=="reset")   { fprintf(fd,"\x1b[49m"); return; }
		}
		void bg_color(const char* col) const
		{
			bg_color(std::string(col));
		}
		//
		void uline(void) const
		{
			fprintf(fd,"\x1b[4m");
		}
		void underline(void) const
		{
			fprintf(fd,"\x1b[4m");
		}
		void bold(void) const
		{
			fprintf(fd,"\x1b[1m");
		}
		void flip(void) const
		{
			fprintf(fd,"\x1b[7m");
		}
		void reset(void) const
		{
			fprintf(fd,"\x1b[0m");
			fprintf(fd,"\x1b[49m");
		}
	};
}
