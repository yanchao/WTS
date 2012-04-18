/* telephone.c - telephone game simulator */

/* Copyright 2000 Wind River Systems, Inc. */
#include "copyright_wrs.h"

/*
DESCRIPTION
This module implements a simulator for the game of "telephone," in which
a message is passed from player to player in a circle, and is distorted
along the way.

The routine startTelephone () launches the simulation.  The simulation
proceeds as follows:

    (1) The operator task sends a predefined message to the first
	player task, then blocks on a binary semaphore.

    (2) The first player task receives the message, distorts it by
	swapping the positions of two words chosen at random, then
	passes the resulting message to the second player task.

    (3) The second player task repeats this process, distorting the
	message and passing it along to the third player task; this
	continues until the last player task receives the message.

    (4) When the last player task receives the message, it distorts
	it, copies the result into a buffer, and gives the semaphore
	on which the operator task is blocked.

    (5) The operator task unblocks, then starts another round by
	sending the predefined message to the first player task.
	The simulation proceeds in this manner until the routine
	stopTelephone () is invoked to halt the simulation.

The following diagram illustrates the flow of data among the various
tasks involved in the simulation:


		           ----------------
		           | operatorTask |    1. msgQSend()
		           |		  |    2. semBTake()
		           ================
		       ___________|___________
                      /                       \ 
                      ^                       | lineQs[0]
                      |                       | 
                      |                       V
        /--------------\  3. semBGive()      /-----------\
        | tPlayer(n)   |  2. msgDistort()    | tPlayer1  |  1. msgQReceive()
        | (lastPlayer) |  1. msgQReceive()   |           |  2. msgDistort()
        ================                     =============  3. msgQSend()
                      ^                       | 
                      |                       | 
                      |   lineQs[n-1]         | lineQs[1]
                      |                       | 
                      |                       V
        /--------------\  3. msgQSend()      /-----------\
        | tPlayer(n-1) |  2. msgDistort()    | tPlayer1  |  1. msgQReceive()
        |              |  1. msgQReceive()   |           |  2. msgDistort()
        ================                     =============  3. msgQSend()
                      ^                       | 
                      |                       | 
                      |   lineQs[n-2]         | lineQs[2]
                      |                       | 
                      |                       V
		      \<----------------------/

*/


/* includes */

#include "vxWorks.h"
#include "msgQLib.h"
#include "private/msgQLibP.h"
#include "taskLib.h"
#include "semLib.h"
#include "string.h"
#include "stdio.h"


/* defines */
 
#if TRUE
#define STATUS_INFO                     /* if defined, print status messages */
#endif

#define MAX_PLAYERS	10		/* max number of players in game */
#define MAX_MSG_LEN	100		/* max length of message */
#define MAX_NUM_WORD	20		/* max number of words in message */

#define MSG_TEXT 	"WindRiver Systems - how smart things think"

#define PLAYER_PRI	200		/* player task priority */
#define PLAYER_STK	10000		/* player task stack size */
#define PLAYER_OPT	0		/* player task option word */

#define OPERATOR_PRI	150		/* operator task priority */
#define OPERATOR_STK	10000		/* operator task stack size */

#ifdef	  STATUS_INFO
#define OPERATOR_OPT	VX_STDIO	/* operator task option word */
#else  /* STATUS_INFO */
#define OPERATOR_OPT	0
#endif /* STATUS_INFO */


/* globals */

MSG_Q_ID lineQs[MAX_PLAYERS];		/* message queues as phone lines */
SEM_ID   operatorSem;			/* semaphore ID */
char     result[MAX_MSG_LEN];		/* char array for resulted message */
int	 playerId[MAX_PLAYERS];		/* int array for player task IDs */
int	 lastPlayerId;			/* last player task ID */
int	 operatorId;			/* operator task ID */


/* forward declarations */

STATUS	 startTelephone (int nPlayers);
STATUS	 stopTelephone (void);
STATUS	 linesCreate (int nPlayers);
STATUS	 playersCreate (int nPlayers);
STATUS	 operatorCreate (void);
STATUS	 msgDistort (char *msg);
void	 playerTask (int inLineIx, int outLineIx);
void	 operatorTask (void);


/*******************************************************************************
*
* startTelephone - start a telephone game simulation
*
* This function creates the tasks and IPC objects used in the telephone
* simulation, starts the simulation in motion, and then exits.
* 
* RETURNS: OK, or ERROR if simulation startup failed.
*/

STATUS startTelephone
    (
    int		nPlayers	/* number of players in game */
    )

    {

    /* validate player count */

    if (nPlayers < 1 || nPlayers > MAX_PLAYERS)
	return (ERROR);

    /* create telephone lines */

    if (linesCreate (nPlayers) == ERROR)
	return (ERROR);

    /* create players */

    if (playersCreate (nPlayers) == ERROR)
	return (ERROR);

    /* create telephone operator */

    if (operatorCreate () == ERROR)
	return (ERROR);

    /* let it rip */

    return (OK);
    }

/*******************************************************************************
*
* linesCreate - create "telephone lines" (message queues)
*
* This function creates the message queues used to simulate the telephone
* lines connecting the players.
*
* RETURNS: OK, or ERROR if message queues couldn't be created.
*/

STATUS linesCreate
    (
    int		nLines		/* number of lines to create */
    )

    {
    int ix;
    MSG_Q_ID msgQId;

    for (ix = 0; ix < nLines; ix++)
	{
	msgQId = msgQCreate (1, MAX_MSG_LEN, MSG_Q_PRIORITY);
	if (msgQId == NULL)
	    return (ERROR);

	lineQs[ix] = msgQId;	/* store the message queue ID */
	}
    
    return (OK);
    }

/*******************************************************************************
*
* playersCreate - create telephone game players
*
* This function creates the tasks used to simulate the players of the
* telephone game.  The task IDs of the player tasks are saved in the
* array playerId[], and the ID of the last player task is also saved in
* lastPlayerId.
*
* RETURNS: OK, or ERROR if player tasks couldn't be created.
*/

STATUS playersCreate
    (
    int		nPlayers	/* number of players to create */
    )

    {
    int  ix;
    int  outQIx;
    char nameBuf[sizeof ("tPlayerN") + ((MAX_PLAYERS - 1) / 10)]; 

    /* clear the playerId[] array */

    for (ix = 0; ix < MAX_PLAYERS; ix++)
	playerId[ix] = 0;

    /* create player tasks */

    for (ix = 0; ix < nPlayers; ix++)
	{
	sprintf (nameBuf, "tPlayer%d", ix + 1);
	outQIx = (ix + 1) % nPlayers;		/* player's output msg Q */

	playerId[ix] = taskSpawn (nameBuf, PLAYER_PRI, PLAYER_OPT, PLAYER_STK,
			          (FUNCPTR) playerTask, ix, outQIx,
			    	  0, 0, 0, 0, 0, 0, 0, 0);

	if (playerId[ix] == ERROR)
	    return(ERROR);
	}

    lastPlayerId = playerId[ix-1];
    return (OK);
    }

/*******************************************************************************
*
* operatorCreate - create telephone operator
*
* This function creates the operator task, as well as a binary semaphore
* used to signal the completion of one round of the simulation.
*
* RETURNS: OK, or ERROR if the operator task or result semaphore couldn't
* be created.
*/

STATUS operatorCreate (void)

    {

    /* create semaphore to guard result */

    operatorSem = semBCreate (SEM_Q_PRIORITY, SEM_EMPTY);
    if (operatorSem == NULL)
	return (ERROR);

    operatorId = taskSpawn ("tOperator", OPERATOR_PRI, OPERATOR_OPT, 
			    OPERATOR_STK, (FUNCPTR) operatorTask, 
			    0, 0, 0, 0, 0, 0, 0, 0, 0, 0);

    return ((operatorId == ERROR) ? ERROR : OK);
    }

/*******************************************************************************
*
* playerTask - telephone player task
*
* This function simulates a single player in the telephone game.  It receives
* a message from the predecessor player task and passes it along, after
* random distortion, to the successor task--unless this task represents the
* last player in the circle, in which case the distorted message is copied
* into a buffer, and a semaphore is given to unblock the operator task.
*
* RETURNS: N/A
*/

void playerTask
    (
    int		inLineIx,	/* index into lineQs[] for input msgQ */
    int		outLineIx	/* index into lineQs[] for output msgQ */
    )

    {
    char	msgBuf[MAX_MSG_LEN];

    FOREVER
	{
	if (msgQReceive (lineQs[inLineIx], msgBuf, (UINT) sizeof (msgBuf),
			 WAIT_FOREVER) == ERROR)
	    break;

#ifdef  STATUS_INFO
	printf ("tPlayer%d's input:\t%s\n", inLineIx+1, msgBuf);
#endif /* STATUS_INFO */

	msgDistort (msgBuf);		/* distort the input message */

#ifdef	STATUS_INFO
	printf ("           output:\t%s\n", msgBuf);
#endif /* STATUS_INFO */

	/* If this is the last player, copy message to result and give
	 * the semaphore on which the operator task is blocked; otherwise,
	 * send the message to the next player task in the circle.
	 */

	if (taskIdSelf () == lastPlayerId)
	    {
	    strcpy (result, msgBuf);
	    if (semGive (operatorSem) == ERROR)
		break;
	    }
	else
	    if (msgQSend (lineQs[outLineIx], msgBuf, strlen (msgBuf) + 1,
			  WAIT_FOREVER, MSG_PRI_NORMAL) == ERROR)
		break;
	taskDelay(3*60);
	}

    /* if we get here, something has gone wrong */

    taskSuspend (0);
    }

/*******************************************************************************
*
* operatorTask - telephone operator task
*
* This function simulates the operator in the telephone game.  It sends a
* message to the first player, then takes a semaphore which will be given
* by the last player when the message has passed fully through the circle
* of player tasks.
*
* RETURNS: N/A
*/

void operatorTask (void)

    {

    FOREVER
	{
	if (msgQSend (lineQs[0], MSG_TEXT, sizeof (MSG_TEXT),
		      WAIT_FOREVER, MSG_PRI_NORMAL) == ERROR)
	    break;

	if (semTake (operatorSem, WAIT_FOREVER) == ERROR)
	    break;

#ifdef   STATUS_INFO
	printf ("Message after passing through all players:\n\t\t%s\n", result);
#endif /* STATUS_INFO */
	}

    /* if we get here, something has gone wrong */

    taskSuspend (0);
    }

/*******************************************************************************
*
* msgDistort - distort a message by swapping two words randomly
*
* This function is called by playerTask () to distort a message.  This is
* done by swapping the positions of two words chosen at random.
*
* RETURNS: OK, or ERROR if malloc () of buffer space fails.
*/

STATUS msgDistort
    (
    char *	msg		/* message text to be distorted in place */
    )

    {
    char *	msgCopy;
    char *	pMsgCopy;
    char *	pNextWord;
    char **	ppLast = NULL;
    char *	words[MAX_NUM_WORD];
    int		numWords;
    int		random1;
    int		random2;
    int		ix;

    /* Break the message into a sequence of strings containing a single
     * word each.  Load pointers to the word strings into words[].
     */

    msgCopy = (char *) malloc (strlen (msg) + 1);
    if (msgCopy == NULL)
	return (ERROR);

    strcpy (msgCopy, msg);
    pMsgCopy = msgCopy;
    numWords = 0;
    pNextWord = pMsgCopy;

    while ((pNextWord != NULL) && (numWords < MAX_NUM_WORD))
    {
	pNextWord = strtok_r (pMsgCopy, " ", ppLast);
	pMsgCopy = NULL;
	words[numWords++] = pNextWord;
    }
    numWords--;
	
    /* Select two words at random, and swap them.  */
    random1 = rand () % (numWords);
    do {
	random2 = rand () % (numWords);
    } while ((random2 == random1) && (numWords > 2));

    pMsgCopy = words[random2];
    words[random2] = words[random1];
    words[random1] = pMsgCopy;

    /* Load distorted message into message buffer.  */

    *msg = NULL;
    for (ix = 0; ix < numWords; ix ++)
    {
	strcat (msg, words[ix]);
	if (ix < numWords - 1)
	    strcat (msg, " ");
    }

    /* free the memory allocated for message copy */

    free (msgCopy);		

    return (OK);
    }

/*******************************************************************************
*
* stopTelephone - stop the telephone game
*
* This function deletes player and operator tasks.
*
* RETURNS: OK, or ERROR if tasks couldn't be deleted.
*/

STATUS stopTelephone (void)

    {
    int		ix;

    /* Delete all player tasks.  */

    for (ix = 0; ix < MAX_PLAYERS && playerId[ix]; ix++)
	{
	if (taskDelete (playerId[ix]) == ERROR)
	    return (ERROR);
	}

    /* Delete operator task.  */

    return (taskDelete (operatorId));
    }
