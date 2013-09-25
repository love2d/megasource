
#include <vector>
#include <iostream>
#include <sstream>
#include <functional>

#include <zlib.h>
#include <physfs.h>
#include <png.h>
#include <jpeglib.h>
#include <ogg/ogg.h>
#include <vorbis/codec.h>
#include <vorbis/vorbisfile.h>
#include "mpg123.h"

extern "C" {
#	include "lua.h"
}

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

	vfunc lua = [](strs &c, strs &l)
	{
		std::string compiled = LUA_RELEASE;

		compiled = compiled.substr(4);

		c << compiled;
		l << "N/A";
		return "Lua";
	};

	vfunc png = [](strs &c, strs &l)
	{
		c << PNG_LIBPNG_VER_STRING;
		l << png_libpng_ver;
		return "libpng";
	};

	vfunc jpeg = [](strs &c, strs &l)
	{
		c << (int)JPEG_LIB_VERSION;
		l << "N/A";
		return "jpeg";
	};

	vfunc ogg = [](strs &c, strs &l)
	{
		c << "N/A";
		l << "N/A";
		return "ogg";
	};

	vfunc vorbis = [](strs &c, strs &l)
	{
		c << "N/A";
		l << "N/A";
		return "vorbis";
	};

	vfunc vorbisfile = [](strs &c, strs &l)
	{
		c << "N/A";
		l << "N/A";
		return "vorbisfile";
	};

	vfunc mpg123 = [](strs &c, strs &l)
	{
		mpg123_init();
		c << "N/A";
		l << "N/A";
		return "mpg123";
	};

	std::vector<vfunc> funcs;
	funcs.push_back(zlib);
	funcs.push_back(physfs);
	funcs.push_back(lua);
	funcs.push_back(png);
	funcs.push_back(jpeg);
	funcs.push_back(ogg);
	funcs.push_back(vorbis);
	funcs.push_back(vorbisfile);
	funcs.push_back(mpg123);

	for (size_t i = 0; i < funcs.size(); ++i)
	{
		vfunc f = funcs[i];
		strs c, l;
		std::string name = f(c, l);
		std::cout << "-- " << pad(name) << "   compiled: " << pad(c.str(), 7) << "   linked: " << pad(l.str(), 7) << std::endl;
	}

	return getchar();
}
