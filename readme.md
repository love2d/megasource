Megasource is a CMake-buildable collection of all [LÖVE][love2d] dependencies.

It is currently only officially supported on Windows, but may also work on macOS. It could certainly also work on Linux, but good package managers makes megasource less relevant there.

Prerequisites
-------------

 - Windows.
 - [Visual Studio 2019][vs2019] or greater.
 - [CMake 3.1][cmake] or greater.
 - [NSIS][nsis] if you want to build the LÖVE installer.

Building
--------

To build just the dependencies:

	$ git clone https://github.com/love2d/megasource megasource
	$ cd megasource
	$ cmake -H. -Bbuild
	$ cmake --build build --target megatest --config Release

... or you can open the solution file in Visual Studio and build from there instead of invoking cmake --build.

Note, that ```-H``` had been replaced in CMake 3.13 with ```-S```.

If you want to build [LÖVE][love2d], clone LÖVE into the *libs* folder. Megasource will automatically look for it there. E.g.:

	$ git clone https://github.com/love2d/megasource megasource
	$ cd megasource
	$ git clone https://github.com/love2d/love libs/love
	$ cmake -H. -Bbuild
	$ cmake --build build --target love/love --config Release

The binaries can be found in *build/love/Release*.

If you want to build the installer, replace the previous build command with:

    $ cmake --build build --target PACKAGE --config Release

A zip with the binaries and the installer will appear in *build*.

[love2d]: http://love2d.org
[cmake]: http://www.cmake.org/
[nsis]: http://nsis.sourceforge.net
[vs2019]: https://visualstudio.microsoft.com/downloads/
