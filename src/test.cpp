
#include <vector>
#include <iostream>
#include <sstream>
#include <functional>

#include <zlib.h>
#include <physfs.h>

typedef std::stringstream strs;
typedef std::function<std::string (strs &, strs &)> vfunc;

std::string pad(std::string s, size_t size = 16)
{
	while (s.length() < size)
		s.append(" ");

	return s;
}

int main(int argc, const char **argv)
{
	vfunc zlib = [](strs &c, strs &l)
	{
		c << ZLIB_VERSION;
		l << zlibVersion();
		return "zlib";
	};

	vfunc physfs = [](strs &c, strs &l)
	{
		PHYSFS_Version compiled;
		PHYSFS_Version linked;

		PHYSFS_VERSION(&compiled);
		PHYSFS_getLinkedVersion(&linked);

		c << (int)compiled.major << "." << (int)compiled.minor << "." << (int)compiled.patch;
		l << (int)linked.major << "." << (int)linked.minor << "." << (int)linked.patch;
		return "PhysicsFS";
	};

	std::vector<vfunc> funcs;
	funcs.push_back(zlib);
	funcs.push_back(physfs);

	for (size_t i = 0; i < funcs.size(); ++i)
	{
		vfunc f = funcs[i];
		strs c, l;
		std::string name = f(c, l);
		std::cout << "-- " << pad(name) << "   compiled: " << pad(c.str(), 7) << "   linked: " << pad(l.str(), 7) << std::endl;
	}

	return getchar();
}
