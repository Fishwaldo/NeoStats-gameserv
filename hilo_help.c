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
const char *gs_help_hilo[] = {
	"Start a HiLo Game in a Channel",
	"Syntax: \2HILO <#Channel>\2",
	"",
	"This command starts the HiLo Game in the specified Channel.",
	"",
        "The Channel Must Exist, and you must be in the Channel",
	NULL
};

const char *gs_help_guess[] = {
	"Takes a Guess of the HiLo number",
	"Syntax: \2GUESS <number>\2",
	"",
	"Take a guess at what the HiLo number is",
	NULL
};
