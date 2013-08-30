gpio-poweroff-module
====================

This is an small kernel driver to handle power off sequence in an embedded device, 
it will toggle a GPIO output pin at power off, 

It can be used to switch off power a power supply hardware just
before the kernel goes to halt.

"poweroff" command line will trigger this after goint in the right 
init state.


