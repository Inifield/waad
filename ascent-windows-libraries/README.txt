Ascent Win32/64 Libraries Distribution
======================================

Installation Instructions:
Navigate to the directory of your chosen compiler (adjust pathes if necessary):

You need to compile the zlib and PCRE libraries.
Depending on your compiler, open the solution under Sources:
  
  VS 2003: VC71-solution.sln
  VS 2005: VC80-solution.sln
  VS 2008: VC90-solution.sln

Depending on the architecture you want to build for, build the configuration:

  Release/Win32 if you are under Win32,
  Release/x64 if you are under Win64.

The two DLL files provided with this distribution should go in your binary folder, OR
under C:\Windows\System32.

This only has to be done once per compiler!
When you update ascent, this doesn't need to be done each time.

Now you can build Ascent as normal.