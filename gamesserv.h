/* GamesServ - Small Games Service - NeoStats Addon Module
** Copyright (c) 2005 DeadNotBuried
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

Bot *gs_bot;

/*
 * Bomb Game Variables
*/
char bombroom[MAXCHANLEN];
char currentbombgamestatus[10];
char bombplayernick[MAXNICK];
int bombcountdowntime;

/*
 * Bomb Game Help
*/
extern const char gs_help_bomb_oneline[];
extern const char gs_help_pass_oneline[];
extern const char *gs_help_bomb[];
extern const char *gs_help_pass[];

/*
 * Bomb Game Procedures
*/
int startbomb (CmdParams* cmdparams);
int passbomb (CmdParams* cmdparams);
void stopbomb(char *nic, char *reason);
int timerupstopbomb(void);


/*
 * Russian Roulette Game Variables
*/
char russroom[MAXCHANLEN];
char currentrussgamestatus[10];
char russplayernick[MAXNICK];
int russcountdowntime;

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
 * Bomb Game Variables
*/
int kickgameschanoponly;

/*
 * Common Help
*/
extern const char *gs_help_set_kickchanoponly[];

/*
 * Common Procedures
*/
int PlayerNickChange (CmdParams* cmdparams);
