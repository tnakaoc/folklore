#include <terra.hpp>
int main(void)
{
	folklore::terra::terra terra("T E R R A\tversion 2.00");
	folklore::terra::print_title(stderr);
	terra.genesis();
	return 0;
}
