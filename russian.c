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
 * Start Russian Roulette Game
*/
int startruss(CmdParams* cmdparams) {
	Channel *c;

	if ( gamestatus[GS_GAME_RUSS] == GS_GAME_STOPPED ) {
		c = FindChannel(cmdparams->av[0]);
		if (!c) {
			irc_prefmsg (gs_bot, cmdparams->source, "You must be in the Channel you wish to start the game in.");
		} else {
			if (IsChannelMember(c, cmdparams->source)) {
				if ( kickgameschanoponly && !IsChanOp(c->name, cmdparams->source->name) ) {
					irc_prefmsg (gs_bot, cmdparams->source, "You must be a Channel Operator to start the game.");
				} else {
					countdowntime[GS_GAME_RUSS] = 60;
					strlcpy (gameroom[GS_GAME_RUSS], cmdparams->av[0], MAXCHANLEN);
					strlcpy (gameplayernick[GS_GAME_RUSS], cmdparams->source->name, MAXNICK);
					gamestatus[GS_GAME_RUSS] == GS_GAME_PLAYING;
					irc_join (gs_bot, gameroom[GS_GAME_RUSS], NULL);
					irc_cmode (gs_bot, gameroom[GS_GAME_RUSS], "+o", gs_bot->name);
					irc_chanprivmsg (gs_bot, gameroom[GS_GAME_RUSS], "\0037Russian Roulette has been started by %s. Who will die this time?", cmdparams->source->name);
					AddTimer (TIMER_TYPE_COUNTDOWN, timerupstopruss, "russcountdown", countdowntime[GS_GAME_RUSS]);
				}
			} else {
				irc_prefmsg (gs_bot, cmdparams->source, "You must be in the Channel you wish to start the game in.");
			}
		}
	} else {
		irc_prefmsg (gs_bot, cmdparams->source, "The Game is already active in %s , Try Again Later.", gameroom[GS_GAME_RUSS]);
	}
	return NS_SUCCESS;
}

/*
 * Shoot Russian Roulette
*/
int shootruss(CmdParams* cmdparams) {
	Client *u;
	Channel *c;
	int ttto;
	int rt;

	if ( !ircstrcasecmp (cmdparams->source->name, gameplayernick[GS_GAME_RUSS]) && ( gamestatus[GS_GAME_RUSS] == GS_GAME_PLAYING ) ) {
		u = FindUser (cmdparams->av[0]);
		if (!u) {
			stopruss(cmdparams->av[0], "noton");
		} else {
			if ( IsExcluded(u) || IsMe(u) || IsBot(u) || IsAway(u) ) {
				irc_chanprivmsg (gs_bot, gameroom[GS_GAME_RUSS], "\0037%s Refuses to play this game %s, try someone else.", u->name, gameplayernick[GS_GAME_RUSS]);
			} else {
				c = FindChannel(gameroom[GS_GAME_RUSS]);
				if (IsChannelMember(c, u)) {
					DelTimer ("russcountdown");
					ttto = ( ( rand() % 5 ) + 1 );
					if ( ttto < countdowntime[GS_GAME_RUSS] ) {
						countdowntime[GS_GAME_RUSS] -= ttto;
					}
					switch (ttto) {
						case 1:
							irc_chanprivmsg (gs_bot, gameroom[GS_GAME_RUSS], "\0037%s places the gun against %s's temple, and pulls the trigger.", gameplayernick[GS_GAME_RUSS], u->name);
							break;
						case 2:
							irc_chanprivmsg (gs_bot, gameroom[GS_GAME_RUSS], "\0037%s points the gun between %s's eyes.", gameplayernick[GS_GAME_RUSS], u->name);
							break;
						default:
							irc_chanprivmsg (gs_bot, gameroom[GS_GAME_RUSS], "\0037%s points the gun at %s and fires.", gameplayernick[GS_GAME_RUSS], u->name);
							break;
					}
					rt = ( rand() % 9 );
					if ( rt == 7 ) {
						irc_chanprivmsg (gs_bot, gameroom[GS_GAME_RUSS], "\0034\2BANG!\2, Nice one %s.", gameplayernick[GS_GAME_RUSS]);
						strlcpy (gameplayernick[GS_GAME_RUSS], u->name, MAXNICK);
						stopruss(cmdparams->av[0],"Shot");
					} else {
						strlcpy (gameplayernick[GS_GAME_RUSS], u->name, MAXNICK);
						irc_chanprivmsg (gs_bot, gameroom[GS_GAME_RUSS], "\0034click!, you now have the gun %s, select someone to shoot.", gameplayernick[GS_GAME_RUSS]);
						DelTimer ("russcountdown");
						AddTimer (TIMER_TYPE_COUNTDOWN, timerupstopruss, "russcountdown", countdowntime[GS_GAME_RUSS]);
					}
				} else {
					stopruss(cmdparams->av[0], "notin");
				}
			}
		}
	}
	return NS_SUCCESS;
}

/*
 * Stop Russian Roulette Game
*/
void stopruss(char *nic, char *reason) {
	char *russdiereason;

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
	irc_kick(gs_bot, gameroom[GS_GAME_RUSS], gameplayernick[GS_GAME_RUSS], russdiereason);
	irc_chanprivmsg (gs_bot, gameroom[GS_GAME_RUSS], "\0037GAME OVER");
	irc_part (gs_bot, gameroom[GS_GAME_RUSS], NULL);
	gameroom[GS_GAME_RUSS][0] = "";
	gameplayernick[GS_GAME_RUSS][0] = "";
	gamestatus[GS_GAME_RUSS] = GS_GAME_STOPPED;
}

/*
 * Russian Roulette Timer Finished
*/
int timerupstopruss(void) {
	stopruss( "", "");
	return NS_SUCCESS;
}
