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
 * Nick Change Check
*/
int PlayerNickChange (const CmdParams *cmdparams)
{
	int i;

	for (i = 0; i < GS_GAME_CHANNEL_TOTAL; i++) {
		if (!ircstrcasecmp (gameplayernick[i], cmdparams->param)) {
			strlcpy (gameplayernick[i], cmdparams->source->name, MAXNICK);
		}
	}
	return NS_SUCCESS;
}

/*
 * Check and part Game channel if no games in channel
*/
void CheckPartGameChannel(int gr) {
	int i;

	irc_chanprivmsg (gs_bot, gameroom[gr], "\0037GAME OVER");
	for (i = 0 ; i < GS_GAME_CHANNEL_TOTAL ; i++) {
		if (i != gr && gamestatus[i] != GS_GAME_CHANNEL_STOPPED && !ircstrcasecmp (gameroom[i], gameroom[gr])) {
			i = (GS_GAME_CHANNEL_TOTAL + 1);
			break;
		}
	}
	if (i <= GS_GAME_CHANNEL_TOTAL) {
		irc_part (gs_bot, gameroom[gr], NULL);
	}
	gameroom[gr][0] = '\0';
	gameplayernick[gr][0] = '\0';
	gamestatus[gr] = GS_GAME_CHANNEL_STOPPED;
	return;
}

/*
 * Check Game not already running,
 * and channel allowed for game,
 * and join channel if needed.
*/
int CheckGameStart(Client *u, char *cn, int gn, int ct, int kg, int cj) {
	Channel *c;
	int i;
	
	if (gamestatus[gn] != GS_GAME_CHANNEL_STOPPED) {
		irc_prefmsg (gs_bot, u, "The Game is already active in %s , Try Again Later.", gameroom[gn]);
		return NS_FAILURE;
	}
	c = FindChannel(cn);
	if (!c) {
		irc_prefmsg (gs_bot, u, "You must be in the Channel you wish to start the game in.");
		return NS_FAILURE;
	}
	if (!IsChannelMember(c, u)) {
		irc_prefmsg (gs_bot, u, "You must be in the Channel you wish to start the game in.");
		return NS_FAILURE;
	}
	if (kickgameschanoponly && kg == NS_TRUE && !IsChanOp(c, u)) {
		irc_prefmsg (gs_bot, u, "You must be a Channel Operator to start the game.");
		return NS_FAILURE;
	}
	if (cj == NS_TRUE) {
		for (i = 0 ; i < GS_GAME_CHANNEL_TOTAL ; i++) {
			if (i != gn && gamestatus[i] != GS_GAME_CHANNEL_STOPPED && !ircstrcasecmp (gameroom[i], gameroom[gn])) {
				i = (GS_GAME_CHANNEL_TOTAL + 1);
				break;
			}
		}
		if (i <= GS_GAME_CHANNEL_TOTAL) {
			irc_join (gs_bot, cn, "+o");
		}
	}
	countdowntime[gn] = ct;
	strlcpy (gameroom[gn], cn, MAXCHANLEN);
	strlcpy (gameplayernick[gn], u->name, MAXNICK);
	gamestatus[gn] = GS_GAME_CHANNEL_PLAYING;
	return NS_SUCCESS;
}

/*
 * Ensure User not already playing a User Game
*/
int CheckUserGameStart(Client *u) {
	UserGameData *ugd;
	
	ugd = (UserGameData *)GetUserModValue(u);
	if (!ugd) {
		return NS_SUCCESS;
	}
	if (ugd->gametype == GS_GAME_USER_TICTACTOE) {
		irc_prefmsg (gs_bot, u, "Unable to start another game, You are already playing Tic Tac Toe.");
	} else {
		irc_prefmsg (gs_bot, u, "Unable to start game, Unknown Error.");
		stopug(u);
	}
	return NS_FAILURE;
}

/*
 * Stop Current User Game, free Game Data and Mod Data
*/
void stopug(Client *u) {
	UserGameData	*ugd;
	
	ugd = (UserGameData *)GetUserModValue(u);
	if (!ugd) {
		return;
	}
	ns_free(ugd->gamedata);
	ns_free(ugd);
	ClearUserModValue(u);
	return;
}
