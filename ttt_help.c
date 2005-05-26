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
 * Help Text
*/
const char gs_help_ttt_oneline[] = "Start a Tic-Tac-Toe Game";
const char gs_help_x_oneline[] = "Mark Tic-Tac-Toe position";

const char *gs_help_ttt[] = {
	"Syntax: \2TTT\2",
	"",
	"This command starts a game of Tic-Tac-Toe.",
	NULL
};

const char *gs_help_x[] = {
	"Syntax: \2X <x position> <y position>\2",
	"",
	"Mark the Tic-Tac-Toe grid Number as yours",
	"",
	"Grid Positions are numbered as follows (x,y):",
	"1,1 | 2,1 | 3,1",
	"---------------",
	"1,2 | 2,2 | 3,2",
	"---------------",
	"1,3 | 2,3 | 3,3",
	NULL
};
