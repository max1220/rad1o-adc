rad1o-stuff
===========

Stuff that might be usefull for owners of a rad1o-badge.


flash.lua
=========

This lua script searches dmesg for the device for the rad1o badge, mounts
it, copys the files in the files table to the mountpoint, unmounts it & then
syncs the filesystem. This is usefull if you don't want to mount it all the
time manually, and don't like the flashgui, like me does. It is quite save,
since it checks the ammount of blocks on the drive. (So if you don't have a
HDD with 3072 512-byte logical blocks, you shouldn't need to worry.)


Dependencys
-----------
Lua 5.1/5.2/5.3
(You'll also need access to the shell tools dmesg, cp, mount, umount and sync)


Installation
------------
Just copy the file somewhere, and edit the file table.


Usage
-----
./flash.lua
(Does not take any arguments)





Build
=====

Copy the included C filess to your f1rmware/l0adables folder.

    cp *.c f1rmware/l0dables

Then add an entry for the new c1d's in f1rmware/l0adables/Makefile.
Your new line should look like this:

    C1D=blinky.c1d invaders.c1d mandel.c1d snake.c1d bricks.c1d schedule.c1d plot.c1d

The build as normal.




plot.c & echo.c
===============

To build these, you need to copy them to the l0dables directory, and modify
your l0dables/Makefile In the first line(C1D=...) add plot.c1d and echo.c1d.


echo.c
------

Copys the value from the DAC to the ADC.


plot.c
------

Displays a plot of either the FFT or the Amplitude plot of the various ADC
channels. Try Channel 0 for some fun with GND!


