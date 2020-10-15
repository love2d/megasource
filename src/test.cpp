
#include <vector>
#include <iostream>
#include <sstream>
#include <functional>

#include <zlib.h>
#include <ogg/ogg.h>
#include <vorbis/codec.h>
#include <vorbis/vorbisfile.h>
#include <freetype/config/ftconfig.h>
#include <freetype/freetype.h>
#include <SDL.h>
#include <AL/al.h>
#include <AL/alext.h>
#include <modplug.h>

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

int main(int argc, char **argv)
{
	vfunc zlib = [](strs &c, strs &l)
	{
		c << ZLIB_VERSION;
		l << zlibVersion();
		return "zlib";
	};

	vfunc lua = [](strs &c, strs &l)
	{
		std::string compiled = LUA_RELEASE;

		compiled = compiled.substr(4);

		c << compiled;
		l << "N/A";
		return "Lua";
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

	vfunc freetype = [](strs &c, strs &l)
	{
		FT_Library lib;
		FT_Init_FreeType(&lib);
		FT_Int major, minor, patch;
		FT_Library_Version(lib, &major, &minor, &patch);
		c << "N/A";
		l << major << "." << minor << "." << patch;
		return "freetype";
	};

	vfunc SDL2 = [](strs &c, strs &l)
	{
		SDL_version compiled;
		SDL_version linked;

		SDL_VERSION(&compiled);
		SDL_GetVersion(&linked);

		c << (int)compiled.major << "." << (int)compiled.minor << "." << (int)compiled.patch;
		l << (int)linked.major << "." << (int)linked.minor << "." << (int)linked.patch;
		return "SDL2";
	};

	vfunc OpenAL = [](strs &c, strs &l)
	{
		alIsEnabled(AL_SOURCE_DISTANCE_MODEL);

		c << "N/A";
		l << "N/A";
		return "OpenAL";
	};

	vfunc modplug = [](strs &c, strs &l)
	{
		ModPlug_Settings settings;
		ModPlug_GetSettings(&settings);
		c << "N/A";
		l << "N/A";
		return "modplug";
	};

	std::vector<vfunc> funcs;
	funcs.push_back(zlib);
	funcs.push_back(lua);
	funcs.push_back(ogg);
	funcs.push_back(vorbis);
	funcs.push_back(vorbisfile);
	funcs.push_back(freetype);
	funcs.push_back(SDL2);
	funcs.push_back(OpenAL);
	funcs.push_back(modplug);

	for (size_t i = 0; i < funcs.size(); ++i)
	{
		vfunc f = funcs[i];
		strs c, l;
		std::string name = f(c, l);
		std::cout << "-- " << pad(name) << "   compiled: " << pad(c.str(), 7) << "   linked: " << pad(l.str(), 7) << std::endl;
	}

	return getchar();
}
