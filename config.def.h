/* clang-format off */

#define TRUE 1
#define FALSE 0

/* max number of character that one block command can output */
#define CMDLENGTH 50

/* appearance */
static unsigned int snap = 10; /* snap pixel */
static unsigned int borderpx = 1; /* border pixel of windows */
static unsigned int systraypinning = FALSE; /* 0: sloppy systray follows selected monitor,  >0: pin systray to monitor X */
static unsigned int systrayspacing = 2; /* systray spacing */
static unsigned int systrayonleft  = FALSE; /* systray on the left of status text */
static int systraypinningfailfirst = TRUE; /* 1: if pinning fails, display systray on the first monitor, FALSE: display systray on the last monitor */
static int swallowfloating = FALSE; /* 1 means swallow floating windows by default */
static int lockfullscreen = FALSE; /* 1 will force focus on the fullscreen window */
static int focusonwheel = FALSE;
static int showsystray = FALSE; /* 0 = no systray */
static int showbar = TRUE; /* 0 = no bar */
static int topbar = TRUE; /* 0 = bottom bar */
static int gappx  = 5;  /* gaps between windows */
static int noborder = TRUE; /* 1 = hide the bar if only a single window is opened. */
static int showtitle = FALSE; /* 1 = hide the bar if only a single window is opened. */
static char font[] = "Liberation Mono:style=Regular:pixelsize=10";
static const char *fonts[] = { font };

/* inverse the order of the blocks, comment to disable */
static unsigned char block_inversed = FALSE;

/* colors */
static char normbg[]     = "#222222";
static char normborder[] = "#444444";
static char normfg[]     = "#bbbbbb";
static char selfg[]      = "#eeeeee";
static char selborder[]  = "#005577";
static char selbg[]      = "#005577";
static char normstickyborder[] = "#719611";
static char selstickyborder[]  = "#aa4450";
static char normfloatborder[]  = "#719611";
static char selfloatborder[]   = "#aa4450";

static char statusbg[]   = "#222222";
static char statusfg_1[] = "#005577";
static char statusfg_2[] = "#005577";
static char statusfg_3[] = "#005577";
static char statusfg_4[] = "#005577";
static char statusfg_5[] = "#005577";

static char *colors[][5] = {
	/* 		 fg	bg	border		sticky_border*/
	[SchemeNorm] = { normfg, normbg, normborder, normstickyborder, normfloatborder },
	[SchemeSel]  = { selfg,  selbg,  selborder,  selstickyborder, selfloatborder },
	[SchemeStatus]={ normfg,  statusbg},
};

/* status bar */
static const Block blocks[] = {
	/* f           command	                        interval	signal */
	{ statusfg_1, "date '+%a, %d %b %y - %I:%M%p'",	5,		1 },
};

/* delimeter between blocks commands. NULL character ('\0') means no delimeter. */
static char delimiter[] = " | ";

/* scratchpads */
typedef struct {
	const char *name;
	const void *cmd;
} Sp;
const char *spcmd0[] = { "st", "-g", "150x35", "-T", "SP-0", "-n", "sp-0", NULL };
const char *spcmd1[] = { "st", "-g", "150x35", "-T", "SP-1", "-n", "sp-1", NULL };
const char *spcmd2[] = { "st", "-g", "150x35", "-T", "SP-2", "-n", "sp-2", NULL };
const char *spcmd3[] = { "st", "-g", "150x35", "-T", "SP-3", "-n", "sp-3", NULL };

static Sp scratchpads[] = {
	/* name 	cmd  */
	{ "sp-0", 	spcmd0 },
	{ "sp-1", 	spcmd1 },
	{ "sp-2", 	spcmd2 },
	{ "sp-3", 	spcmd3 },
};

/* tagging */
static const char *tags[] = { "1", "2", "3", "4", "5", "6", "7", "8", "9" };
static const Rule rules[] = {
	/* xprop(1):
	 * WM_CLASS(STRING) = instance, class
	 * WM_NAME(STRING) = title */
	/* class           instance title  tags mask centered floating terminal noswallow monitor */
	{ "st-256color",   NULL,    NULL,  0, 	     FALSE,   FALSE,   TRUE,    FALSE,  -1 },
	{ NULL, 	   "sp-0",  NULL,  SPTAG(0), TRUE,    TRUE,    TRUE,    FALSE,  -1 },
	{ NULL, 	   "sp-1",  NULL,  SPTAG(1), TRUE,    TRUE,    TRUE,    FALSE,  -1 },
	{ NULL, 	   "sp-2",  NULL,  SPTAG(2), TRUE,    TRUE,    TRUE,    FALSE,  -1 },
	{ NULL, 	   "sp-3",  NULL,  SPTAG(3), TRUE,    TRUE,    TRUE,    FALSE,  -1 },
	{ NULL, 	   NULL,    "Event Tester", 0,FALSE,  FALSE,   FALSE,   TRUE,   -1 },
};

/* layout(s) */
static float mfact = 0.50; /* factor of master area size [0.05..0.95] */
static int nmaster = 1; /* number of clients in master area */
static int resizehints = 0; /* 1 = respect size hints in tiled resizals */
static const Layout layouts[] = {
	/* first entry is default */
	/* symbol 	arrange function */
	{ "T",		tile	},
	{ "M",		monocle },
 	{ "[D]",	deck	},
	{ "F",		NULL	},
};

/* key definitions */
#define MODKEY Mod4Mask
#define TAGKEYS(KEY,TAG)                                                \
{ MODKEY,			KEY,	view,		{.ui = 1 << TAG} }, \
{ MODKEY|ControlMask,		KEY,	toggleview, 	{.ui = 1 << TAG} }, \
{ MODKEY|ShiftMask,		KEY,	tag,		{.ui = 1 << TAG} }, \
{ MODKEY|ControlMask|ShiftMask, KEY,	toggletag,	{.ui = 1 << TAG} },

/* helper for spawning shell commands in the pre dwm-5.0 fashion */
#define SHCMD(cmd) { .v = (const char*[]){ "/bin/sh", "-c", cmd, NULL } }

/* mediakey's keycodes */
/* #include <X11/XF86keysym.h> */
#include "powermenu.c"

/* commands */
static const char *dmenucmd[]	= { "dmenu_run", "-g", "3", "-l", "15", "-i", "-p", "RUN:", NULL };
static const char *termcmd[]	= { "st", NULL };

/* Xresources preferences to load at startup */
ResourcePref resources[] = {
	{ "font", 			STRING, 	&font },
	{ "normbg",			STRING, 	&normbg },
	{ "normborder", 		STRING, 	&normborder },
	{ "normfg",			STRING, 	&normfg },
	{ "selbg",			STRING,		&selbg },
	{ "selborder",			STRING, 	&selborder },
	{ "selfg",			STRING, 	&selfg },
	{ "selstickyborder", 		STRING, 	&selstickyborder },
	{ "normstickyborder",		STRING, 	&normstickyborder },
	{ "selfloatborder", 		STRING, 	&selfloatborder },
	{ "normfloatborder",		STRING, 	&normfloatborder },
	{ "borderpx", 			INTEGER,	&borderpx },
	{ "snap", 			INTEGER,	&snap },
	{ "showbar", 			INTEGER,	&showbar },
	{ "topbar", 			INTEGER,	&topbar },
	{ "nmaster", 			INTEGER,	&nmaster },
	{ "resizehints", 		INTEGER,	&resizehints },
	{ "mfact", 			FLOAT,		&mfact },
	{ "focusonwheel", 		INTEGER, 	&focusonwheel},
	{ "lockfullscreen", 		INTEGER, 	&lockfullscreen},
	{ "showsystray", 		INTEGER, 	&showsystray },
	{ "systraypinning", 		INTEGER, 	&systraypinning },
	{ "systrayspacing", 		INTEGER, 	&systrayspacing },
	{ "systrayonleft", 		INTEGER, 	&systrayonleft },
	{ "systraypinningfailfirst", 	INTEGER, 	&systraypinningfailfirst },
	{ "noborder", 			INTEGER, 	&noborder },
	{ "gaps",			INTEGER,	&gappx },
	{ "showtitle", 			INTEGER, 	&showtitle },
	{ "swallowfloating", 		INTEGER, 	&swallowfloating },
	{ "statusfg_1",			STRING,		&statusfg_1 },
	{ "statusfg_2",			STRING,		&statusfg_2 },
	{ "statusfg_3",			STRING,		&statusfg_3 },
	{ "statusfg_4",			STRING,		&statusfg_4 },
	{ "statusfg_5",			STRING,		&statusfg_5 },
	{ "statusbg",			STRING,		&statusbg   },
	{ "block_inversed",             INTEGER,        &block_inversed },
	{ "block_delimiter",            CHAR,		&delimiter[1] },
};

/* keybindings */
static const Key keys[] = {
	/* modifier		key 			function	argument */
	{ MODKEY,		XK_space,		spawn,		{.v = dmenucmd } },
	{ MODKEY,		XK_Return,		spawn,		{.v = termcmd } },

	{ MODKEY,		XK_b,			togglebar,	{0} },

	{ MODKEY,		XK_j,			focusstack,	{.i = +1 } },
	{ MODKEY,		XK_k,			focusstack,	{.i = -1 } },

	{ MODKEY|ControlMask,	XK_equal,		incnmaster,	{.i = +1 } },
	{ MODKEY|ControlMask,	XK_minus,		incnmaster,	{.i = -1 } },

	{ MODKEY,		XK_h,			setmfact,	{.f = -0.05} },
	{ MODKEY,		XK_l,			setmfact,	{.f = +0.05} },

	{ MODKEY|ShiftMask,	XK_c,			killclient,	{0} },

	{ MODKEY,		XK_bracketleft,		setlayout,	{.v = &layouts[0]} },
	{ MODKEY|ShiftMask,	XK_bracketleft,		setlayout,	{.v = &layouts[1]} },
	{ MODKEY,		XK_bracketright,	setlayout,	{.v = &layouts[2]} },
	{ MODKEY|ShiftMask, 	XK_bracketright, 	setlayout,      {.v = &layouts[3]} },
	{ MODKEY|ControlMask,	XK_space, 		setlayout, 	{0} },

	{ MODKEY,		XK_s, 			togglefloating,	{0} },
	{ MODKEY,		XK_f, 			togglefullscr, 	{0} },
	{ MODKEY,		XK_x, 			togglesticky,	{0} },

	{ MODKEY|ShiftMask,	XK_j, 			pushdown,	{0} },
	{ MODKEY|ShiftMask,	XK_k, 			pushup,		{0} },
	{ MODKEY|ShiftMask,	XK_h, 			view_adjacent, 	{.i = -1 } },
	{ MODKEY|ShiftMask,	XK_l, 			view_adjacent, 	{.i = +1 } },

	{ MODKEY,		XK_Down, 		moveresize,	{.v = "0x 25y 0w 0h" } },
	{ MODKEY,		XK_Up,			moveresize,	{.v = "0x -25y 0w 0h" } },
	{ MODKEY,		XK_Right, 		moveresize,	{.v = "25x 0y 0w 0h" } },
	{ MODKEY,		XK_Left,		moveresize,	{.v = "-25x 0y 0w 0h" } },

	{ MODKEY|ShiftMask,	XK_Down,		moveresize,	{.v = "0x 0y 0w 25h" } },
	{ MODKEY|ShiftMask,	XK_Up,			moveresize,	{.v = "0x 0y 0w -25h" } },
	{ MODKEY|ShiftMask,	XK_Right, 		moveresize,	{.v = "0x 0y 25w 0h" } },
	{ MODKEY|ShiftMask,	XK_Left,		moveresize,	{.v = "0x 0y -25w 0h" } },

	{ MODKEY|ControlMask,	XK_Up,			moveresizeedge,	{.v = "t"} },
	{ MODKEY|ControlMask,	XK_Down,		moveresizeedge,	{.v = "b"} },
	{ MODKEY|ControlMask,	XK_Left,		moveresizeedge,	{.v = "l"} },
	{ MODKEY|ControlMask,	XK_Right, 		moveresizeedge,	{.v = "r"} },

	{ MODKEY|ControlMask|ShiftMask,	XK_Up,		moveresizeedge,	{.v = "T"} },
	{ MODKEY|ControlMask|ShiftMask,	XK_Down,	moveresizeedge,	{.v = "B"} },
	{ MODKEY|ControlMask|ShiftMask,	XK_Left,	moveresizeedge,	{.v = "L"} },
	{ MODKEY|ControlMask|ShiftMask,	XK_Right, 	moveresizeedge,	{.v = "R"} },

	{ MODKEY,		XK_n, 			focusmaster,	{0} },
	{ MODKEY|ShiftMask,	XK_n,			zoom,		{0} },
	{ MODKEY,		XK_Tab, 		view,		{0} },

	{ MODKEY,		XK_0, 			view,		{.ui = ~0 } },
	{ MODKEY|ShiftMask,	XK_0, 			tag, 		{.ui = ~0 } },

	{ MODKEY,		XK_comma, 		focusmon,	{.i = -1 } },
	{ MODKEY,		XK_period, 		focusmon,	{.i = +1 } },

	{ MODKEY|ShiftMask,	XK_comma, 		tagmon,	 	{.i = -1 } },
	{ MODKEY|ShiftMask,	XK_period, 		tagmon,	 	{.i = +1 } },

	TAGKEYS( 		XK_1, 					0 )
	TAGKEYS( 		XK_2, 					1 )
	TAGKEYS( 		XK_3, 					2 )
	TAGKEYS( 		XK_4, 					3 )
	TAGKEYS( 		XK_5, 					4 )
	TAGKEYS( 		XK_6, 					5 )
	TAGKEYS( 		XK_7, 					6 )
	TAGKEYS( 		XK_8, 					7 )
	TAGKEYS( 		XK_9, 					8 )

	{ MODKEY,	 	XK_u, 			togglescratch, 	{.ui = 0 } },
	{ MODKEY,	 	XK_i, 			togglescratch, 	{.ui = 1 } },
	{ MODKEY,	 	XK_o, 			togglescratch, 	{.ui = 2 } },
	{ MODKEY,	 	XK_p, 			togglescratch, 	{.ui = 3 } },

	{ MODKEY|Mod1Mask|ControlMask, 	XK_Escape,	quit,		{0} },	// quit WM
	{ MODKEY|Mod1Mask, 	XK_r, 			quit,		{1} },	// reload WM
	{ MODKEY|Mod1Mask,	XK_Escape,		powermenu,	{0} },  // powermenu
};

/* button definitions */
/* click can be ClkTagBar, ClkLtSymbol, ClkStatusText, ClkWinTitle, ClkClientWin, or ClkRootWin */
static const Button buttons[] = {
	/* click event 		mask 			button 		function 		argument */
	{ ClkLtSymbol,		0,			Button1,	setlayout,		{0} },
	{ ClkLtSymbol,		0,			Button3,	setlayout,		{.v = &layouts[2]} },
	{ ClkWinTitle,		0,			Button2,	zoom,			{0} },
	{ ClkStatusText,	0,			Button2,	spawn,			{.v = termcmd } },
	{ ClkClientWin,		MODKEY,			Button1,	movemouse,		{0} },
	{ ClkClientWin,		MODKEY,			Button2,	togglefloating,		{0} },
	{ ClkClientWin,		MODKEY|ShiftMask,	Button1,	resizemouse,		{0} },
	{ ClkStatusText,        0,			Button1,        sendstatusbar,		{.i = 1 } },
	{ ClkTagBar,		0,			Button1,	view,			{0} },
	{ ClkTagBar,		0,			Button3,	toggleview,		{0} },
	{ ClkTagBar,		MODKEY,			Button1,	tag,			{0} },
	{ ClkTagBar,		MODKEY,			Button3,	toggletag,		{0} },
};
/* clang-format on */
