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
 * Start Russian Roulette Game
*/
int startruss(CmdParams* cmdparams) {
	static char line[512];
	int argc;
	char **argv;
	int tuic;
	Client *u;
	Channel *c;

	strlcpy (line, cmdparams->param, 512);
	argc = split_buf (line, &argv, 0);

	if (!ircstrcasecmp (currentrussgamestatus, "stopped")) {
		c = FindChannel(argv[1]);
		if (!c) {
			irc_prefmsg (gs_bot, cmdparams->source, "You must be in the Channel you wish to start the game in.");
		} else {
			u = FindUser(cmdparams->source->name);
			tuic = IsChannelMember(c, u);
			if (tuic == 1) {					
				russcountdowntime = 60;
				strlcpy (russroom, argv[1], MAXCHANLEN);
				strlcpy (russplayernick, cmdparams->source->name, MAXNICK);
				strlcpy (currentrussgamestatus, "playing", 10);
				irc_join (gs_bot, russroom, NULL);
				irc_cmode (gs_bot, russroom, "+o", gs_bot->name);
				irc_chanprivmsg (gs_bot, russroom, "\0037Russian Roulette has been started by %s. Who will die this time?", cmdparams->source->name);
				AddTimer (TIMER_TYPE_COUNTDOWN, timerupstopruss, "russcountdown", russcountdowntime);
			} else {
				irc_prefmsg (gs_bot, cmdparams->source, "You must be in the Channel you wish to start the game in.");
			}
		}
	} else {
		irc_prefmsg (gs_bot, cmdparams->source, "The Game is already active in %s , Try Again Later.", russroom);
	}
	ns_free (argv);
	return NS_SUCCESS;
}

/*
 * Shoot Russian Roulette
*/
int shootruss(CmdParams* cmdparams) {
	static char line[512];
	int argc;
	char **argv;
	int cmt;
	Client *u;
	Channel *c;
	int ttto;
	int rt1;

	strlcpy (line, cmdparams->param, 512);
	argc = split_buf (line, &argv, 0);

	if (!ircstrcasecmp (cmdparams->source->name, russplayernick)) {
		u = FindUser (argv[1]);
		if (!u) {
			stopruss(argv[1], "noton");
		} else {
			if (IsExcluded(u) || IsMe(u)) {
				irc_chanprivmsg (gs_bot, russroom, "\0037%s Refuses to play this game %s, try someone else.", u->name, russplayernick);
			} else {
				c = FindChannel(russroom);
				cmt = IsChannelMember(c, u);
				if (cmt == 1) {
					DelTimer ("russcountdown");
					ttto = ( ( rand() % 5 ) + 1 );
					if ( ttto < russcountdowntime ) {
						russcountdowntime -= ttto;
					}
					switch (ttto) {
						case 1:
							irc_chanprivmsg (gs_bot, russroom, "\0037%s places the gun against %s's temple, and pulls the trigger.", russplayernick, u->name);
							break;
						case 2:
							irc_chanprivmsg (gs_bot, russroom, "\0037%s points the gun between %s's eyes.", russplayernick, u->name);
							break;
						default:
							irc_chanprivmsg (gs_bot, russroom, "\0037%s points the gun at %s and fires.", russplayernick, u->name);
							break;
					}
					rt1 = ( rand() % 6 );
					if ( rt1 == 5 ) {
						irc_chanprivmsg (gs_bot, russroom, "\0034\2BANG!\2, Nice one %s.", russplayernick);
						strlcpy (russplayernick, u->name, MAXNICK);
						stopruss(argv[1],"Shot");
					} else {
						strlcpy (russplayernick, u->name, MAXNICK);
						irc_chanprivmsg (gs_bot, russroom, "\0034click!, you now have the gun %s, select someone to shoot.", russplayernick);
						DelTimer ("russcountdown");
						AddTimer (TIMER_TYPE_COUNTDOWN, timerupstopruss, "russcountdown", russcountdowntime);
					}
				} else {
					stopruss(argv[1], "notin");
				}
			}
		}
	}
	ns_free (argv);
	return NS_SUCCESS;
}

/*
 * Stop Russian Roulette Game
*/
void stopruss(char *nic, char *reason) {
	Client *u;
	Channel *c;
	char *russdiereason;

	c = FindChannel(russroom);
	u = FindUser(russplayernick);

	if (!ircstrcasecmp (reason, "noton")) {
		russdiereason = "\0034Pick someone thats on the Network next time.";
		DelTimer ("russcountdown");
	} else {
		if (!ircstrcasecmp (reason, "notin")) {
			russdiereason = "\0034Pick someone thats in the Channel next time.";
			DelTimer ("russcountdown");
		} else {
			if (!ircstrcasecmp (reason, "")) {
				russdiereason = "\0034Use The Gun Luke!";
			} else {
				russdiereason = "\0034Ducking might me an idea, You would have less holes in the head then.";
				DelTimer ("russcountdown");
			}
		}
	}
	irc_kick(gs_bot, russroom, russplayernick, russdiereason);
	irc_chanprivmsg (gs_bot, russroom, "\0037GAME OVER");
	irc_part (gs_bot, russroom, NULL);
	strlcpy (currentrussgamestatus, "stopped", 10);
}

/*
 * Russian Roulette Timer Finished
*/
int timerupstopruss(void) {
	stopruss( "", "");
}
