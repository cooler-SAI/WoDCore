## Overview
Firestorm WoD Leak World of Warcraft Warlords of Draenor (6.2.3.20779) emulator based on TrinityCore project, JadeCore (5.0.5), Garrosh (5.4.X)

### BEWARE ! 
Never commit into interrealm only crash fix, wod branch is the prod branch !

## Requirements

+ Platform: Linux, Windows or Mac
+ Git ≥ 1.7+ https://sourceforge.net/projects/gitextensions/
+ ACE ≥ 6.2.0
+ MySQL ≥ 5.5.0+  http://dev.mysql.com/downloads/mysql/5.6.html
+ SQLyog https://github.com/webyog/sqlyog-community/wiki/Downloads
+ CMake ≥ 3.2+ https://cmake.org/download/
+ OpenSSL ≥ 1.0.x http://www.slproweb.com/products/Win32OpenSSL.html
Find the 64bit version by finding the latest 1.0.x Win64 OpenSSL that is NOT the "light" version. NOT 1.1.x Version.
Example: Win64 OpenSSL v1.0.2k
Find the 32bit version by finding the latest 1.0.x Win32 OpenSSL that is NOT the "light" version. NOT 1.1.x Version.
Example: Win32 OpenSSL v1.0.2k
+ GCC ≥ 4.7.2 (Linux only)
+ MS Visual Studio ≥ 14 (2017) (Windows only x64) https://www.visualstudio.com/thank-you-downloading-visual-studio/?sku=Community&rel=15
+ boost 1.64.0 64bit: https://sourceforge.net/projects/boost/files/boost-binaries/1.64.0/boost_1_64_0-msvc-14.1-64.exe/download
               32bit: https://sourceforge.net/projects/boost/files/boost-binaries/1.64.0/boost_1_64_0-msvc-14.1-32.exe/download
+ Xcode 7 (Mac only)
+ P>S> ONLY FOR LOCAL PLAY DOWNLOAD Microsoft Visual C++ 2015 Redistributable Package 
https://www.microsoft.com/en-us/download/details.aspx?id=48145
Detailed installation guides are available in the wiki for
[Windows](http://collab.kpsn.org/display/tc/Win),
[Linux](http://collab.kpsn.org/display/tc/Linux) and
[Mac OSX](http://collab.kpsn.org/display/tc/Mac).

In order to complete the installation of the core, you need some other dependencies, they are:
- LibCurl, under linux its the package libcurl4-openssl-dev, under windows you may have to compile it your self,
- EasyJson headers, for that you only need to execute in your git repo :

```
git submodule init && git submodule update --remote
```

- If you have the message `Git was NOT FOUND on your system` please uninstall and update your git version (you can also change the path for the binary on yout PATH)
- **NEW** Windows user you need the mysql server installed, download the last version (5.7+) here : [MySQL Community Server](http://dev.mysql.com/downloads/mysql/) if you need the x86 build just change the include path on cmake
- **NEW** If you use MSVC 2013 you *MUST* have the update 3+

## Commit naming convention
- When fixing bug, by all means include the issue number.
- When completing feature if you have some task identifier, include that.
- If the identifier included goes with subject, it often makes sense to just reuse it. (**Use Core/XXX: "Commit name" format**)
- If the project is large, mention which component is affected.
- **Describe the change in functionality** , **never** the code (that can be seen from the diff).

####How to Cppcheck
run ```cppcheck --enable=all --xml-version=2 > PathToFile/cppcheck.xml --inconclusive path PathToFile/wod/src/ --verbose```

and run ```./cppcheck-htmlreport --file=PathToFile/cppcheck.xml --source-dir=PathToFile/wod/src --report-dir=PathToFile/wod/cppcheck/``` to get the html out
