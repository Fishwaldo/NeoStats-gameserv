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
 * Help Text
*/
const char gs_help_bomb_oneline[] = "Start a Bomb Game in a Channel";
const char gs_help_pass_oneline[] = "Pass the Bomb to another Nick in the Channel";

const char *gs_help_bomb[] = {
	"Syntax: \2BOMB <#Channel>\2",
	"",
	"This command starts the Bomb Game in the specified Channel.",
	"",
        "The Channel Must Exist, and you must be in the Channel",
	"",
	"The person holding the Bomb when it explodes is kicked from the Channel",
	"",
	"Each player in turn gets less time to pass the Bomb to another Nick",
	NULL
};

const char *gs_help_pass[] = {
	"Syntax: \2pass <nickname>\2",
	"",
	"This option will pass the bomb to the specified Nickname",
	"The Nickname Must be in the channel at the time, or you lose.",
	NULL
};
