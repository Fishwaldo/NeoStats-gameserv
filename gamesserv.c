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
*/

#include "neostats.h"    /* Required for bot support */
#include "gamesserv.h"

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
	if (!ircstrcasecmp (bombplayernick, cmdparams->param)) {
		strlcpy (bombplayernick, cmdparams->source->name, MAXNICK);
	}
	if (!ircstrcasecmp (russplayernick, cmdparams->param)) {
		strlcpy (russplayernick, cmdparams->source->name, MAXNICK);
	}
	return NS_SUCCESS;
}

static bot_cmd gs_commands[]=
{
	{"BOMB",	startbomb,	1,	0,	gs_help_bomb,	gs_help_bomb_oneline},
	{"PASS",	passbomb,	1,	0,	gs_help_pass,	gs_help_pass_oneline},
	{"RUSSIAN",	startruss,	1,	0,	gs_help_russ,	gs_help_russ_oneline},
	{"SHOOT",	shootruss,	1,	0,	gs_help_shoot,	gs_help_shoot_oneline},
	{NULL,		NULL,		0, 	0,	NULL,		NULL}
};

/*
 * BotInfo
*/
static BotInfo gs_botinfo = 
{
	"GamesServ",
	"GamesServ1",
	"WS",
	BOT_COMMON_HOST,
	"Small Game Service",
	BOT_FLAG_SERVICEBOT,
	gs_commands,
	NULL,
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
int ModInit (Module *mod_ptr)
{
	me.want_nickip = 1;
	/* clear Bomb Game variables */
	strlcpy (bombroom, "", MAXCHANLEN);
	strlcpy (bombplayernick, "", MAXNICK);
	strlcpy (currentbombgamestatus, "stopped", 10);
	/* clear Russian Roulette Game variables */
	strlcpy (russroom, "", MAXCHANLEN);
	strlcpy (russplayernick, "", MAXNICK);
	strlcpy (currentrussgamestatus, "stopped", 10);
	russcountdowntime = 60;
	return NS_SUCCESS;
}

/*
 * Exit module
*/
void ModFini (void)
{
	if (!ircstrcasecmp (currentbombgamestatus, "playing")) {
		DelTimer ("bombcountdown");
	}
	if (!ircstrcasecmp (currentrussgamestatus, "playing")) {
		DelTimer ("russcountdown");
	}
}

#ifdef WIN32 /* temp */

int main (int argc, char **argv)
{
	return 0;
}
#endif
