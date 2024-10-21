#include <X11/Xlib.h>

void die(char *call);
static const char *xprop(char *name, char *type, Window win, long length);
Window current_window(void);
unsigned long *get_groups_num(void);
unsigned long *get_group(void);
static const char *get_groups_str(void);
static const char *get_time(void);
static const char *get_window_name(Window win);
static const char *get_layout(void);
static const char *get_battery(void);
void format(void);
int init_display(void);
