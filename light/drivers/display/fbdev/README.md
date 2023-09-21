# drivers/display/fbdev

fbdev is a driver for the linear frame buffer. It has adopted the name fbdev from the linux kernel (which none of the code is from).

It currently contains some OS specific code and I plan to move that to the OS soon, namely window.c, cursor.c.
