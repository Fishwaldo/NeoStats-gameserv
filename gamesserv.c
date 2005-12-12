/* GamesServ - Small Games Service - NeoStats Addon Module
** Copyright (c) 2005 Justin Hammond, Mark Hetherington, DeadNotBuried
**
**  This program is free software; you can redistribute it and/or modify
**  it under the terms of the GNU General Public License as published by
**  the Free Software Foundation; either version 2 of the License, or
**  (at your option) any later version.
**
**  This program is distributed in the hope that it will be useful,
**  but WITHOUT ANY WARRANTY; without even the implied warranty of
**  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
**  GNU General Public License for more details.
**
**  You should have received a copy of the GNU General Public License
**  along with this program; if not, write to the Free Software
**  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307
**  USA
**
** GamesServ CVS Identification
** $Id$
*/

#include "neostats.h"    /* Required for bot support */
#include "gamesserv.h"

/*
 * Module Variables
*/
int kickgameschanoponly;

/*
 * Game Variables
*/
char gameroom[GS_GAME_CHANNEL_TOTAL][MAXCHANLEN];
int gamestatus[GS_GAME_CHANNEL_TOTAL];
char gameplayernick[GS_GAME_CHANNEL_TOTAL][MAXNICK];
int countdowntime[GS_GAME_CHANNEL_TOTAL];

Bot *gs_bot;

/*
 * Copyright info and About
*/
static const char *gs_copyright[] = {
	"Copyright (c) 2005 Justin Hammond, Mark Hetherington, DeadNotBuried",
	NULL
};

static const char *gs_about[] = {
	"\2Small IRC Games Service\2",
	"",
	"This Service is intended as a fun only.",
	"",
	"If you take any of these games seriously",
	"you should turn off your computer, drop",
	"it off a cliff, and find a life outside",
	"of IRC.",
	NULL
};

/*
 * Module Info definition 
*/
ModuleInfo module_info = {
	"GamesServ",
	"Small Games Module For NeoStats",
	gs_copyright,
	gs_about,
	NEOSTATS_VERSION,
	MODULE_VERSION,
	__DATE__,
	__TIME__,
	0,
	0,
	0,
};

/*
 * Module event list
*/
ModuleEvent module_events[] = {
	{EVENT_NICK, PlayerNickChange, 0},
	NS_EVENT_END()
};

/*
 * Commands and Settings
*/
static bot_cmd gs_commands[]=
{
	{"TTT",		startttt,	0,	0,	gs_help_ttt, 0, NULL, NULL},
	{"X",		playttt,	2,	0,	gs_help_x, 0, NULL, NULL},
	{"BOMB",	startbomb,	1,	0,	gs_help_bomb, 0, NULL, NULL},
	{"THROW",	throwbomb,	1,	0,	gs_help_throw, 0, NULL, NULL},
	{"RUSSIAN",	startruss,	1,	0,	gs_help_russ, 0, NULL, NULL},
	{"SHOOT",	shootruss,	1,	0,	gs_help_shoot, 0, NULL, NULL},
	{"HILO",	starthilo,	1,	0,	gs_help_hilo, 0, NULL, NULL},
	{"GUESS",	guesshilo,	1,	0,	gs_help_guess, 0, NULL, NULL},
	NS_CMD_END()
};

static bot_setting gs_settings[]=
{
	{"KICKGAMESCHANOPONLY",	&kickgameschanoponly,	SET_TYPE_BOOLEAN,	0,	0,	NS_ULEVEL_ADMIN,	NULL,	gs_help_set_kickchanoponly,	NULL,	(void *)0 },
	NS_SETTING_END()
};

/*
 * BotInfo
*/
static BotInfo gs_botinfo = 
{
	"GamesServ",
	"GamesServ1",
	"GamesServ",
	BOT_COMMON_HOST,
	"Small Game Service",
	BOT_FLAG_SERVICEBOT,
	gs_commands,
	gs_settings,
};

/*
 * Online event processing
*/
int ModSynch (void)
{
	/* Introduce a bot onto the network */
	gs_bot = AddBot (&gs_botinfo);	
	if (!gs_bot) {
		return NS_FAILURE;
	}
	srand((unsigned int)me.now);
	return NS_SUCCESS;
}

/*
 * Init module
*/
int ModInit( void )
{
	int i;

	/* clear Game variables */
	for (i = 0; i < GS_GAME_CHANNEL_TOTAL; i++) {
		gameroom[i][0] = '\0';
		gameplayernick[i][0] = '\0';
		gamestatus[i] = GS_GAME_CHANNEL_STOPPED;
		countdowntime[i] = TS_ONE_MINUTE;
	}
	ModuleConfig (gs_settings);
	return NS_SUCCESS;
}

/*
 * Exit module
*/
int ModFini( void )
{
	if ( gamestatus[GS_GAME_CHANNEL_BOMB] == GS_GAME_CHANNEL_PLAYING ) {
		DelTimer ("bombcountdown");
	}
	if ( gamestatus[GS_GAME_CHANNEL_RUSS] == GS_GAME_CHANNEL_PLAYING ) {
		DelTimer ("russcountdown");
	}
	if ( gamestatus[GS_GAME_CHANNEL_HILO] == GS_GAME_CHANNEL_PLAYING ) {
		DelTimer ("hilocountdown");
	}
	return NS_SUCCESS;
}
