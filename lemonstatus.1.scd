LEMONSTATUS(1)

# NAME

lemonstatus - generate lemonbar content

# SYNOPSIS

*lemonstatus* | lemonbar -p

# DESCRIPTION

*lemonstatus* is developed for use with OpenBSD and cwm. It depends on++
*apm* and *kqueue*, and displays group numbers from \_NET_CURRENT_DESKTOP++
and \_NET_NUMBER_OF_DESKTOPS X properties.

# CONFIGURATION

*lemonstatus* is configured through config.h file, 'cause it doesn't suck.

## Signals

- REDRAW_SIGNAL (defaults to SIGUSR1) - redraw status immediately
- REPLACE_SIGNAL (defaults to SIGUSR2) - replace current instance (useful after make install)

# SEE ALSO

lemonbar(1)

# AUTHOR

peajack <peajackboatan@ya.ru>
