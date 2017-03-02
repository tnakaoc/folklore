#pragma once
#include <karma.hpp>
#include <stdio.h>
#include <string.h>
namespace folklore
{
	class okina
	{
	private:
		int        rnum_;
		uint64_t fs_thr_;
		char*     rname_;
		char*      path_;
	public:
		okina()=delete;
		okina(const okina&)=delete;
		const okina& operator=(const okina&)=delete;
		okina(const char* message)
			:rnum_{0},fs_thr_{0},rname_{nullptr},path_{nullptr}
		{
			fprintf(stderr,"\t%s\n",message);
			FILE* fp=fopen(".runnumber","r");
			if(fp)
			{
				fscanf(fp,"%d",&rnum_);
			}
			else
			{
				fp=fopen(".runnumber","w");
				fprintf(fp,"%d",rnum_);
			}
			fclose(fp);
			fp=fopen(".terra","r");
			if(!fp)
			{
				fp=fopen("~/.terra","r");
			}
			if(fp)
			{
				char line[256];
				while(fgets(line,256,fp)!=nullptr)
				{
					if(line[0]=='#') continue;
					char src[128];
					char dst[128];
					for(char& c:line)
					{
						if(c=='='){c=' '; break;}
					}
					sscanf(line,"%s %s",src,dst);
					auto len=strlen(dst);
					if(len==0) continue;
					if(strcmp(src,"path")==0)
					{
						path_=(char*)malloc(len+1);
						strcpy(path_,dst);
					}
					else if(strcmp(src,"name")==0)
					{
						rname_=(char*)malloc(len+1);
						strcpy(rname_,dst);
					}
					else if(strcmp(src,"filesize")==0)
					{
						uint64_t unit=1UL;
						for(char c:dst)
						{
							if(c=='\0') break;
							switch(c)
							{
								case 'T':
									unit=1024UL*1024*1024*1024;
									break;
								case 'G':
									unit=1024UL*1024*1024;
									break;
								case 'M':
									unit=1024UL*1024;
									break;
								case 'K':
								case 'k':
									unit=1024UL;
									break;
								default:
									continue;
							}
							break;
						}
						double val=1.;
						sscanf(dst,"%lf",&val);
						fs_thr_=(uint64_t)(val*unit);
					}
				}
				fclose(fp);
			}
			else
			{
				rname_=(char*)malloc(strlen("run")+1);
				strcpy(rname_,"run");
				path_=(char*)malloc(strlen("/tmp")+1);
				strcpy(path_,"/tmp");
			}
			fprintf(stderr,"\tinitial file settings\n");
			fprintf(stderr,"\tpath       = %s\n",path_);
			fprintf(stderr,"\trunname    = %s\n",rname_);
			fprintf(stderr,"\trun number = %d\n",rnum_+1);
			if(fs_thr_==0)
			{
				fprintf(stderr,"\tfile unit  = Infty\n");
			}
			else
			{
				double v=fs_thr_;
				uint8_t c=0;
				for(;;++c)
				{
					if((v/1024.) < 1.) break;
					v=v/1024.;
				}
				char u=" KMGT"[c];
				fprintf(stderr,"\tfile unit  = %.2lf %cByte (%luByte)\n",v,u,fs_thr_);
			}
		}
		~okina()
		{
			free(rname_);
			free(path_);
		}
		//void record(unsigned char* flag)
		void record(std::atomic_uchar* flag)
		{
			record(flag,"127.0.0.1");
		}
		//void record(unsigned char* flag,const char* ip)
		void record(std::atomic_uchar* flag,const char* ip)
		{
			fprintf(stderr,"[okina::record]: initializing okina\n");
			int dst_(0);
			while(dst_<=0)
			{
				folklore::make_connection<folklore::karma::takeyabu()>().client(ip,dst_);
				usleep(1000);
			}
			std::thread okina_th([&,flag,dst_](){
				FILE* fp;
				int dst=dst_;
				fprintf(stderr,"[okina::record]: connected for kaguya [%d]\n",dst);
				char fname[128];
				char* lbuf=(char*)malloc(1024*1024*128);
				fprintf(stderr,"[okina::record]: start recording...\n");
				sprintf(fname,"%s/%s%04d.rdf",path_,rname_,rnum_+1);
				fprintf(stderr,"[okina::record]: next run=%s\n",fname);
				uint32_t file_counter(0);
				for(;;)
				{
					bool overflowed{false};
					if((flag[0]==folklore::karma::start_ok())||(flag[0]==folklore::karma::nssta_ok()))
					{
						flag[2]=1;
						auto make_filename=[&](){
							if(file_counter==0)
							{
								sprintf(fname,"%s/%s%04d.rdf",path_,rname_,rnum_);
							}
							else
							{
								sprintf(fname,"%s/%s%04d_%03d.rdf",path_,rname_,rnum_,file_counter);
							}
						};
						if(flag[0]==folklore::karma::start_ok())
						{
							if(file_counter==0)
							{
								fp=fopen(".runnumber","w");
								fscanf(fp,"%d",&rnum_);
								fclose(fp);
								++rnum_;
								fp=fopen(".runnumber","w");
								fprintf(fp,"%d",rnum_);
								fclose(fp);
							}
							make_filename();
						}
						else
						{
							strcpy(fname,"/tmp/shm");
						}
						int32_t preset{0};
						memcpy(&preset,flag+4,sizeof(preset));
						if(preset!=0)
						{
							fprintf(stderr,"[okina::record]: Preset time = %ds\n",preset);
						}
						else
						{
							fprintf(stderr,"[okina::record]: Preset time = Infty\n");
						}
						fprintf(stderr,"[okina::record]: start signal reached. filename=%s\n",fname);
						fp=fopen(fname,"wb");
						if(!fp)
						{
							fprintf(stderr,"[okina::record]: File path \"%s\" maybe invalid.\n",path_);
							fprintf(stderr,"[okina::record]: run begins by using backup disk space.(\"/tmp\")\n");
							fprintf(stderr,"[okina::record]: Please take care about\n");
							fprintf(stderr,"[okina::record]:   1. disk free space\n");
							fprintf(stderr,"[okina::record]:   2. file override\n");
							free(path_);
							path_=(char*)malloc(strlen("/tmp")+1);
							strcpy(path_,"/tmp");
							make_filename();
							fp=fopen(fname,"wb");
							if(!fp)
							{
								fprintf(stderr,"\n");
								fprintf(stderr,"*******************************\n");
								fprintf(stderr,"*                             *\n");
								fprintf(stderr,"* OKINA FATAL ERROR           *\n");
								fprintf(stderr,"* CANNOT CREATE OUTPUT FILE   *\n");
								fprintf(stderr,"* PLEASE CHECK:               *\n");
								fprintf(stderr,"*       FILE PATH             *\n");
								fprintf(stderr,"*       DIRECTORY PERMISSION  *\n");
								fprintf(stderr,"*       DISC FREE SPACE       *\n");
								fprintf(stderr,"* AND, RESTART TERRA          *\n");
								fprintf(stderr,"*                             *\n");
								fprintf(stderr,"*******************************\n");
								fprintf(stderr,"\n");
								return;
							}
						}
						{
							FILE* ffp=fopen("/tmp/okina","w");
							fprintf(ffp,"%s",fname);
							fclose(ffp);
						}
						char fheader[512];
						struct tm *st;
						time_t stat;
						time_t stot;
						stat=time(nullptr);
						st=localtime(&stat);
						char time_str[256];
						auto make_fileheader=[&](const char* times){
							const int32_t fhheader=0xf0000000+512;
							memset(fheader,0,512);
							memset(time_str,0,256);
							sprintf(time_str,"%s time=%4d/%02d/%02d %02d:%02d:%02d  ",times,st->tm_year+1900,st->tm_mon+1,st->tm_mday,st->tm_hour,st->tm_min,st->tm_sec);
							memcpy(fheader,&fhheader,sizeof(int32_t));
							memcpy(flag+64 ,time_str,strlen(time_str));
							memcpy(flag+128,fname,strlen(fname));
							memcpy(fheader+64,flag+64,512-64);
						};
						make_fileheader("start");
						fprintf(stderr,"[okina::record]: %s\n",time_str);
						fwrite(fheader,512,1,fp);
						uint32_t blk{0};
						uint64_t  fs{0};
						uint32_t   s{0};
						auto receiver=[&]()->int32_t{
							char fl=1;
							int r=send(dst,&fl,sizeof(char),MSG_NOSIGNAL);
							r=recv(dst,&s,sizeof(int),MSG_WAITALL);
							if(r==sizeof(int))
							{
								if(s==0) return 0;
								else     r=recv(dst,lbuf,s,MSG_WAITALL);
							}
							if(r<=0)
							{
								return -1;
							}
							fwrite(lbuf,s,1,fp);
							++blk;
							fs+=s;
							return s;
						};
						flag[2]=0;
						while(((flag[0]&0x01)&((flag[0]>>7)&0x01))==1)
						{
							if(receiver()<0) break;
							if(flag[0]==folklore::karma::start_ok() && preset==0)
							if(fs_thr_!=0&&fs>fs_thr_)
							{
								flag[0]=folklore::karma::stop();
								overflowed=true;
								while(flag[0]!=folklore::karma::stop_ok()) usleep(10000);
								flag[2]=1;
								break;
							}
							if(preset!=0)
							{
								stot=time(nullptr);
								if((stot-stat)>preset)
								{
									fprintf(stderr,"\n[okina::record]: preset time %ds elapsed\n",preset);
									flag[0]=folklore::karma::stop();
									while(flag[0]!=folklore::karma::stop_ok()) usleep(10000);
									flag[2]=1;
									break;
								}
							}
						}
						usleep(1000);
						receiver();
						stot=time(nullptr);
						st=localtime(&stot);
						make_fileheader("stop");
						fwrite(fheader,512,1,fp);
						fclose(fp);
						double ela=difftime(stot,stat);
						auto print_ender=[&](FILE* fd_){
							fprintf(fd_,"[okina::record]: %s\n",time_str);
							fprintf(fd_,"[okina::record]: write data to %s\n",fname);
							fprintf(fd_,"[okina::record]: file size     = %.2lfMB\n",fs/1024./1024.);
							fprintf(fd_,"[okina::record]: total block   = %dblocks\n",blk);
							fprintf(fd_,"[okina::record]: total time    = %ds\n",(int)ela);
							fprintf(fd_,"[okina::record]: transfer rate = %.2lfKB/s\n",fs/ela/1024);
							fprintf(fd_,"[okina::record]: trigger rate  = %.2lf blocks/s\n\n",blk/ela);
							if(overflowed)
							{
								fprintf(fd_,"[okina::record]: file size reached at limit value.\n");
								fprintf(fd_,"[okina::record]: file size = %lu\n",fs);
								fprintf(fd_,"[okina::record]: limit     = %lu\n",fs_thr_);
								fprintf(fd_,"[okina::record]: run continue in the same condition.\n");
							}
						};
						fp=fopen(".terra_log","a");
						print_ender(stderr);
						print_ender(fp);
						fclose(fp);
						if(overflowed)
						{
							++file_counter;
							flag[2]=0;
							flag[0]=folklore::karma::start();
							fprintf(stderr,"[okina::record]: try to start next run\n\n");
							fprintf(stderr,"[okina::record]: next run started\n");
						}
						else
						{
							file_counter=0;
						}
					}
					else if(flag[1]==1)
					{
						break;
					}
					else
					{
						usleep(1000);
					}
					flag[2]=0;
				}
				fprintf(stderr,"[okina]: shutdown signal reached\n");
				if(lbuf) free(lbuf);
				close(dst);
			});
			okina_th.detach();
		}
	};
}
