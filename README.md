# Forlyns Quest #

This is an example C++ application using the SDL library.
It tries to be as nicely C++11 as possible but do keep in
mind that SDL is written in C and, at some stage, you have
to interface with it.

## Story ##
You are Forlyn a man skilled at many things such as Farming,
drinking... and getting into trouble. When Forlyn was out 
gathering mushrooms for his evening stew he was attacked by a 
pack of hungry wolves, who chased him into a gaping chasm. When
he awoke he found himself within the deepest depths of the undead
caverns. can you help Forlyn escape or are his brains doomed to
be devoured.

## Installation ##
You will have to have the SDL development libraries installed on
your system.  The easiest way to compile is to use a command-line

`$ g++ -c -std=c++0x src/*.cpp -I /usr/include/sigc++-2.0/`
`$ g++ -o ForlynsQuest *.o -lSDL_image`

which will produce an executable file called "ForlynsQuest" in the
top-level directory.  To execute this file do the following

`$ ./ForlynsQuest`
 
from the top-level directory.  The game will expect to find the
`assets` directory under its current working directory.

## Credits ##
The sprites in this game were created by me using paint and gimp 2.0

The event system is based on [Libsigc++](http://libsigc.sourceforge.net/)
and is used under the [LGPL](http://www.gnu.org/copyleft/lgpl.html).
