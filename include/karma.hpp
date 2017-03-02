#pragma once
//#include <atomic>
namespace folklore_impl
{
	constexpr unsigned char set_bit(int b7,int b6,int b5,int b4,int b3,int b2,int b1,int b0)
	{
		return b0|b1<<1|b2<<2|b3<<3|b4<<4|b5<<5|b6<<6|b7<<7;
	}
}
namespace folklore{ namespace karma
{
	namespace port
	{
		static const int mt_fuji  = 39876;
		static const int takeyabu = 39888;
		static const int fullmoon = 39500;
		static const int newmoon  = 39501;
	}
	constexpr int mt_fuji(void)   { return port::mt_fuji;  }
	constexpr int takeyabu(void)  { return port::takeyabu; }
	constexpr int fullmoon(void)  { return port::fullmoon; }
	constexpr int newmoon(void)   { return port::newmoon;  }
	namespace aux_
	{
		constexpr int tsukuyomi_size(void) { return 512; }
	}
//
	namespace daq
	{
		static const unsigned char reload    = folklore_impl::set_bit(0,1,0,0,0,0,0,0);
		static const unsigned char reload_ok = folklore_impl::set_bit(1,1,0,0,0,0,0,0);
		static const unsigned char start     = folklore_impl::set_bit(0,0,0,0,0,0,1,1);
		static const unsigned char start_ok  = folklore_impl::set_bit(1,0,0,0,0,0,1,1);
		static const unsigned char nssta     = folklore_impl::set_bit(0,0,0,0,0,0,0,1);
		static const unsigned char nssta_ok  = folklore_impl::set_bit(1,0,0,0,0,0,0,1);
		static const unsigned char stop      = folklore_impl::set_bit(0,0,0,0,0,1,0,0);
		static const unsigned char stop_ok   = folklore_impl::set_bit(1,0,0,0,0,1,0,0);
	}
	constexpr unsigned char reload(void)    { return daq::reload;    }
	constexpr unsigned char start(void)     { return daq::start;     }
	constexpr unsigned char nssta(void)     { return daq::nssta;     }
	constexpr unsigned char stop(void)      { return daq::stop;      }
	constexpr unsigned char reload_ok(void) { return daq::reload_ok; }
	constexpr unsigned char start_ok(void)  { return daq::start_ok;  }
	constexpr unsigned char nssta_ok(void)  { return daq::nssta_ok;  }
 	constexpr unsigned char stop_ok(void)   { return daq::stop_ok;   }
// to be expired
	constexpr int count_accepted_trigers(void)      { return 0; }
	constexpr int count_all_trigers(void)           { return 8; }
	constexpr int register_controled_run_mode(void) { return 0; }
	constexpr int s_in_controled_run_mode(void)     { return 1; }
	constexpr int s_in_gate_mode(void)              { return 2; }
	constexpr int multi_board_sync_mode(void)       { return 3; }
//
	namespace BID
	{
		constexpr uint64_t v1724_dpp_pha(void){ return 1;  }
		constexpr uint64_t v1720_dpp_psd(void){ return 2;  }
		constexpr uint64_t v1720_dpp_ci (void){ return 3;  }
		constexpr uint64_t caen_GPIO    (void){ return 6;  }
	}
	template <uint64_t ID,int Size_=4>
	class header_base_t
	{
	private:
		static const uint64_t BID_bits=(ID<<58);
		int32_t val[Size_];
	public:
		constexpr size_t size() const
		{
			return sizeof(int32_t)*Size_;
		}
		header_base_t()
		{
			val[0]=0x50000000;
			memcpy(val+2,&BID_bits,sizeof(uint64_t));
		}
		constexpr int BID(void) const{ return ID; }
		void copy(void* buf)
		{
			memcpy(buf,val,size());
		}
		void set_size(uint32_t siz)
		{
			val[0]=0x50000000u+siz;
		}
		uint32_t get_size(void) const
		{
			return val[0]&0x0fffffffu;
		}
		void set_uid(int32_t uid)
		{
			val[1]=uid;
		}
		void set_time(uint64_t tim)
		{
			uint64_t t(tim);
			t|=BID_bits;
			memcpy(val+2,&t,sizeof(uint64_t));
		}
	};
// for TERRA
	namespace kaguya_param
	{
		static const int kaguya_size = 1024*1024*512;
		static const int kaguya_thr  = 1024*1024*(512-128);
	}
	constexpr int kaguya_size(void) { return kaguya_param::kaguya_size; }
	constexpr int kaguya_thr (void) { return kaguya_param::kaguya_thr;  }
}}
