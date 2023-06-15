
/* Copyright 2021,2023 Jon Dybeck
 * Distributed under the MIT license. */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/Xproto.h>
#include <X11/extensions/XTest.h>

static void handle_KeyPress (XEvent *eventp) {
  XKeyEvent* keyeventp = (XKeyEvent*) eventp;
  KeySym keysym;
  KeyCode keycode = keyeventp->keycode;
  fprintf(stderr, "KeyPress   keycode %u\n", keycode);
}

static void handle_KeyRelease (XEvent *eventp) {
  XKeyEvent* keyeventp = (XKeyEvent*) eventp;
  KeySym keysym;
  KeyCode keycode = keyeventp->keycode;
  fprintf(stderr, "KeyRelease keycode %u\n", keycode);
}

static void main_send(void) {
  const char* displayname = ":0";
  Display *display = XOpenDisplay (displayname);
  if (!display) {
    fprintf (stderr, "%s:  unable to open display '%s'\n",
    "xedo", XDisplayName (displayname));
    exit (1);
  }
  int screen = DefaultScreen(display);

  XSizeHints hints;
  hints.width = hints.min_width = 40;
  hints.height = hints.min_height = 40;
  hints.x = hints.y = 0;
  hints.flags = 0;

  XSetWindowAttributes attr;
  attr.event_mask = KeyPressMask | KeyReleaseMask | KeymapStateMask | StructureNotifyMask;
  attr.background_pixel = WhitePixel(display, screen);
  attr.border_pixel = BlackPixel(display, screen);

  unsigned long mask = CWBackPixel | CWBorderPixel | CWEventMask;

  unsigned borderwidth = 1;

  Window window = XCreateWindow (display, RootWindow (display, screen), hints.x, hints.y,
         hints.width, hints.height, borderwidth, 0,
         InputOutput, (Visual *)CopyFromParent,
         mask, &attr);

  XMapWindow (display, window);

  unsigned int done = 0;
  while (!done) {
    XEvent event;
    XNextEvent(display, &event);
    switch (event.type) {
      case KeyPress:
        handle_KeyPress(&event);
        break;
      case KeyRelease:
        handle_KeyRelease(&event);
        break;
      case KeymapNotify:
        break;
      case DestroyNotify:
        fputs("Quit.", stderr);
        done = 1;
        break;
      case CirculateNotify:
      case ConfigureNotify:
      case GravityNotify:
      case MapNotify:
      case ReparentNotify:
      case UnmapNotify:
        break;
      default:
        fprintf(stderr, "Unexpected event type: %i\n", event.type);
        done = 1;
        break;
    }
  }

  XCloseDisplay (display);
  exit(EXIT_SUCCESS);
}

static void main_recv(void) {
  const char* displayname = ":0";
  Display *display = XOpenDisplay (displayname);
  if (!display) {
    fprintf (stderr, "%s:  unable to open display '%s'\n",
    "xedo", XDisplayName (displayname));
    exit (1);
  }
  int screen = DefaultScreen(display);

  unsigned int done = 0;
  while (!done) {
    KeyCode keycode = 40; /* TODO Read keycode. */
    int is_press = 1;
    int type = KeyPress;

    unsigned u;

    scanf("%u", &u);
    keycode = u;

    XTestFakeKeyEvent(display, keycode, is_press, CurrentTime);
    XSync(display, False);
    XFlush(display);
  }

  XCloseDisplay(display);
  exit(EXIT_SUCCESS);
}

int
main (int argc, char *argv[]) {

  if (argc == 2) {
    if (strcmp(argv[1], "send") == 0) {
      main_send();
    } else if (strcmp(argv[1], "recv") == 0) {
      main_recv();
    } else {
      exit(EXIT_FAILURE);
    }
  } else {
    exit(EXIT_FAILURE);
  }

  return 0;
}
