Megasource is a CMake-buildable collection of all [LÖVE][love2d] dependencies.

It is currently only officially supported on Windows, but may also work on macOS. It could certainly also work on Linux, but good package managers makes megasource less relevant there.

Prerequisites
-------------

 - Windows.
 - LOVE 12+: [Visual Studio 2019][vs2019].
 - LOVE 11 and older: [Visual Studio 2013, 2015, or 2017][oldvs]. Official builds of LOVE 11 and older use Visual Studio 2013.
 - [CMake 3.1][cmake] or greater.
 - [NSIS][nsis] if you want to build the LÖVE installer.

Building
--------

To build just the dependencies:

	$ hg clone https://bitbucket.org/rude/megasource megasource
	$ cd megasource
	$ cmake -G "Visual Studio 16 2019" -A Win32 -H. -Bbuild
	$ cmake --build build --target megatest --config Release

... or you can open the solution file in Visual Studio and build from there instead of invoking cmake --build.

If you want to build [LÖVE][love2d], clone LÖVE into the *libs* folder. Megasource will automatically look for it there. E.g.:

	$ hg clone https://bitbucket.org/rude/megasource megasource
	$ cd megasource
	$ hg clone https://bitbucket.org/rude/love libs/love
	$ cmake -G "Visual Studio 16 2019" -A Win32 -H. -Bbuild
	$ cmake --build build --target love/love --config Release

The binaries can be found in *build/love/Release*.

If you want to build the installer, replace the previous build command with:

    $ cmake --build build --target PACKAGE --config Release

A zip with the binaries and the installer will appear in *build*.

[love2d]: http://love2d.org
[cmake]: http://www.cmake.org/
[nsis]: http://nsis.sourceforge.net
[vs2019]: https://visualstudio.microsoft.com/downloads/
[oldvs]: https://visualstudio.microsoft.com/vs/older-downloads/
