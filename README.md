Build
=====

Copy the included C filess to your f1rmware/l0adables folder.

    cp *.c f1rmware/l0dables

Then add an entry for the new c1d's in f1rmware/l0adables/Makefile.
Your new line should look like this:

    C1D=blinky.c1d invaders.c1d mandel.c1d snake.c1d bricks.c1d schedule.c1d plot.c1d

The build as normal.


Use
===

plot
----

For best performance & stability, start using the testapp:

Set the CPU speed to max in the speed submenu, and run using execute.
The CPU might get hot, so use with care & <b>don't burn your finger</b>

Left changes from FFT to Amplitude plot and vice versa.

By default, the headset ADC input is used, but this is easily changed in plot.c on line 88.
Try channel 1 for some fun with GND.

echo
----
This example just ciopys the value from the ADC to the DAC.


Todo
====

 * Multichannel support
 * Figure out why plot crashes when loaded via camp app.
