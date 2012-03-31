2012-03-30:
Adding this directory structure to keep track of the various binary releases that get uploaded to the server. The structure is:

<versionNumber1>
  |-> <Win32>
  |-> <Linux32>
  |-> <Linux64>
<versionNumber2>
  |-> <Win32>
  |-> <Linux32>
  |-> <Linux64>
...

VersionNumber is simply the SVN revision of that build.

Please note that the number in the directory refers to the version number that 