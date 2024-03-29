#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void
powermenu()
{
#if defined S_LOCK || defined S_RESTART_DWM || defined S_SCREEN_OFF ||         \
    defined S_EXIT || defined S_REBOOT || defined S_SHUTDOWN ||                \
    defined S_FORMAT || defined S_FORMAT_CLEAR
#error(conflicting macro names)
#endif

#define S_LOCK            "Lock"
#define S_RESTART_DWM     "Restart Dwm"
#define S_SCREEN_OFF      "Screen Off"
#define S_EXIT            "Exit"
#define S_REBOOT          "Reboot"
#define S_SHUTDOWN        "Shutdown"
#define S_LOAD_XRESOURCES "Load Xresources"
#define S_CHANGE_WP       "Change WP"

#define S_FORMAT(ACTION) S_##ACTION
#define S_FORMAT_CLEAR   "sed 's/^..//'"

	FILE* exit_menu = popen(
	    "echo \"" S_FORMAT(LOAD_XRESOURCES) "\n" S_FORMAT(CHANGE_WP) "\n" S_FORMAT(LOCK) "\n" S_FORMAT(RESTART_DWM) "\n" S_FORMAT(
	        SCREEN_OFF) "\n" S_FORMAT(EXIT) "\n" S_FORMAT(REBOOT) "\n" S_FORMAT(SHUTDOWN) "\" | dmenu -i -l 10 -p 'Power Menu':",
	    "r");

	char exit_action[16];

	if (exit_menu == NULL ||
	    fscanf(exit_menu, "%15[a-zA-Z -]", exit_action) == EOF) {
		fputs("Error. Failure in exit_dwm.", stderr);
		goto close_streams;
	}

	if (strcmp(exit_action, S_LOCK) == 0)
		system("xsecurelock");
	else if (strcmp(exit_action, S_RESTART_DWM) == 0)
		quit(&(const Arg){1});
	else if (strcmp(exit_action, S_SCREEN_OFF) == 0)
		system("sleep .5; xset dpms force off");
	else if (strcmp(exit_action, S_EXIT) == 0)
		quit(&(const Arg){0});
	else if (strcmp(exit_action, S_REBOOT) == 0)
		system("systemctl reboot");
	else if (strcmp(exit_action, S_SHUTDOWN) == 0)
		system("systemctl poweroff -i");
	else if (strcmp(exit_action, S_CHANGE_WP) == 0)
		system("RandoWall");
	else if (strcmp(exit_action, S_LOAD_XRESOURCES) == 0) {
		system("xrdb $HOME/.Xresources");
		quit(&(const Arg){1});
	}

close_streams:
	pclose(exit_menu);

#undef S_LOCK
#undef S_RESTART_DWM
#undef S_SCREEN_OFF
#undef S_EXIT
#undef S_REBOOT
#undef S_SHUTDOWN
#undef S_FORMAT
#undef S_FORMAT_CLEAR
}
