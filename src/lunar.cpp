#include <lunar.hpp>
int main(int argc,char* argv[])
{
	folklore::lunar::lunar lunar(argc==2?argv[1]:"127.0.0.1");
	folklore::lunar::print_title(stderr);
	typedef folklore::lunar::dummy_eclipse_t ec_t;
	ec_t* e1 = new ec_t();
	lunar.observe(e1);
	lunar.console();
	delete e1;
	return 0;
}
