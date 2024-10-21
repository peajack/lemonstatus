#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <sys/event.h>
#include <sys/signal.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <machine/apmvar.h>
#include <unistd.h>
#include <time.h>
#include <unistd.h>

#include <X11/XKBlib.h>
#include <X11/extensions/XKBrules.h>
#include <X11/Xatom.h>

#include "lemonstatus.h"
#include "config.h"

Display *dpy;
Window root;

void die(char *call) {
    perror(call);
    exit(1);
}

static const char *xprop(char *name, char *type, Window win, long length) {
    Atom return_type;
    int return_format;
    unsigned long nitems, after;
    static unsigned char *data = NULL;

    Atom name_atom = XInternAtom(dpy, name, 0);
    Atom type_atom = XInternAtom(dpy, type, 0);

    int status = XGetWindowProperty(dpy, win, name_atom, 0, length,
                                    0, type_atom, &return_type,
                                    &return_format, &nitems,
                                    &after, &data);
    if (status != Success) {
        return NULL;
    }

    return (const char *)data;
}

Window current_window(void) {
    const char *data = xprop("_NET_ACTIVE_WINDOW", "WINDOW",
                             root, 1);
    Window win = ((Window *)data)[0];
    return win;
}

static const char *get_window_name(Window win) {
    if (win == 0) {
        return "(n/a)";
    }
    const char *data = xprop("_NET_WM_NAME", "UTF8_STRING", win,
                             LONG_MAX / 4);

    char *_NET_WM_NAME = (char *)data;

    // some windows don't set _NET_WM_NAME
    if (_NET_WM_NAME == NULL) {
        const char *fallback = xprop("WM_NAME", "STRING", win,
                                     LONG_MAX / 4);
        char *WM_NAME = (char *)fallback;
        return WM_NAME;
    } else {
        return _NET_WM_NAME;
    }
}

unsigned long *get_groups_num(void) {
    const char *data = xprop("_NET_NUMBER_OF_DESKTOPS", "CARDINAL",
                             root, 4);
    if (data == NULL) {
        return (unsigned long *)1;
    }
    return (unsigned long *)data;
}

unsigned long *get_group(void) {
    const char *data = xprop("_NET_CURRENT_DESKTOP", "CARDINAL",
                             root, 4);
    if (data == NULL) {
        return (unsigned long *)1;
    }

    return (unsigned long *)data;
}

static const char *get_groups_str(void) {
    unsigned long current = *get_group();
    unsigned long last = *get_groups_num();

    char result[100];
    for (unsigned long i = 1; i <= last; i++) {
        if (i == current) {
            char tmp[40];
            snprintf(tmp, sizeof(tmp),
                     "%%{B%s}%%{F%s} %lu %%{B-}%%{F-}",
                     ACTIVE_GROUP_BG, ACTIVE_GROUP_FG, i);
            strncat(result, tmp, sizeof(result) - sizeof(tmp) - 1);
        } else {
            char tmp[8];
            snprintf(tmp, sizeof(tmp), " %lu ", i);
            strncat(result, tmp, sizeof(result) - sizeof(tmp) - 1);
        }
    }
    return strdup(result);
}

static const char *get_layout(void) {
    XkbStateRec state;
    XkbGetState(dpy, XkbUseCoreKbd, &state);

    XkbRF_VarDefsRec vd;
    XkbRF_GetNamesProp(dpy, NULL, &vd);

    char *tok = strtok(vd.layout, ",");

    for (int i = 0; i < state.group; i++) {
        tok = strtok(NULL, ",");
        if (tok == NULL) {
            return "";
        }
    }

    return tok;
}

static const char *get_battery(void) {
    int apm_dev_fd = open("/dev/apm", O_RDONLY);
    if (apm_dev_fd < 0) {
        return "(n/a)";
    }

    struct apm_power_info apm_info;
    if ((ioctl(apm_dev_fd, APM_IOC_GETPOWER, &apm_info)) < 0) {
        return "(n/a)";
    }

    char *state = NULL;
    
    switch (apm_info.ac_state) {
    case APM_AC_OFF:
        state = "-";
        break;
    case APM_AC_ON:
        state = "+";
        break;
    case APM_AC_BACKUP:
    case APM_AC_UNKNOWN:
        return "-";
    default:
        state = "?";
    }

    static char buf[7];
    snprintf(buf, sizeof(buf), "%s %d%%", state, apm_info.battery_life);
    return strdup(buf);
}

static const char *get_time(void) {
    time_t t = time(NULL);
    struct tm tm = *localtime(&t);

    static char buf[20];
    snprintf(buf, sizeof(buf), "%d.%02d.%02d %02d:%02d:%02d",
             tm.tm_mday, tm.tm_mon + 1, tm.tm_year + 1900,
             tm.tm_hour, tm.tm_min, tm.tm_sec);
    return strdup(buf);
}

int init_display(void) {
    dpy = XOpenDisplay(NULL);
    if (dpy == NULL) {
        die("XOpenDisplay");
    }

    root = XRootWindow(dpy, 0);

    XSelectInput(dpy, root, PropertyChangeMask);

    XkbSelectEventDetails(dpy, XkbUseCoreKbd, XkbStateNotify,
                          XkbAllStateComponentsMask, XkbGroupStateMask);

    return ConnectionNumber(dpy);
}

void format(void) {
    const char *layout = get_layout();
    const char *groups = get_groups_str();
    Window window = current_window();
    const char *window_name = get_window_name(window);
    const char *battery = get_battery();
    const char *time = get_time();
 
    printf("%1$s %%{B%6$s}%%{F%7$s} %2$s %%{B-}%%{F-} %%{r} %3$s %%{B%6$s}%%{F%7$s} BAT %4$s %%{B-}%%{F-} %5$s \n",
           groups, window_name, layout, battery, time, SECONDARY_BG, SECONDARY_FG);
    fflush(stdout);
}

int main(int argc, char *argv[]) {
    int kq;
    if ((kq = kqueue()) == -1) {
        die("kqueue");
    }
    
    int x_fd = init_display();

    format();

    sigset_t mask;
    sigemptyset(&mask);
    sigaddset(&mask, REDRAW_SIGNAL);
    sigaddset(&mask, REPLACE_SIGNAL);
    sigprocmask(SIG_BLOCK, &mask, NULL);

    struct kevent changes[4];
    struct kevent events[4];
    
    EV_SET(&changes[0], x_fd, EVFILT_READ, EV_ADD, 0, 0, 0);
    EV_SET(&changes[1], 4269, EVFILT_TIMER, EV_ADD | EV_ENABLE, 0, INTERVAL, 0);
    EV_SET(&changes[2], REDRAW_SIGNAL, EVFILT_SIGNAL, EV_ADD, 0, 0, 0);
    EV_SET(&changes[3], REPLACE_SIGNAL, EVFILT_SIGNAL, EV_ADD, 0, 0, 0);

    kevent(kq, changes, 4, events, 4, NULL);

    for (;;) {
        int nev = kevent(kq, changes, 4, events, 4, NULL);
        if (nev == -1) {
            die("kevent");
        } else if (nev > 0) {
            for (int i = 0; i < nev; i++) {
                if (events[i].ident == REPLACE_SIGNAL) {
                    execl(argv[0], argv[0], (char *)NULL);
                    return 0;
                }
            }
            format();
        }
    }

    close(kq);
    XCloseDisplay(dpy);

    return 0;
}
