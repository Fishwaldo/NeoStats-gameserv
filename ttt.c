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
 * Start Tic Tac Toe Game
*/
int startttt(const CmdParams *cmdparams) {
	UserGameData *ugd;
	TicTacToe *tttd;
	int x, y;
	
	if (CheckUserGameStart(cmdparams->source) != NS_SUCCESS) {
		return NS_SUCCESS;
	}
	ugd = ns_calloc(sizeof(UserGameData));
	tttd = ns_calloc(sizeof(TicTacToe));
	ugd->gametype = GS_GAME_USER_TICTACTOE;
	ugd->gamedata = (char *)tttd;
	tttd->turnnum = 0;
	for ( x = 0 ; x < 3 ; x++ ) {
		for ( y = 0 ; y < 3 ; y++ ) {
			tttd->grid[x][y] = '?';
		}
	}
	SetUserModValue(cmdparams->source, ugd);
	irc_prefmsg (gs_bot, cmdparams->source, "Tic Tac Toe has started.");
	switch (rand()%2) {
		case 1:
			irc_prefmsg (gs_bot, cmdparams->source, "You get the first turn.");
			break;
		default:
			gsturn(cmdparams->source, ugd, tttd);
			break;
	}
	return NS_SUCCESS;
}

/*
 * mark position
*/
int playttt(const CmdParams *cmdparams) {
	UserGameData *ugd;
	TicTacToe *tttd;
	int x, y;

	ugd = (UserGameData *)GetUserModValue(cmdparams->source);
	if (!ugd) {
		return NS_SUCCESS;
	}
	if (ugd->gametype != GS_GAME_USER_TICTACTOE) {
		return NS_SUCCESS;
	}
	tttd = (TicTacToe *)ugd->gamedata;
	if (!tttd) {
		return NS_SUCCESS;
	}
	if (cmdparams->ac != 2) {
		irc_prefmsg (gs_bot, cmdparams->source, "Invalid Position.");
		return NS_SUCCESS;
	}
	x = atoi(cmdparams->av[0]);
	y = atoi(cmdparams->av[1]);
	if ( x < 1 || x > 3 || y < 1 || y > 3 ) {
		irc_prefmsg (gs_bot, cmdparams->source, "Invalid Position.");
		return NS_SUCCESS;
	}
	if (tttd->grid[(x-1)][(y-1)] != '?') {
		if (tttd->grid[(x-1)][(y-1)] == 'O') {
			irc_prefmsg (gs_bot, cmdparams->source, "I have already marked that position.");
		} else if (tttd->grid[(x-1)][(y-1)] == 'X') {
			irc_prefmsg (gs_bot, cmdparams->source, "You have already marked that position.");
		}
		return NS_SUCCESS;
	}
	tttd->grid[(x-1)][(y-1)] = 'X';
	tttd->turnnum++;
	if (CheckTTTWinner(cmdparams->source, ugd, tttd) != NS_SUCCESS) {
		gsturn(cmdparams->source, ugd, tttd);
	}
	return NS_SUCCESS;
}

/*
 * GamesServ Turn
*/
void gsturn(Client *u, UserGameData *ugd, TicTacToe *tttd) {
	int i, i2, x, y;
	
	if (!ugd || ! tttd) {
		return;
	}
	if (ugd->gametype != GS_GAME_USER_TICTACTOE) {
		return;
	}
	/*
	 * TODO: Get a decent system for picking pseudo clients turn
	 * currently just selects a random position
	*/
	i = ((rand() % (9 - tttd->turnnum)) + 1);
	i2 = 0;
	for ( x = 0 ; x < 3 ; x++ ) {
		for ( y = 0 ; y < 3 ; y++ ) {
			if (tttd->grid[x][y] == '?') {
				i2++;
			}
			if (i == i2) {
				break;
			}
		}
		if (i == i2) {
			break;
		}
	}
	tttd->grid[x][y] = 'O';
	irc_prefmsg (gs_bot, u, "Game Board:");
	for ( i = 0 ; i < 3 ; i++ ) {
		irc_prefmsg (gs_bot, u, " %s | %s | %s", ( tttd->grid[0][i] == 'X' ) ? "X" : ( tttd->grid[0][i] == 'O' ) ? "O" : "-", ( tttd->grid[1][i] == 'X' ) ? "X" : ( tttd->grid[1][i] == 'O' ) ? "O" : "-", ( tttd->grid[2][i] == 'X' ) ? "X" : ( tttd->grid[2][i] == 'O' ) ? "O" : "-");
		if (i < 2) {
			irc_prefmsg (gs_bot, u, " ---|---|---");
		}
	}	
	tttd->turnnum++;
	CheckTTTWinner(u, ugd, tttd);
	return;
}

/*
 * Check for Tic Tac Toe Winner
 *
 * returns NS_SUCCESS for game over, else NS_FAILURE
*/
int CheckTTTWinner(Client *u, UserGameData *ugd, TicTacToe *tttd) {
	int i, w;
	
	if (!ugd || !tttd) {
		return NS_FAILURE;
	}
	if (ugd->gametype != GS_GAME_USER_TICTACTOE) {
		return NS_FAILURE;
	}
	w = 0;
	for ( i = 0 ; i < 3 ; i++ ) {
		if (!w && tttd->grid[i][0] == tttd->grid[i][1] && tttd->grid[i][0] == tttd->grid[i][2]) {
			if (tttd->grid[i][0] == 'O') {
				w = 1;
			} else if (tttd->grid[i][0] == 'X') {
				w = 2;
			}
		}
	}
	if (!w) {
		for ( i = 0 ; i < 3 ; i++ ) {
			if (!w && tttd->grid[0][i] == tttd->grid[1][i] && tttd->grid[0][i] == tttd->grid[2][i]) {
				if (tttd->grid[i][0] == 'O') {
					w = 1;
				} else if (tttd->grid[i][0] == 'X') {
					w = 2;
				}
			}
		}
	}
	if (!w && tttd->grid[0][0] == tttd->grid[1][1] && tttd->grid[0][0] == tttd->grid[2][2]) {
		if (tttd->grid[0][0] == 'O') {
			w = 1;
		} else if (tttd->grid[0][0] == 'X') {
			w = 2;
		}
	}
	if (!w && tttd->grid[2][0] == tttd->grid[1][1] && tttd->grid[2][0] == tttd->grid[0][2]) {
		if (tttd->grid[2][0] == 'O') {
			w = 1;
		} else if (tttd->grid[2][0] == 'X') {
			w = 2;
		}
	}
	switch (w) {
		case 1:
			irc_prefmsg (gs_bot, u, "Looks like I win yet again :)");
			stopug(u);
			return NS_SUCCESS;
		case 2:
			irc_prefmsg (gs_bot, u, "You only won because I am having a bad day.");
			stopug(u);
			return NS_SUCCESS;
		default:
			break;
	}
	if (tttd->turnnum >= 8) {
		irc_prefmsg (gs_bot, u, "Looks like another Draw (Game Over)");
		stopug(u);
		return NS_SUCCESS;
	}
	return NS_FAILURE;
}
