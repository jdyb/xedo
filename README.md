
_WIP: This is tool is not finished yet._

# XeDo: Forward keyboard input from one computer to another

This is a ugly work-in-progress hack, `xedo send` and `xedo recv`.

The send command opens window similar to xev in one Xorg display and prints
any keyboard events to stdout. The recv command reads these events from stdin
and injects them to a Xorg display.

So by running the send and recv commands on different computers with a ssh
pipe between them one can control the remote display via keyboard.

# Build instructions

Tested on Ubuntu 20.04 and CentOS 7, probably works on other distros.
Compile it this way:

```bash
cc $(pkg-config x11 xtst --cflags) -o xedo xedo.c $(pkg-config x11 xtst --libs)
```

# Bugs? What bugs?

Ah, yes, there probably are a few ~~bugs~~ features in here!

To start with, this tool does not attempt to deal with keyboard layouts at all.

Also, do not start both the sender and receiver with the same X11 display,
it will loop the keyboard events.

# License (MIT)

Please see ./LICENSE for the full text.

# Xev and xdotool

This tool is inspired by the _Xev_ and _xdotool_. _Xev_ is a tool for
printing information about X11 events (such as keyboard typing). _xdotool_
is a command line tool for automating actions on X11 windows, such as typing
something. Check them out!

