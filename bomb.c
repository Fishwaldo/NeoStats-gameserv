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

typedef enum STOP_REASON
{
	STOP_NONE,
	STOP_NOT_ONLINE,
	STOP_NOT_INCHANNEL
} STOP_REASON;

/* Prototypes */
static void stopbomb( char *nic, STOP_REASON reason );

/*
 * Bomb Timer Finished
*/
int timerupstopbomb(void *userptr)
{
	stopbomb( NULL, STOP_NONE );
	return NS_SUCCESS;
}

/*
 * Start Bomb Game
*/
int startbomb(const CmdParams *cmdparams) {
	if (CheckGameStart(cmdparams->source, cmdparams->av[0], GS_GAME_CHANNEL_BOMB, TS_ONE_MINUTE, NS_TRUE, NS_TRUE) != NS_SUCCESS) {
		return NS_SUCCESS;
	}
	irc_chanprivmsg (gs_bot, gameroom[GS_GAME_CHANNEL_BOMB], "\0037A Bomb has been brought into the channel by %s. Don''t be the last one with it.", cmdparams->source->name);
	AddTimer (TIMER_TYPE_COUNTDOWN, timerupstopbomb, "bombcountdown", countdowntime[GS_GAME_CHANNEL_BOMB], NULL);
	return NS_SUCCESS;
}

/*
 * Throw Bomb
*/
int throwbomb(const CmdParams *cmdparams) {
	Client *u;
	Channel *c;
	int ttto;

	if ( !ircstrcasecmp (cmdparams->source->name, gameplayernick[GS_GAME_CHANNEL_BOMB]) && ( gamestatus[GS_GAME_CHANNEL_BOMB] == GS_GAME_CHANNEL_PLAYING ) ) {
		u = FindUser (cmdparams->av[0]);
		if (!u) {
			stopbomb( cmdparams->av[0], STOP_NOT_ONLINE );
		} else {
			c = FindChannel(gameroom[GS_GAME_CHANNEL_BOMB]);
			if (IsChannelMember(c, u)) {
				ttto = ( ( rand() % 5 ) + 1 );
				if ( ttto < countdowntime[GS_GAME_CHANNEL_BOMB] ) {
					countdowntime[GS_GAME_CHANNEL_BOMB] -= ttto;
				}
				switch (ttto) {
					case 1:
						irc_chanprivmsg (gs_bot, gameroom[GS_GAME_CHANNEL_BOMB], "\0037%s places the bomb in %s's pocket.", gameplayernick[GS_GAME_CHANNEL_BOMB], u->name);
						break;
					case 2:
						irc_chanprivmsg (gs_bot, gameroom[GS_GAME_CHANNEL_BOMB], "\0037%s sneaks the bomb next to %s.", gameplayernick[GS_GAME_CHANNEL_BOMB], u->name);
						break;
					case 3:
						irc_chanprivmsg (gs_bot, gameroom[GS_GAME_CHANNEL_BOMB], "\0037%s throws the bomb at %s, who now has a large bruise.", gameplayernick[GS_GAME_CHANNEL_BOMB], u->name);
						break;
					default:
						irc_chanprivmsg (gs_bot, gameroom[GS_GAME_CHANNEL_BOMB], "\0037%s passes the Bomb to %s.", gameplayernick[GS_GAME_CHANNEL_BOMB], u->name);
						break;
				}
				if ( IsExcluded(u) || IsMe(u) || IsBot(u) || IsAway(u) ) {
					irc_chanprivmsg (gs_bot, gameroom[GS_GAME_CHANNEL_BOMB], "\0037the Bomb Bounces off an invisible Force Field, and returns to %s.", gameplayernick[GS_GAME_CHANNEL_BOMB]);
				} else {
					strlcpy (gameplayernick[GS_GAME_CHANNEL_BOMB], u->name, MAXNICK);
				}
				SetTimerInterval("bombcountdown", countdowntime[GS_GAME_CHANNEL_BOMB]);
			} else {
				stopbomb( cmdparams->av[0], STOP_NOT_INCHANNEL );
			}
		}
	}
	return NS_SUCCESS;
}

/*
 * Stop Bomb Game
*/
static void stopbomb( char *nic, STOP_REASON reason )
{
	switch( reason )
	{
		case STOP_NOT_ONLINE:
			irc_chanprivmsg (gs_bot, gameroom[GS_GAME_CHANNEL_BOMB], "\0037%s must be blind there is no %s on the network.", gameplayernick[GS_GAME_CHANNEL_BOMB], nic);
			DelTimer ("bombcountdown");
			break;
		case STOP_NOT_INCHANNEL:
			irc_chanprivmsg (gs_bot, gameroom[GS_GAME_CHANNEL_BOMB], "\0037%s must be blind, %s isn''t in the channel.", gameplayernick[GS_GAME_CHANNEL_BOMB], nic);
			DelTimer ("bombcountdown");
			break;
		default:
			break;
	}
	irc_kick(gs_bot, gameroom[GS_GAME_CHANNEL_BOMB], gameplayernick[GS_GAME_CHANNEL_BOMB], "\0034BOOOOOM !!!!!!");
	CheckPartGameChannel(GS_GAME_CHANNEL_BOMB);
}

