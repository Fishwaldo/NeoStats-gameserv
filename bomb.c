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
 * Start Bomb Game
*/
int startbomb(CmdParams* cmdparams) {
	Channel *c;

	if (gamestatus[GS_GAME_BOMB] == GS_GAME_STOPPED) {
		c = FindChannel(cmdparams->av[0]);
		if (!c) {
			irc_prefmsg (gs_bot, cmdparams->source, "You must be in the Channel you wish to start the game in.");
		} else {
			if (IsChannelMember(c, cmdparams->source)) {					
				if ( kickgameschanoponly && !IsChanOp(c->name, cmdparams->source->name) ) {
					irc_prefmsg (gs_bot, cmdparams->source, "You must be a Channel Operator to start the game.");
				} else {
					countdowntime[GS_GAME_BOMB] = 60;
					strlcpy (gameroom[GS_GAME_BOMB], cmdparams->av[0], MAXCHANLEN);
					strlcpy (gameplayernick[GS_GAME_BOMB], cmdparams->source->name, MAXNICK);
					gamestatus[GS_GAME_BOMB] = GS_GAME_PLAYING;
					irc_join (gs_bot, gameroom[GS_GAME_BOMB], "+o");
					irc_chanprivmsg (gs_bot, gameroom[GS_GAME_BOMB], "\0037A Bomb has been brought into the channel by %s. Don''t be the last one with it.", cmdparams->source->name);
					AddTimer (TIMER_TYPE_COUNTDOWN, timerupstopbomb, "bombcountdown", countdowntime[GS_GAME_BOMB]);
				}
			} else {
				irc_prefmsg (gs_bot, cmdparams->source, "You must be in the Channel you wish to start the game in.");
			}
		}
	} else {
		irc_prefmsg (gs_bot, cmdparams->source, "The Game is already active in %s , Try Again Later.", gameroom[GS_GAME_BOMB]);
	}
	return NS_SUCCESS;
}

/*
 * Pass Bomb
*/
int passbomb(CmdParams* cmdparams) {
	Client *u;
	Channel *c;
	int ttto;

	if ( !ircstrcasecmp (cmdparams->source->name, gameplayernick[GS_GAME_BOMB]) && ( gamestatus[GS_GAME_BOMB] == GS_GAME_PLAYING ) ) {
		u = FindUser (cmdparams->av[0]);
		if (!u) {
			stopbomb(cmdparams->av[0], "noton");
		} else {
			c = FindChannel(gameroom[GS_GAME_BOMB]);
			if (IsChannelMember(c, u)) {
				ttto = ( ( rand() % 5 ) + 1 );
				if ( ttto < countdowntime[GS_GAME_BOMB] ) {
					countdowntime[GS_GAME_BOMB] -= ttto;
				}
				switch (ttto) {
					case 1:
						irc_chanprivmsg (gs_bot, gameroom[GS_GAME_BOMB], "\0037%s places the bomb in %s's pocket.", gameplayernick[GS_GAME_BOMB], u->name);
						break;
					case 2:
						irc_chanprivmsg (gs_bot, gameroom[GS_GAME_BOMB], "\0037%s sneaks the bomb next to %s.", gameplayernick[GS_GAME_BOMB], u->name);
						break;
					case 3:
						irc_chanprivmsg (gs_bot, gameroom[GS_GAME_BOMB], "\0037%s throws the bomb at %s, who now has a large bruise.", gameplayernick[GS_GAME_BOMB], u->name);
						break;
					default:
						irc_chanprivmsg (gs_bot, gameroom[GS_GAME_BOMB], "\0037%s passes the Bomb to %s.", gameplayernick[GS_GAME_BOMB], u->name);
						break;
				}
				if ( IsExcluded(u) || IsMe(u) || IsBot(u) || IsAway(u) ) {
					irc_chanprivmsg (gs_bot, gameroom[GS_GAME_BOMB], "\0037the Bomb Bounces off an invisible Force Field, and returns to %s.", gameplayernick[GS_GAME_BOMB]);
				} else {
					strlcpy (gameplayernick[GS_GAME_BOMB], u->name, MAXNICK);
				}
				SetTimerInterval("bombcountdown", countdowntime[GS_GAME_BOMB]);
			} else {
				stopbomb(cmdparams->av[0], "notin");
			}
		}
	}
	return NS_SUCCESS;
}

/*
 * Stop Bomb Game
*/
void stopbomb(char *nic, char *reason) {

	if (!ircstrcasecmp (reason, "noton")) {
		irc_chanprivmsg (gs_bot, gameroom[GS_GAME_BOMB], "\0037%s must be blind there is no %s on the network.", gameplayernick[GS_GAME_BOMB], nic);
		DelTimer ("bombcountdown");
	} else {
		if (!ircstrcasecmp (reason, "notin")) {
			irc_chanprivmsg (gs_bot, gameroom[GS_GAME_BOMB], "\0037%s must be blind, %s isn''t in the channel.", gameplayernick[GS_GAME_BOMB], nic);
			DelTimer ("bombcountdown");
		}
	}
	irc_kick(gs_bot, gameroom[GS_GAME_BOMB], gameplayernick[GS_GAME_BOMB], "\0034BOOOOOM !!!!!!");
	irc_chanprivmsg (gs_bot, gameroom[GS_GAME_BOMB], "\0037GAME OVER");
	irc_part (gs_bot, gameroom[GS_GAME_BOMB], NULL);
	gameroom[GS_GAME_BOMB][0] = "";
	gameplayernick[GS_GAME_BOMB][0] = "";
	gamestatus[GS_GAME_BOMB] = GS_GAME_STOPPED;
}

/*
 * Bomb Timer Finished
*/
int timerupstopbomb(void) {
	stopbomb( "", "");
	return NS_SUCCESS;
}
