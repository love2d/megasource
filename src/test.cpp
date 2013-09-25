
#include <iostream>
#include <zlib.h>

int main(int argc, const char **argv)
{
	const char *prefix = " -- ";

	std::cout << prefix << "Starting test" << std::endl;
	std::cout << prefix << "zlib version: " << zlibVersion() << std::endl;
	std::cout << prefix << "Ending test" << std::endl;

	return 0;
}
