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
 * Start Bomb Game
*/
int startbomb(CmdParams* cmdparams) {
	static char line[512];
	int argc;
	char **argv;
	int tuic;
	Client *u;
	Channel *c;

	strlcpy (line, cmdparams->param, 512);
	argc = split_buf (line, &argv, 0);

	if (!ircstrcasecmp (currentbombgamestatus, "stopped")) {
		c = FindChannel(argv[1]);
		if (!c) {
			irc_prefmsg (gs_bot, cmdparams->source, "You must be in the Channel you wish to start the game in.");
		} else {
			u = FindUser(cmdparams->source->name);
			tuic = IsChannelMember(c, u);
			if (tuic == 1) {					
				if ( kickgameschanoponly && !IsChanOp(c->name, u->name) ) {
					irc_prefmsg (gs_bot, cmdparams->source, "You must be a Channel Operator to start the game.");
				} else {
					bombcountdowntime = 60;
					strlcpy (bombroom, argv[1], MAXCHANLEN);
					strlcpy (bombplayernick, cmdparams->source->name, MAXNICK);
					strlcpy (currentbombgamestatus, "playing", 10);
					irc_join (gs_bot, bombroom, NULL);
					irc_cmode (gs_bot, bombroom, "+o", gs_bot->name);
					irc_chanprivmsg (gs_bot, bombroom, "\0037A Bomb has been brought into the channel by %s. Don''t be the last one with it.", cmdparams->source->name);
					AddTimer (TIMER_TYPE_COUNTDOWN, timerupstopbomb, "bombcountdown", bombcountdowntime);
				}
			} else {
				irc_prefmsg (gs_bot, cmdparams->source, "You must be in the Channel you wish to start the game in.");
			}
		}
	} else {
		irc_prefmsg (gs_bot, cmdparams->source, "The Game is already active in %s , Try Again Later.", bombroom);
	}
	ns_free (argv);
	return NS_SUCCESS;
}

/*
 * Pass Bomb
*/
int passbomb(CmdParams* cmdparams) {
	static char line[512];
	int argc;
	char **argv;
	int cmt;
	Client *u;
	Channel *c;
	int ttto;

	strlcpy (line, cmdparams->param, 512);
	argc = split_buf (line, &argv, 0);

	if ( !ircstrcasecmp (cmdparams->source->name, bombplayernick) && !ircstrcasecmp (currentbombgamestatus, "playing") ) {
		u = FindUser (argv[1]);
		if (!u) {
			stopbomb(argv[1], "noton");
		} else {
			c = FindChannel(bombroom);
			cmt = IsChannelMember(c, u);
			if (cmt == 1) {
				DelTimer ("bombcountdown");
				ttto = ( ( rand() % 5 ) + 1 );
				if ( ttto < bombcountdowntime ) {
					bombcountdowntime -= ttto;
				}
				switch (ttto) {
					case 1:
						irc_chanprivmsg (gs_bot, bombroom, "\0037%s places the bomb in %s's pocket.", bombplayernick, u->name);
						break;
					case 2:
						irc_chanprivmsg (gs_bot, bombroom, "\0037%s sneaks the bomb next to %s.", bombplayernick, u->name);
						break;
					case 3:
						irc_chanprivmsg (gs_bot, bombroom, "\0037%s throws the bomb at %s, who now has a large bruise.", bombplayernick, u->name);
						break;
					default:
						irc_chanprivmsg (gs_bot, bombroom, "\0037%s passes the Bomb to %s.", bombplayernick, u->name);
						break;
				}
				if ( IsExcluded(u) || IsMe(u) || is_bot(u) || u->user->is_away == 1 ) {
					irc_chanprivmsg (gs_bot, bombroom, "\0037the Bomb Bounces off an invisible Force Field, and returns to %s.", bombplayernick);
				} else {
					strlcpy (bombplayernick, u->name, MAXNICK);
				}
				AddTimer (TIMER_TYPE_COUNTDOWN, timerupstopbomb, "bombcountdown", bombcountdowntime);
			} else {
				stopbomb(argv[1], "notin");
			}
		}
	}
	ns_free (argv);
	return NS_SUCCESS;
}

/*
 * Stop Bomb Game
*/
void stopbomb(char *nic, char *reason) {
	Client *u;
	Channel *c;

	c = FindChannel(bombroom);
	u = FindUser(bombplayernick);

	if (!ircstrcasecmp (reason, "noton")) {
		irc_chanprivmsg (gs_bot, bombroom, "\0037%s must be blind there is no %s on the network.", bombplayernick, nic);
		DelTimer ("bombcountdown");
	} else {
		if (!ircstrcasecmp (reason, "notin")) {
			irc_chanprivmsg (gs_bot, bombroom, "\0037%s must be blind, %s isn''t in the channel.", bombplayernick, nic);
			DelTimer ("bombcountdown");
		}
	}
	irc_kick(gs_bot, bombroom, bombplayernick, "\0034BOOOOOM !!!!!!");
	irc_chanprivmsg (gs_bot, bombroom, "\0037GAME OVER");
	irc_part (gs_bot, bombroom, NULL);
	strlcpy (bombroom, "", MAXCHANLEN);
	strlcpy (bombplayernick, "", MAXNICK);
	strlcpy (currentbombgamestatus, "stopped", 10);
}

/*
 * Bomb Timer Finished
*/
int timerupstopbomb(void) {
	stopbomb( "", "");
}
