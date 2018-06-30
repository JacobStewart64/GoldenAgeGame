# GoldenAgeGame
An MMO made with irrlicht, cpp-httplib, and ENet in Visual Studio 2017

# Everything you need except Irrlicht and OpenSSL comes setup already!

# External code comes from...
- http://irrlicht.sourceforge.net/
  - I built version 1.9.0 (Build 9200) [btw I'm running a NVIDIA DeForce GTX 750 Ti to run and test my game with]
- https://github.com/yhirose/cpp-httplib
  - I got the latest master at the time, the version included here in the repo has minor modifications that produce error messages when you are missing .pem keys and stuff
- http://enet.bespin.org/index.html
  - The most recent stable release (1.3.13)
- https://www.openssl.org/
  - openssl-1.1.1-pre8

# Trying to get everything going for a good login.
- passwords not stored as plaintext, established key (stored as file on a secure machine).
- client and login server communicating with TLS 1.3(?) HTTPS
- login server and game server encrypt UDP communications over my intranet with an established key.
  - not done!
- client and game server communicate with encrypted UDP, encryption info is established over secure channels via the login server. Hackers could crack a client, and all they would get is the power to look at udp packets in plaintext. It is not critical, there are other ways to figure out the way the game server works anyway. The game server already needs to be aware of that problem.
  - getting close!
- we don't use cloud storage or a db. We don't touch the hard disk rarely but for creating accounts or updating account information every once in a while as people play. Plain files and directories will work great? I'm thinking it's better than cloggin the tubes with requests to something like MongoDB. I guess I'll need RAID lol... It's like owning vs renting, you have to mow your own lawn. It will be fun they said! I need a riding lawn mower lol.
  - I'm a lot farther ahead conceptually than codely lol
    - Give me a month or two, I'm slow lol. The login is the hard part to me. I've already coded an MMO in Irrlicht but it became a dusty old project from the noob days and I deleted it lol. It never had a login, just gameplay, running around together in a room and auto attacking mobs lol. This time I want to stay on top of organization and take it to the next level with a solid demo made out of placeholder assets. Hopefully I can get some support for creation of the game, or I can sell it all as an MMO starter kit lol. Once I get past Login and auto attacking mobs in the starting area I will be making a private repo and leaving this one to help people. I will make a few youtubes of me adding some of the basic features soon to help n00bs, plus my friend wants to see what's up with this coding stuff lol.

# KEWL THINGS
- the cryptinfo class is ballin. soon it will completely encapsulate the use of libcrypto in my 3 projects with a few modifications. I will make a seperate repo just for it. Super easy to work with cbc encryption through its interface. OpenSSL needs to work on its interface I believe lol.
- the debug function and macro are great. I will make a brother for it that doesn't do an endl at the end soon.

# THINGS TO WORK ON
- There are a few places yet where I'm repeating code. Try to keep it high and tight, productivity goes up.
- Threads... how do u do those lol
- I wanna fork my game server and login server!

*note - you will want to replace the stuff in any Auth folder, as it is compromised due to it being publicly available on Github lol. I will put instructions someday. I want this to be a well documented project that can take n00bs on an epic journey lol.

If you have any advice or criticism to give or you want to help, or make a request, open an issue please!