/* GamesServ - Small Games Service - NeoStats Addon Module
** Copyright (c) 2005 DeadNotBuried
** Portions Copyright (c) 1999-2005, NeoStats,
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

Bot *gs_bot;

/*
 * Bomb Game Help
*/
extern const char gs_help_bomb_oneline[];
extern const char gs_help_throw_oneline[];
extern const char *gs_help_bomb[];
extern const char *gs_help_throw[];

/*
 * Bomb Game Procedures
*/
int startbomb (CmdParams* cmdparams);
int throwbomb (CmdParams* cmdparams);
void stopbomb(char *nic, char *reason);
int timerupstopbomb(void);


/*
 * Russian Roulette Game Help
*/
extern const char gs_help_russ_oneline[];
extern const char gs_help_shoot_oneline[];
extern const char *gs_help_russ[];
extern const char *gs_help_shoot[];

/*
 * Russian Roulette Game Procedures
*/
int startruss (CmdParams* cmdparams);
int shootruss (CmdParams* cmdparams);
void stopruss(char *nic, char *reason);
int timerupstopruss(void);


/*
 * Defines
*/
#define GS_GAME_TOTAL		0x00000002	/* Number Of Games */

#define GS_GAME_BOMB		0x00000000	/* Bomb Game */
#define GS_GAME_RUSS		0x00000001	/* Russian Roulette Game */

#define GS_GAME_STOPPED		0x00000001	/* Game Not Running */
#define GS_GAME_STARTING	0x00000002	/* Game Starting */
#define GS_GAME_PLAYING		0x00000003	/* Game Running */
#define GS_GAME_STOPPING	0x00000004	/* Game Stopping */

/*
 * Game Variables
*/
extern char gameroom[GS_GAME_TOTAL][MAXCHANLEN];
extern int gamestatus[GS_GAME_TOTAL];
extern char gameplayernick[GS_GAME_TOTAL][MAXNICK];
extern int countdowntime[GS_GAME_TOTAL];

/*
 * Common Variables
*/
extern int kickgameschanoponly;

/*
 * Common Help
*/
extern const char *gs_help_set_kickchanoponly[];

/*
 * Common Procedures
*/
int PlayerNickChange (CmdParams* cmdparams);
