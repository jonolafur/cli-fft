2016-12-27:
Adding this directory structure to keep track of the various binary releases that get uploaded to the server. The structure is:

<versionNumber1>
  |-> <Win32>
  |-> <Win64>
  |-> <Linux32>
  |-> <Linux64>
<versionNumber2>
  |-> <Win32>
  |-> <Win64>
  |-> <Linux32>
  |-> <Linux64>
...

VersionNumber is simply the SVN revision of that build.

There seems to some ambiguity of how to name windows builds. Sometimes things are called win32 although they are 64 bit binaries. Here Win32 refers to 32 bit binaries and Win64 to 64 bit binaries.

Please note that the number in the directory refers to the svn revision number.