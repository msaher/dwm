/* See LICENSE file for copyright and license details. */

/* appearance */
static const unsigned int borderpx  = 5;        /* border pixel of windows */
static const unsigned int snap      = 32;       /* snap pixel */
static const int swallowfloating    = 0;        /* 1 means swallow floating windows by default */
static const unsigned int gappih    = 20;       /* horiz inner gap between windows */
static const unsigned int gappiv    = 10;       /* vert inner gap between windows */
static const unsigned int gappoh    = 10;       /* horiz outer gap between windows and screen edge */
static const unsigned int gappov    = 30;       /* vert outer gap between windows and screen edge */
static       int smartgaps          = 0;        /* 1 means no outer gap when there is only one window */
static const int showbar            = 1;        /* 0 means no bar */
static const int topbar             = 1;        /* 0 means bottom bar */
static const int startontag         = 1;        /* 0 means no tag active on start */
/* static const char *fonts[]          = { "Dejavu:pixelsize=22;1", "fontawesome:pixelsize=15", "Source Han Sans JP,源ノ角ゴシック JP,Source Han Sans JP Medium,源ノ角ゴシック JP Medium:style=Medium,Regular", "Noto Color Emoji" }; */
static const char *fonts[]          = { "DejaVu Sans Mono:pixelsize=30:antialias=true:autohint=true", "fontawesome:pixelsize=15;" "Source Han Sans JP,源ノ角ゴシック JP,Source Han Sans JP Medium,源ノ角ゴシック JP Medium:style=Medium,Regular;", "Noto Color Emoji" };
static const char dmenufont[]       = "monospace:size=10";
static char normbgcolor[]           = "#222222";
static char normbordercolor[]       = "#444444";
static char normfgcolor[]           = "#bbbbbb";
static char selfgcolor[]            = "#eeeeee";
static char selbordercolor[]        = "#005577";
static char selbgcolor[]            = "#005577";
static char *colors[][3] = {
       /*               fg           bg           border   */
       [SchemeNorm] = { normfgcolor, normbgcolor, normbordercolor },
       [SchemeSel]  = { selfgcolor,  selbgcolor,  selbordercolor  },
};

/* tagging */
static const char *tags[] = { "1", "2", "3", "4", "5", "6", "7", "8", "9" };

static const Rule rules[] = {
	/* xprop(1):
	 *	WM_CLASS(STRING) = instance, class
	 *	WM_NAME(STRING) = title
	 */
	/* class     instance  title           tags mask  isfloating  isterminal  noswallow  monitor */
	/* { "Gimp",    NULL,     NULL,           0,         1,          0,           0,        -1 }, */
	/* { "Firefox", NULL,     NULL,           1 << 8,    0,          0,          -1,        -1 }, */
	{ "qTox",     NULL,          NULL,       1 << 8,       0,            0,         0,        -1 },
	{ "Element",  NULL,          NULL,       1 << 8,       0,            0,         0,        -1 },
	{ "discord",  NULL,          NULL,       1 << 7,       0,            0,         0,        -1 },
	{ "St",       NULL,          NULL,       0,            0,            1,         0,        -1 },
	{ "Sxiv",    "random",       NULL,       0,            1,            0,         0,        -1 },
	{ NULL,      NULL,     "Event Tester",   0,            0,            0,         1,        -1 }, /* xev */
};

/* layout(s) */
static const float mfact     = 0.55; /* factor of master area size [0.05..0.95] */
static const int nmaster     = 1;    /* number of clients in master area */
static const int resizehints = 0;    /* 1 means respect size hints in tiled resizals */

#define FORCE_VSPLIT 1  /* nrowgrid layout: force two clients to always split vertically */
#include "layouts.c"

static const Layout layouts[] = {
	/* symbol     arrange function */
	{ "[]=",      tile },    /* first entry is default */
	{ "TTT",      bstack },
	{ "===",      bstackhoriz },
	{ "HHH",      grid },
	{ "###",      nrowgrid },
	{ "---",      horizgrid },
	{ ":::",      gaplessgrid },
	{ ">M>",      centeredfloatingmaster },
	{ "|M|",      centeredmaster },
	{ "H[]",      deck },
	{ "[M]",      monocle },
	{ "[@]",      spiral },
	{ "[\\]",     dwindle },
	{ "><>",      NULL },    /* no layout function means floating behavior */
	{ NULL,       NULL },
};

static const char *layoutmenu_cmd = "dwm-layoutmenu";

static void layoutmenu(const Arg *arg);

void
layoutmenu(const Arg *arg) {
	FILE *p;
	char c[3], *s;
	int i;

	if (!(p = popen(layoutmenu_cmd, "r")))
		 return;
	s = fgets(c, sizeof(c), p);
	pclose(p);

	if (!s || *s == '\0' || c == '\0')
		 return;

	i = atoi(c);
	setlayout(&((Arg) { .v = &layouts[i] }));
}

/* key definitions */
#define MODKEY Mod4Mask
#define TAGKEYS(KEY,TAG) \
	{ MODKEY,                       KEY,      view,           {.ui = 1 << TAG} }, \
	{ Mod1Mask,                     KEY,      toggleview,     {.ui = 1 << TAG} }, \
	{ MODKEY|ShiftMask,             KEY,      tag,            {.ui = 1 << TAG} }, \
	{ Mod1Mask|ShiftMask,           KEY,      toggletag,      {.ui = 1 << TAG} },

/* helper for spawning shell commands in the pre dwm-5.0 fashion */
#define SHCMD(cmd) { .v = (const char*[]){ "/bin/sh", "-c", cmd, NULL } }

/* commands */
static char dmenumon[2] = "0"; /* component of dmenucmd, manipulated in spawn() */
static const char *dmenucmd[] = { "dmenu_run", "-m", dmenumon, "-fn", dmenufont, "-nb", normbgcolor, "-nf", normfgcolor, "-sb", selbordercolor, "-sf", selfgcolor, NULL };
static const char *termcmd[]  = { "st", NULL };

#include "movestack.c"
static Key keys[] = {
	/* modifier                     key        function        argument */
	/* { MODKEY,                       XK_p,      spawn,          {.v = dmenucmd } }, */
	/* { MODKEY|ShiftMask,             XK_Return, spawn,          {.v = termcmd } }, */
	{ MODKEY|ShiftMask,             XK_b,      togglebar,      {0} },
	{ MODKEY,                       XK_j,      focusstack,     {.i = +1 } },
	{ MODKEY,                       XK_k,      focusstack,     {.i = -1 } },
	{ MODKEY|ShiftMask,             XK_j,      movestack,      {.i = +1 } },
	{ MODKEY|ShiftMask,             XK_k,      movestack,      {.i = -1 } },
	{ MODKEY,                       XK_u,      resetnmaster,   {0} },
	{ MODKEY,                       XK_equal,  incnmaster,     {.i = +1 } },
	{ MODKEY,                       XK_minus,  incnmaster,     {.i = -1 } },
	{ MODKEY,                       XK_h,      setmfact,       {.f = -0.05} },
	{ MODKEY,                       XK_l,      setmfact,       {.f = +0.05} },
	{ MODKEY|ShiftMask,             XK_h,      setcfact,       {.f = +0.25} },
	{ MODKEY|ShiftMask,             XK_l,      setcfact,       {.f = -0.25} },
	{ MODKEY|ShiftMask,             XK_o,      setcfact,       {.f =  0.00} },
	{ MODKEY,                       XK_e,      zoom,           {0} },
	{ MODKEY,                       XK_i,      incrgaps,       {.i = +1 } },
	{ MODKEY,                       XK_o,      incrgaps,       {.i = -1 } },
	/* { MODKEY|Mod4Mask,              XK_i,      incrigaps,      {.i = +1 } }, */
	/* { MODKEY|Mod4Mask|ShiftMask,    XK_i,      incrigaps,      {.i = -1 } }, */
	/* { MODKEY|Mod4Mask,              XK_o,      incrogaps,      {.i = +1 } }, */
	/* { MODKEY|Mod4Mask|ShiftMask,    XK_o,      incrogaps,      {.i = -1 } }, */
	/* { MODKEY|Mod4Mask,              XK_6,      incrihgaps,     {.i = +1 } }, */
	/* { MODKEY|Mod4Mask|ShiftMask,    XK_6,      incrihgaps,     {.i = -1 } }, */
	/* { MODKEY|Mod4Mask,              XK_7,      incrivgaps,     {.i = +1 } }, */
	/* { MODKEY|Mod4Mask|ShiftMask,    XK_7,      incrivgaps,     {.i = -1 } }, */
	/* { MODKEY|Mod4Mask,              XK_8,      incrohgaps,     {.i = +1 } }, */
	/* { MODKEY|Mod4Mask|ShiftMask,    XK_8,      incrohgaps,     {.i = -1 } }, */
	/* { MODKEY|Mod4Mask,              XK_9,      incrovgaps,     {.i = +1 } }, */
	/* { MODKEY|Mod4Mask|ShiftMask,    XK_9,      incrovgaps,     {.i = -1 } }, */
	{ MODKEY,                       XK_g,      togglegaps,     {0} },
	{ MODKEY|ShiftMask,             XK_g,      defaultgaps,    {0} },
	{ MODKEY,                       XK_Tab,    view,           {0} },
	{ MODKEY,                       XK_q,      killclient,     {0} },
	{ MODKEY,                       XK_t,      setlayout,      {.v = &layouts[0]} },
	{ MODKEY,                       XK_r,      setlayout,      {.v = &layouts[1]} },
	{ MODKEY|ShiftMask,             XK_r,      setlayout,      {.v = &layouts[2]} },
	{ MODKEY|ControlMask,           XK_h,      setlayout,      {.v = &layouts[3]} },
	{ MODKEY,                       XK_n,      setlayout,      {.v = &layouts[4]} },
	{ MODKEY|ShiftMask,             XK_n,      setlayout,      {.v = &layouts[5]} },
	{ Mod1Mask,                     XK_g,      setlayout,      {.v = &layouts[6]} },
	{ Mod1Mask,                     XK_c,      setlayout,      {.v = &layouts[7]} },
	{ MODKEY,                       XK_c,      setlayout,      {.v = &layouts[8]} },
	{ Mod1Mask,                     XK_d,      setlayout,      {.v = &layouts[9]} },
	{ MODKEY,                       XK_f,      setlayout,      {.v = &layouts[10]} },
	{ MODKEY,                       XK_s,      setlayout,      {.v = &layouts[11]} },
	{ MODKEY|ShiftMask,             XK_s,      setlayout,      {.v = &layouts[12]} },
	{ MODKEY|ShiftMask,             XK_f,      setlayout,      {.v = &layouts[13]} },
	{ Mod1Mask|ShiftMask,           XK_space,  setlayout,      {0} },
	{ Mod1Mask,                     XK_space,  togglefloating, {0} },
        { MODKEY,                  XK_apostrophe,  cyclelayout,    {.i = +1} },
        { MODKEY,                  XK_semicolon,   cyclelayout,    {.i = -1} },
        { MODKEY,                       XK_p,      layoutmenu,     {0} },
	{ MODKEY,                       XK_0,      view,           {.ui = ~0 } },
	{ MODKEY|ShiftMask,             XK_0,      tag,            {.ui = ~0 } },
	{ MODKEY,                       XK_F5,     xrdb,           {.v = NULL } },
	{ MODKEY,                       XK_F9,     focusmon,       {.i = -1 } },
	{ MODKEY,                       XK_F10,    focusmon,       {.i = +1 } },
	{ MODKEY|ShiftMask,             XK_F9,     tagmon,         {.i = -1 } },
	{ MODKEY|ShiftMask,             XK_F10,    tagmon,         {.i = +1 } },
	TAGKEYS(                        XK_1,                      0)
	TAGKEYS(                        XK_2,                      1)
	TAGKEYS(                        XK_3,                      2)
	TAGKEYS(                        XK_4,                      3)
	TAGKEYS(                        XK_5,                      4)
	TAGKEYS(                        XK_6,                      5)
	TAGKEYS(                        XK_7,                      6)
	TAGKEYS(                        XK_8,                      7)
	TAGKEYS(                        XK_9,                      8)
	{ MODKEY|ShiftMask,             XK_q,      quit,           {0} },
};

/* button definitions */
/* click can be ClkTagBar, ClkLtSymbol, ClkStatusText, ClkWinTitle, ClkClientWin, or ClkRootWin */
static Button buttons[] = {
	/* click                event mask      button          function        argument */
	{ ClkLtSymbol,          0,              Button1,        setlayout,      {0} },
	{ ClkLtSymbol,          0,              Button3,        setlayout,      {.v = &layouts[2]} },
	{ ClkWinTitle,          0,              Button2,        zoom,           {0} },
	{ ClkStatusText,        0,              Button2,        spawn,          {.v = termcmd } },
	{ ClkClientWin,         MODKEY,         Button1,        movemouse,      {0} },
	{ ClkClientWin,         MODKEY,         Button2,        togglefloating, {0} },
	{ ClkClientWin,         MODKEY,         Button3,        resizemouse,    {0} },
	{ ClkTagBar,            0,              Button1,        view,           {0} },
	{ ClkTagBar,            0,              Button3,        toggleview,     {0} },
	{ ClkTagBar,            MODKEY,         Button1,        tag,            {0} },
	{ ClkTagBar,            MODKEY,         Button3,        toggletag,      {0} },
};

