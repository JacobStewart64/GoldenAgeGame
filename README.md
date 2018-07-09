# GoldenAgeGame
An MMO made with Irrlicht, cpp-httplib, OpenSSL, and ENet in Visual Studio 2017

# Everything you need except Irrlicht and OpenSSL comes setup already!

# External code comes from...
- http://irrlicht.sourceforge.net/
  - I built version 1.9.0 (Build 9200) [btw I'm running a NVIDIA GeForce GTX 750 Ti to run and test my game with]
  - You might need DirectX 10 SDK June 10 release. You can get this from microsoft website. You may have to configure your project paths so the compiler can find the headers and libs.
  - get the latest irrlicht source from github
  - navigate source->Irrlicht->Irrlicht15.0.vcxproj.sln (it's deep in the directory)
  - open it, retarget solution, configure to your liking, and build it. I built the static link library.
- https://github.com/yhirose/cpp-httplib
  - I got the latest master at the time.
- http://enet.bespin.org/index.html
  - The most recent stable release (1.3.13)
- https://www.openssl.org/
  - openssl-1.1.1-pre8
  - download it from openssl website
  - download activeperl for windows (from perl website?)
  - download (?and build?) NASM and set your PATH variable
  - open x64 Native Tools Command Prompt for VS 2017 [IN ADMINISTRATOR MODE SO YOU CAN MAKE INSTALL]
  - cd to openssl unzipped directory
  - (this next part might not be quite right, I'll try to fix it, just run Configure, maybe perl Configure, and it will tell you the available options. Pick your target.)
  - run Configure WinIA64 (something like that option lol)
  - run nmake
  - run nmake test
  - run nmake install (if tests were successful)

# Next task: THOROUGHLY COMPLETE DESIGN OF PLAYER DATA AND 99% FINISH LOGIN INCLUDING SPAWNING INTO WORLD

# KEWL THINGS
- the cryptinfo class is ballin. soon it will completely encapsulate the use of libcrypto in my 3 projects with a few modifications. I will make a seperate repo just for it. Super easy to work with cbc encryption through its interface. OpenSSL needs to work on its interface I believe lol.
- the debug function and macro are great. I will make a brother for it that doesn't do an endl at the end soon.

# THINGS TO WORK ON
- There are a few places yet where I'm repeating code. Try to keep it high and tight, productivity goes up.
- Threads... how do u do those lol
- There is a hard to reproduce bug in it somewhere, maybe more than one lol.

*note - you will want to replace the stuff in any Auth folder, as it is compromised due to it being publicly available on Github lol. I will put instructions someday. I want this to be a well documented project that can take n00bs on an epic journey lol.

If you have any advice or criticism to give or you want to help, or make a request, open an issue please!

# LICENSE
- I'm not 100% sure if my license is legit - but it's the GNU AFFERO GENERAL PUBLIC LICENSE (Version 3, 19 November 2007). You can use this in a commercial project but you must make available all of your code (you must open-source your project as well). With the level of MMO demo I will be creating in the end here, if people rip it off and get big, at least they have to provide all their sick well engineered code created with all their resources as open source lol.
