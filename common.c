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
 * Check and part Game channel if no games in channel
*/
void CheckPartGameChannel(int gr) {
	int i;

	for (i = 0 ; i < GS_GAME_TOTAL ; i++)
		if (!ircstrcasecmp (gameroom[i], gameroom[gr]) && i != gr) {
			i = -1;
			break;
		}
	if (i != -1) {
		irc_part (gs_bot, gameroom[gr], NULL);
	}
}
