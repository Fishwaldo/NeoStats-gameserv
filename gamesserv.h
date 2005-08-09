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

Bot *gs_bot;

/*
 * Game typedefs/structures
*/

/* Tic Tac Toe */
typedef struct TicTacToe {
	int turnnum;
	char grid[3][3];
} TicTacToe;

typedef struct UserGameData {
	int gametype;
	char *gamedata;
} UserGameData;

/*
 * Bomb Game Help
*/
extern const char *gs_help_bomb[];
extern const char *gs_help_throw[];

/*
 * Bomb Game Procedures
*/
int startbomb (CmdParams* cmdparams);
int throwbomb (CmdParams* cmdparams);

/*
 * Russian Roulette Game Help
*/
extern const char *gs_help_russ[];
extern const char *gs_help_shoot[];

/*
 * Russian Roulette Game Procedures
*/
int startruss (CmdParams* cmdparams);
int shootruss (CmdParams* cmdparams);
void stopruss(char *nic, char *reason);
int timerupstopruss(void *);

/*
 * HiLo Game Help
*/
extern const char *gs_help_hilo[];
extern const char *gs_help_guess[];

/*
 * HiLo Game Procedures
*/
int starthilo(CmdParams* cmdparams);
int guesshilo(CmdParams* cmdparams);
void stophilo(char *nic, int hlg);
int timerupstophilo(void *);

/*
 * Tic-Tac-Toe Game Help
*/
extern const char gs_help_ttt_oneline[];
extern const char gs_help_x_oneline[];
extern const char *gs_help_ttt[];
extern const char *gs_help_x[];

/*
 * Tic-Tac-Toe Game Procedures
*/
int startttt(CmdParams* cmdparams);
int playttt(CmdParams* cmdparams);
void gsturn(Client *u, UserGameData *ugd, TicTacToe *tttd);
int CheckTTTWinner(Client *u, UserGameData *ugd, TicTacToe *tttd);

/*
 * Defines
*/
#define GS_GAME_CHANNEL_TOTAL	0x00000003	/* Number Of Channel Games */

#define GS_GAME_CHANNEL_BOMB		0x00000000	/* Bomb Game */
#define GS_GAME_CHANNEL_RUSS		0x00000001	/* Russian Roulette Game */
#define GS_GAME_CHANNEL_HILO		0x00000002	/* HiLo Game */

#define GS_GAME_CHANNEL_STOPPED		0x00000001	/* Game Not Running */
#define GS_GAME_CHANNEL_STARTING	0x00000002	/* Game Starting */
#define GS_GAME_CHANNEL_PLAYING		0x00000003	/* Game Running */
#define GS_GAME_CHANNEL_STOPPING	0x00000004	/* Game Stopping */

#define GS_GAME_CHANNEL_NOJOIN		0x00000000	/* Don't Join Game Channel */
#define GS_GAME_CHANNEL_JOIN		0x00000001	/* Join Game Channel */
#define GS_GAME_CHANNEL_NOKICK		0x00000000	/* No Kicks From Channels */
#define GS_GAME_CHANNEL_KICK		0x00000001	/* Loser Kicked From Game Channel */

#define GS_GAME_USER_TICTACTOE		0x00000000	/* User Tic Tac Toe Game */

/*
 * Game Variables
*/
extern char gameroom[GS_GAME_CHANNEL_TOTAL][MAXCHANLEN];
extern int gamestatus[GS_GAME_CHANNEL_TOTAL];
extern char gameplayernick[GS_GAME_CHANNEL_TOTAL][MAXNICK];
extern int countdowntime[GS_GAME_CHANNEL_TOTAL];

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
void CheckPartGameChannel(int gr);
int CheckGameStart(Client *u, char *cn, int gn, int ct, int kg, int cj);
int CheckUserGameStart(Client *u);
void stopug(Client *u);
