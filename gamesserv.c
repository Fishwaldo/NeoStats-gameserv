/* GamesServ - Small Games Service - NeoStats Addon Module
** Copyright (c) 2004 DeadNotBuried
** Portions Copyright (c) 1999-2005, NeoStats",
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
char gameroom[GS_GAME_TOTAL][MAXCHANLEN];
int gamestatus[GS_GAME_TOTAL];
char gameplayernick[GS_GAME_TOTAL][MAXNICK];
int countdowntime[GS_GAME_TOTAL];

/*
 * Copyright info and About
*/
const char *gs_copyright[] = {
	"Copyright (c) 2005 DeadNotBuried",
	"Portions Copyright (c) 1999-2005, NeoStats",
	NULL
};

const char *gs_about[] = {
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
	"3.0",
	__DATE__,
	__TIME__,
	0,
	0,
};

/*
 * Module event list
*/
ModuleEvent module_events[] = {
	{EVENT_NICK, PlayerNickChange},
	{EVENT_NULL, NULL}
};

/*
 * Nick Change Check
*/
int PlayerNickChange (CmdParams* cmdparams)
{
	int i;

	for (i = 0; i < GS_GAME_TOTAL; i++) {
		if (!ircstrcasecmp (gameplayernick[i], cmdparams->param)) {
			strlcpy (gameplayernick[i], cmdparams->source->name, MAXNICK);
		}
	}
	return NS_SUCCESS;
}

/*
 * Commands and Settings
*/
static bot_cmd gs_commands[]=
{
	{"BOMB",	startbomb,	1,	0,	gs_help_bomb,	gs_help_bomb_oneline},
	{"THROW",	throwbomb,	1,	0,	gs_help_throw,	gs_help_throw_oneline},
	{"RUSSIAN",	startruss,	1,	0,	gs_help_russ,	gs_help_russ_oneline},
	{"SHOOT",	shootruss,	1,	0,	gs_help_shoot,	gs_help_shoot_oneline},
	{NULL,		NULL,		0, 	0,	NULL,		NULL}
};

static bot_setting gs_settings[]=
{
	{"KICKGAMESCHANOPONLY",	&kickgameschanoponly,	SET_TYPE_BOOLEAN,	0,	0,	NS_ULEVEL_ADMIN,	"KickGamesStartByChanOpsOnly",	NULL,	gs_help_set_kickchanoponly,	NULL,	(void *)0 },
	{NULL,			NULL,			0,			0,	0,	0,			NULL,				NULL,	NULL, 				NULL },
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
};

/*
 * Init module
*/
int ModInit( void )
{
	int i;

	/* clear Game variables */
	for (i = 0; i < GS_GAME_TOTAL; i++) {
		gameroom[i][0] = '\0';
		gameplayernick[i][0] = '\0';
		gamestatus[i] = GS_GAME_STOPPED;
		countdowntime[i] = 60;
	}
	ModuleConfig (gs_settings);
	return NS_SUCCESS;
}

/*
 * Exit module
*/
int ModFini( void )
{
	if ( gamestatus[GS_GAME_BOMB] == GS_GAME_PLAYING ) {
		DelTimer ("bombcountdown");
	}
	if ( gamestatus[GS_GAME_RUSS] == GS_GAME_PLAYING ) {
		DelTimer ("russcountdown");
	}
	return NS_SUCCESS;
}
