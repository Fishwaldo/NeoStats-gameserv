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

static int num_low;
static int num_high;
static int num;
/*
 * Start HiLo Game
*/
int starthilo(CmdParams* cmdparams) {
	Channel *c;

	if (gamestatus[GS_GAME_HILO] == GS_GAME_STOPPED) {
		c = FindChannel(cmdparams->av[0]);
		if (!c) {
			irc_prefmsg (gs_bot, cmdparams->source, "You must be in the Channel you wish to start the game in.");
		} else {
			if (IsChannelMember(c, cmdparams->source)) {					
				num_low = (rand() % 999000);
				num = (num_low + (rand() % 999) + 1);
				num_high = (num_low + 1000);
				countdowntime[GS_GAME_HILO] = 120;
				strlcpy (gameroom[GS_GAME_HILO], cmdparams->av[0], MAXCHANLEN);
				strlcpy (gameplayernick[GS_GAME_HILO], cmdparams->source->name, MAXNICK);
				gamestatus[GS_GAME_HILO] = GS_GAME_PLAYING;
				irc_join (gs_bot, gameroom[GS_GAME_HILO], "+o");
				irc_chanprivmsg (gs_bot, gameroom[GS_GAME_HILO], "\0037A game of HiLo has been started by %s. Can you guess the number between %d and %d.", cmdparams->source->name, num_low, num_high);
				AddTimer (TIMER_TYPE_COUNTDOWN, timerupstophilo, "hilocountdown", countdowntime[GS_GAME_HILO]);
			} else {
				irc_prefmsg (gs_bot, cmdparams->source, "You must be in the Channel you wish to start the game in.");
			}
		}
	} else {
		irc_prefmsg (gs_bot, cmdparams->source, "The Game is already active in %s , Try Again Later.", gameroom[GS_GAME_HILO]);
	}
	return NS_SUCCESS;
}

/*
 * Guess Number
*/
int guesshilo(CmdParams* cmdparams) {
	int hlg;

	if (gamestatus[GS_GAME_HILO] == GS_GAME_PLAYING) {
		hlg = atoi(cmdparams->av[0]);
		if (hlg == num) {
			stophilo(cmdparams->source->name, hlg);
			return NS_SUCCESS;
		}
		if (hlg > num_low && hlg < num_high) {
			if (hlg < num) {
				num_low = hlg;
			} else {
				num_high = hlg;
			}
			irc_chanprivmsg (gs_bot, gameroom[GS_GAME_HILO], "\0037%s You are a bit closer with %d , number is between %d and %d.", cmdparams->source->name, hlg, num_low, num_high);
			return NS_SUCCESS;
		}
	}
	return NS_SUCCESS;
}

/*
 * Stop HiLo Game
*/
void stophilo(char *nic, int hlg) {

	if (!hlg) {
		irc_chanprivmsg (gs_bot, gameroom[GS_GAME_HILO], "\0037Times Up, it looks like your all Losers :)");
	} else {
		irc_chanprivmsg (gs_bot, gameroom[GS_GAME_HILO], "\0037%s is correct with %d and wins, the rest of you are just Losers :)", nic, hlg);
		DelTimer ("hilocountdown");
	}
	irc_chanprivmsg (gs_bot, gameroom[GS_GAME_HILO], "\0037GAME OVER");
	CheckPartGameChannel(GS_GAME_HILO);
	gameroom[GS_GAME_HILO][0] = '\0';
	gamestatus[GS_GAME_HILO] = GS_GAME_STOPPED;
}

/*
 * HiLO Timer Finished
*/
int timerupstophilo(void) {
	stophilo( "", 0);
	return NS_SUCCESS;
}
