# G213Manager
C++ program to manipulate the colors in the 5 areas of the Logitech G213 keyboard and also the breathe and cycle effects.
This is based on the work of SebiTimerWater GitHub user : https://github.com/SebiTimeWaster/G213Colors
but for me the python program change only 3 areas of the keyboard, probably because i dont know to much about python.

G213Manager was built on linux and the only requirement for compile is the libusb library of your distribution, was tested with the version 1.x of the library.

USAGE :
You can run the software without argumentes to see the list of options.
Because the software need to have access to the USB device , run as root.

to run only

g213Manager <options>

on the directory installed, and also need to be executable

Where options are :

-c <color>              Set selected color for 5 areas<br>
-c<n> <color>           Set selected color for n area can be any number between 1 and 5 (i.e -c1)<br>
-x <speed>              Set cycle animation with a speed indicated in milliseconds<br>
-b <color> <speed>      Set breathe animation with the specified color and the speed in milliseconds<br>

* Color is a hex encoded color in the format RRGGBB
  i.e. ff0000 is red, 00ff00 is green and so on
* Speed is in milliseconds, range: 32 - 65535


KNOWN ISSUES :
None i found , but in case your keyboard is blocked unplug and plug again on your USB port.

CHANGELOG : 
Initial Checkin
