# GoldenAgeGame
An MMO made with irrlicht, cpp-httplib, and winsock udp sockets in Visual Studio 2017

The output folder is supposed to be configured and set up with the correct folders, and executable is to be ran from the ouput
directory so the working directory doesn't need to be changed.

Output Directories
- Client
- GameServer
- LoginServer 

You'll need httplib and openssl.
      
I'm about to start writing the game server and the part where you choose character at login. I'm still in the very beginning. I will use an https login server to establish encryption and decryption keys to use for udp communcations between the game server and client. They will be negotiated on client login. This provides decent protection against some nuisances.

I will only put up to walking around and fighting mobs in a single area on github.
      
