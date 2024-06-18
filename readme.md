Megasource is a CMake-buildable collection of all [LÖVE][love2d] dependencies.

It is currently only officially supported on Windows, but may also work on macOS. It could certainly also work on Linux, but good package managers makes megasource less relevant there.

Versions
--------

This repository uses git tags for the library dependencies of a specific released version of LÖVE.

For in-development versions of LÖVE, there may be a branch in this repository corresponding to a similar branch in the love repository.

Prerequisites
-------------

 - Windows.
 - LOVE 12+: [Visual Studio 2019][vs2019] or newer.
 - LOVE 11 and older: [Visual Studio 2013][oldvs] or newer. Official builds of LOVE 11 and older use Visual Studio 2013.
 - [CMake 3.1][cmake] or greater.
 - [NSIS][nsis] if you want to build the LÖVE installer.

Building
--------

To build just the dependencies for 64 bit x86 using Visual Studio 2022:

	$ git clone https://github.com/love2d/megasource megasource
	$ cd megasource
	$ cmake -G "Visual Studio 17 2022" -A x64 -S . -B build
	$ cmake --build build --target megatest --config Release

... or you can open the solution file in Visual Studio and build from there instead of invoking cmake --build.

To build for a different version of Visual Studio (or a different build tool), replace ```"Visual Studio 17 2022"``` with one of the other [build system generators][generators]. For example Visual Studio 2019 is ```"Visual Studio 16 2019"```.

Similarly, to build for a different architecture than 64 bit x86, use a different [architecture name][architectures] for the ```-A``` parameter.

If you want to build [LÖVE][love2d], clone LÖVE into the *libs* folder. Megasource will automatically look for it there. E.g.:

	$ git clone https://github.com/love2d/megasource megasource
	$ cd megasource
	$ git clone https://github.com/love2d/love libs/love
	$ cmake -G "Visual Studio 17 2022" -A x64 -S . -B build
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
[generators]: https://cmake.org/cmake/help/latest/manual/cmake-generators.7.html#visual-studio-generators
[architectures]: https://cmake.org/cmake/help/latest/generator/Visual%20Studio%2016%202019.html#platform-selection
