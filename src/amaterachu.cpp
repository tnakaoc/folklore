#include <aux/folklore_core.hpp>
#include <console_state.hpp>
#include <karma.hpp>
unsigned char flag[folklore::karma::aux_::tsukuyomi_size()];
unsigned char status[folklore::karma::aux_::tsukuyomi_size()];
unsigned char check;
time_t    tim;
struct tm* st;
void fmt_time(char* buf)
{
	tim=time(NULL);
	st=localtime(&tim);
	sprintf(buf,"%4d/%02d/%02d %02d:%02d:%02d",st->tm_year+1900,st->tm_mon+1,st->tm_mday,st->tm_hour,st->tm_min,st->tm_sec);
}
void read_request(int32_t soc)
{
	int16_t req(folklore::karma::aux_::tsukuyomi_size());
	send(soc,&req,sizeof(req),MSG_WAITALL);
	usleep(100);
	recv(soc,status,req,MSG_WAITALL);
}
void write_request(int32_t soc)
{
	int16_t req(0);
	send(soc,&req,sizeof(req),MSG_WAITALL);
	usleep(100);
	send(soc,flag,folklore::karma::aux_::tsukuyomi_size(),MSG_WAITALL);
}
int main(int argc,char* argv[])
{
	if(argc==1) return 0;
	memset(flag,0,folklore::karma::aux_::tsukuyomi_size());
	memset(status,0,folklore::karma::aux_::tsukuyomi_size());
	char ip[256]="127.0.0.1";
	std::string cmd(argv[1]);
	int32_t sock(0);
	folklore::make_connection<folklore::karma::fullmoon()> conn;
	if(conn.client(ip,sock)==-1)
	{
		printf("%s\n",ip);
		fprintf(stderr,"Cannot find the Tsukuyomi server\n");
		fprintf(stderr,"Maybe, the Terra is not awaken or an IP address is wrong.\n");
		return 0;
	}
	read_request(sock);
	memcpy(flag,status,sizeof(flag));
	if(argc==4)
	{
		size_t len=strlen(argv[3]);
		strncpy(ip,argv[3],256>len?len:256);
	}
	if(argc>=3)
	{
		size_t len=strlen(argv[2]);
		memcpy(flag+256,argv[2],len<255?len:255);
	}
	if(cmd=="nssta")
	{
		flag[0]=folklore::karma::nssta();
	}
	else if(cmd=="start")
	{
		flag[0]=folklore::karma::start();
	}
	else if(cmd=="stop")
	{
		flag[0]=folklore::karma::stop();
	}
	else if(cmd=="preset")
	{
		if(status[0]==folklore::karma::nssta_ok() ||
		   status[0]==folklore::karma::start_ok())
		{
			folklore::console_state con;
			con.fg_color("red");
			fprintf(stderr,"ERROR\n");
			con.reset();
			fprintf(stderr,"Please stop data taking before setting preset time value\n");
			return 0;
		}
		int32_t prs=[&]()
		{
			int32_t val{0};
			sscanf(argv[2],"%d",&val);
			for(char c:std::string(argv[2]))
			{
				switch(c)
				{
					case 's':
						return val;
					case 'm':
					case 'M':
						return val*60;
					case 'h':
					case 'H':
						return val*60*60;
					case 'd':
					case 'D':
						return val*60*60*24;
				}
			}
			return val;
		}();
		memcpy(flag,status,sizeof(flag));
		memcpy(flag+4,&prs,sizeof(prs));
		write_request(sock);
		return 0;
	}
	else if(cmd=="kill")
	{
		if(status[0]!=folklore::karma::stop_ok())
		{
			flag[0]=folklore::karma::stop();
			write_request(sock);
			sleep(1);
		}
		if(std::string(argv[2])=="lunar")
		{
			flag[1]=2;
		}
		else if(std::string(argv[2])=="terra")
		{
			flag[1]=1;
		}
		else
		{
			return 0;
		}
		write_request(sock);
		return 0;
	}
	else if(cmd=="status")
	{
		read_request(sock);
		if(status[0]==0)
		{
			fprintf(stderr,"No run information found.\n");
			fprintf(stderr,"Maybe, you have not start any run after the Terra is awaken.\n");
			return 0;
		}
		if(argc==2)
		{
			printf("%d\n",status[0]);
		}
		else
		{
			std::string opt(argv[2]);
			if(opt=="-x")
			{
				printf("0x%x\n",status[0]);
			}
			if(opt=="-c")
			{
				int32_t prs{0};
				memcpy(&prs,status+4,sizeof(prs));
				folklore::console_state con;
				con.device(stderr);
				con.bold();
				con.uline();
				fprintf(stderr,"status");
				con.reset();
				fprintf(stderr," = ");
				con.bold();
				switch(status[0])
				{
					case folklore::karma::nssta():
					case folklore::karma::nssta_ok():
						con.fg_color("green");
					   	fprintf(stderr,"no save start\n");
						break;
					case folklore::karma::start():
					case folklore::karma::start_ok():
						con.fg_color("blue");
					   	fprintf(stderr,"start\n");
						break;
					case folklore::karma::stop():
					case folklore::karma::stop_ok():
						con.fg_color("red");
					   	fprintf(stderr,"stop\n");
						break;
					default:
						break;
				}
				con.reset();
				fprintf(stderr,"\n%s\n",status+64);
				fprintf(stderr,"file       = ");
				con.fg_color("red");
				fprintf(stderr,"%s\n",status+128);
				con.fg_color("reset");
				fprintf(stderr,"comment    = ");
				con.fg_color("yellow");
				fprintf(stderr,"%s\n",status+256);
				con.reset();
				fprintf(stderr,"preset     = ");
				con.fg_color("cyan");
				if(prs!=0) fprintf(stderr,"%ds\n",prs);
				else       fprintf(stderr,"Infty\n");
				con.reset();
			}
		}
		return 0;
	}
	else
	{
		return 0;
	}
	read_request(sock);
	if((status[0]&flag[0])!=flag[0])
	{
		write_request(sock);
	}
	close(sock);
	return 0;
}
