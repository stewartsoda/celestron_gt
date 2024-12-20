/*
Author and Copyright of this file and of the stellarium telescope library:
Johannes Gajdosik, 2006
*/
Stellarium Telescope Protocol version 1.0
The Stellarium Telescope Protocol works on top of TCP/IP.
The client is the stellarium program (or any similar program),
the server is called "telescope server" and controls the telescope.
Depending on the implementation the server may handle one or many
simultaneous clients. The reference server implementation accepts
an unlimited number of simultaneous clients.
The protocol is message based: both server and client may
spontaneousely send messages as often as they want.
When the server looses control of the telescope he should
actively close the connection to all clients.
The first two bytes of any message describes the total
length of that message in bytes (including the first 2 bytes).
The next 2 bytes of any message describe the type of this message.
The byte order for all kind of integers is always
"least significiant byte first".
-----------------------
server->client:
MessageCurrentPosition (type = 0):
LENGTH (2 bytes,integer): length of the message
TYPE (2 bytes,integer): 0
TIME (8 bytes,integer): current time on the server computer in microseconds
since 1970.01.01 UT. Currently unused.
RA (4 bytes,unsigned integer): right ascension of the telescope (J2000)
a value of 0x100000000 = 0x0 means 24h=0h,
a value of 0x80000000 means 12h
DEC (4 bytes,signed integer): declination of the telescope (J2000)
a value of -0x40000000 means -90degrees,
a value of 0x0 means 0degrees,
a value of 0x40000000 means 90degrees
STATUS (4 bytes,signed integer): status of the telescope, currently unused.
status=0 means ok, status<0 means some error
---------------------
client->server:
MessageGoto (type =0)
LENGTH (2 bytes,integer): length of the message
TYPE (2 bytes,integer): 0
TIME (8 bytes,integer): current time on the client computer in microseconds
since 1970.01.01 UT. Currently unused.
RA (4 bytes,unsigned integer): right ascension of the telescope (J2000)
a value of 0x100000000 = 0x0 means 24h=0h,
a value of 0x80000000 means 12h
DEC (4 bytes,signed integer): declination of the telescope (J2000)
a value of -0x40000000 means -90degrees,
a value of 0x0 means 0degrees,
a value of 0x40000000 means 90degrees
