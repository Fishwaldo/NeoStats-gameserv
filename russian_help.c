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
const char gs_help_russ_oneline[] = "Start Russian Roulette in a Channel";
const char gs_help_shoot_oneline[] = "Shoots the Nick Specified in the Channel";

const char *gs_help_russ[] = {
	"Syntax: \2RUSSIAN <#Channel>\2",
	"",
	"This command starts Russian Roulette in the specified Channel.",
	"",
        "The Channel Must Exist, and you must be in the Channel",
	"",
	"The person holding the Gun selects a Nick to Shoot",
	"if the chamber is empty, that nick then receives the Gun.",
	"if the chamber was full, then the Nick is kicked from the Channel.",
	"",
	"Each player in turn gets less time to Shoot another Nick",
	NULL
};

const char *gs_help_shoot[] = {
	"Syntax: \2SHOOT <nickname>\2",
	"",
	"This option will attempt to shoot the specified Nick",
	"The Nickname Must be in the channel at the time, or you lose.",
	NULL
};
