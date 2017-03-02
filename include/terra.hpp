#pragma once
#include <aux/folklore_core.hpp>
#include <karma.hpp>
#include <terra/kaguya.hpp>
#include <terra/okina.hpp>
#include <terra/tsukuyomi.hpp>
#include <terra/mikado.hpp>
#include <sys/file.h>
#include <fcntl.h>
namespace folklore{namespace terra
{
	class terra
	{
	private:
		folklore::kaguya kaguya;
		folklore::okina   okina;
		int fd;
		bool success;
		void fatal(void)
		{
			folklore::console_state cons;
			cons.device(stderr);
			cons.fg_color("red");
			cons.bold();
			fprintf(stderr,"[terra::fatal]:\n");
			fprintf(stderr,"[terra::fatal]: Failed to pass multiple startup checking.\n");
			fprintf(stderr,"[terra::fatal]:\n");
			cons.reset();
		}
	public:
		terra(const char* message)
			:kaguya("K A G U Y A\tversion 1.01"),okina("O K I N A\tversion 2.00")
		{
			fd = open("/tmp/terra", O_CREAT|O_RDWR, S_IRUSR | S_IWUSR);
			auto ret = flock(fd,LOCK_NB|LOCK_EX);
			success = (ret==0);
			fprintf(stderr,"\n------------------------------------------\n");
			fprintf(stderr,"\n\t%s\n\n",message);
			fprintf(stderr,"------------------------------------------\n\n");
		}
		~terra()
		{
			flock(fd,LOCK_NB|LOCK_UN);
			close(fd);
		}
		void genesis(void)
		{
			if(!success)
			{
				fatal();
				return;
			}
			fprintf(stderr,"[terra::genesis]: TERRA server initializing...\n");
			fprintf(stderr,"[terra::genesis]: Please wait for running any lunar processes.\n");
			folklore::tsukuyomi* tsukuyomi=tsukuyomi::get_instance();
			tsukuyomi->run();
			sleep(1);
			kaguya.record();
			sleep(1);
			okina.record(tsukuyomi->recorder());
			sleep(1);
			fprintf(stderr,"[terra::genesis]: Done!\n");
			fprintf(stderr,"[terra::genesis]: You can run lunar.\n");
			tsukuyomi->console();
		}
		explicit operator bool() const noexcept
		{
			return success;
		}
	};
}}
