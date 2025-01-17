#define REDRAW_SIGNAL   SIGUSR1
#define REPLACE_SIGNAL  SIGUSR2
#define INTERVAL        5000

#define DISPLAY_BATT        1
#define DISPLAY_CPU         1

/* Normal font logos */
/*
 * #define GROUP_LOGO       " G "
 * #define WINDOW_LOGO      "W "
 * #define TIME_LOGO        " T "
 * #define LAYOUT_LOGO      "KBD"

 * #define AC_ON_LOGO       "BAT +"
 * #define AC_OFF_LOGO      "BAT -"
 * #define AC_UKNOWN_LOGO   "BAT -"
 * #define AC_DEFAULT_LOGO  "BAT ?"

 * #define BATT_FULL_LOGO   "%%{F$C_GREEN}"
 * #define BATT_MID_LOGO    "%%{F$C_ORANGE}"
 * #define BATT_LOG_LOGO    "%%{F$C_RED}"
 *
 * #define CPU_LOGO         "S"
 * #define TEMP_LOGO        "T"
 * #define MEM_LOGO         "M"
*/

/* Nerd font logos */
#define GROUP_LOGO       "  "
#define WINDOW_LOGO      " "
#define TIME_LOGO        "  "
#define LAYOUT_LOGO      "  "

#define AC_ON_LOGO       "󰂄"
#define AC_OFF_LOGO      "-"
#define AC_UNKNOWN_LOGO  "-"
#define AC_DEFAULT_LOGO  "?"
#define BATT_FULL_LOGO   "󰁹"
#define BATT_MID_LOGO    "󰁾"
#define BATT_LOG_LOGO    "󰁻"

#define CPU_LOGO         "󰓅"
#define TEMP_LOGO        ""
#define MEM_LOGO         "󰍛"

/* generic colors */
#define C_BLUE           "#83A598"
#define C_YELLOW         "#FABD2F"
#define C_GREEN          "#98971A"
#define C_ORANGE         "#ffA500"
#define C_RED            "#FF5555"
#define C_BLACK          "#282828"
#define C_GRAY           "#928374"

/* colors gruvbox dark scheme */
#define GROUP_LOGO_BG    "#B8BB26"
#define GROUP_LOGO_FG    "#282828"
#define CURRENT_GROUP_BG "#665C54"
#define CURRENT_GROUP_FG "#928374"
#define ACTIVE_GROUP_BG  "#504945"
#define ACTIVE_GROUP_FG  "#ebdbb2"
#define NORMAL_GROUP_BG  "#3C3836"
#define NORMAL_GROUP_FG  "#7C6F64"

#define LAYOUT_BG        "#3C3836"
#define LAYOUT_FG        "#928374"

#define BATT_BG          "#3C3836"
#define BATT_FG          "#928374"

#define CPU_BG           "#3C3836"
#define CPU_FG           "#928374"

#define TIME_BG          "#928374"
#define TIME_FG          "#3C3836"
