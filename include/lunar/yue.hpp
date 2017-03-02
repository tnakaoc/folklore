#pragma once
#include <stdio.h>
#include <cstdint>
#include "../console_state.hpp"
namespace folklore{ namespace lunar
{
	void print_title(decltype(stdout) fd)
	{
		folklore::console_state conn;
		conn.device(fd);
		uint8_t rand{0};
		FILE* fp=fopen("/dev/urandom","rb");
		fread(&rand,sizeof(rand),1,fp);
		fclose(fp);
		if(rand>64)
		{
			conn.fg_color("yellow");
			fprintf(fd,"\n");
			fprintf(fd,"      ///     /// ///  //   ///   /////   ///////\n");
			fprintf(fd,"     ///     /// ///  ///  ///  ///  //  ///  ///\n");
			fprintf(fd,"    ///     /// ///  //// ///  ///  /// //////// \n");
			fprintf(fd,"   ///     /// ///  /// ////  //////// /// ///   \n");
			fprintf(fd,"  ///////  /////   ///  ///  ///  /// ///  ///   \n");
			fprintf(fd,"\n");
		}
		else if(rand>16)
		{
			conn.fg_color("yellow");
			fprintf(fd,"\n");
			fprintf(fd,"           ///////////// \n");
			fprintf(fd,"          ///        /// \n");
			fprintf(fd,"         ///        ///  \n");
			fprintf(fd,"        //////////////   \n");
			fprintf(fd,"       ///        ///    \n");
			fprintf(fd,"      ///        ///     \n");
			fprintf(fd,"     //////////////      \n");
			fprintf(fd,"    ///        ///       \n");
			fprintf(fd,"  ///         ///        \n");
			fprintf(fd,"///       /////          \n");
			fprintf(fd,"\n");
		}
		else
		{
			conn.fg_color("green");
			fprintf(fd,"\n");
			fprintf(fd,"       /////////////////            ///        \n");
			fprintf(fd,"         ///      ///      /////////////////// \n");
			fprintf(fd,"       ///       ///      ///  ///  ///   ///  \n");
			fprintf(fd,"    //////////////////       ///     ///       \n");
			fprintf(fd,"           /// ///              ///            \n");
			fprintf(fd,"         ///  ///       ///////////////////    \n");
			fprintf(fd,"      ///    ///              ////             \n");
			fprintf(fd,"    ///     ///             /// ///            \n");
			fprintf(fd,"  ///      ///            ///     ///          \n");
			fprintf(fd,"///    /////            ///        ///         \n");
			fprintf(fd,"\n");
		}
		conn.fg_color("reset");
	}
	void print_title(void)
	{
		print_title(stdout);
	}
}}
