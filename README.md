How-To Use
==========

Copy the included C filess to your f1rmware/l0adables folder.

    cp *.c f1rmware/l0dables

Then add an entry for the new c1d's in f1rmware/l0adables/Makefile.
Your new line should look like this:

    C1D=blinky.c1d invaders.c1d mandel.c1d snake.c1d bricks.c1d schedule.c1d echo.c1d

The build as normal.


Todo
====

 * Better channel support
 * Display multiple channels at once
 * Use the avg of multiple samples for one value, to increase accuracy.
