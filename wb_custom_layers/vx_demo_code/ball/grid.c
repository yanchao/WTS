/* grid.c - Wind River Bouncing Balls C Sample Program */

/* Grid, list, and wall functionality. */

/* includes */

#include "ball.h"

/* C "objects" */

/****************************************************************
 * Grid
 *
 * The grid consists of two 2D arrays: "grid", which shows the
 * walls and all balls, and "balls" which is a array of lists,
 * each list representing the balls at that position.
 *
 * Note that the arrays are each HEIGHT x WIDTH; that is, an array
 * of rows, each element of which is a column, and so references
 * are of the form grid[y][x] and balls[y][x].  */

char    grid [HEIGHT][WIDTH];   /* characters representing the balls */
LIST    balls[HEIGHT][WIDTH];   /* list of balls at each position */


/*
 * gridInit - initialize grid ball characters and lists and walls
 *
 * RETURNS: nothing.
 */


void gridInit (void)
    {
    int r, c;                   /* row and column indices */
    
    for (r = 0; r < HEIGHT; r++)
        {
        for (c = 0; c < WIDTH; c++)
            {
            grid[r][c] =  ' ';
            listInit (balls[r][c]);
            }
        }                              
    for (r = 0; r < HEIGHT; r++)
        {
        wallXNew (pointNew (0, r));
        wallXNew (pointNew (WIDTH - 1, r));
        }                              
    for (c = 0; c < WIDTH; c++)
        {
        wallYNew (pointNew (c, 0));
        wallYNew (pointNew (c, HEIGHT - 1));
        }
    } 

 
/*
 * gridAddBall - add a given ball to the grid at a given point
 *
 * RETURNS: nothing.
 */

void gridAddBall (BALL * pBall, POINT point)
    {
    grid [point.y][point.x] = pBall->pBallShow ();
    listAddBall (& balls [point.y][point.x], pBall);
    }


/*
 * gridDeleteBall - delete a given ball at a given grid point
 *
 * Remove the ball from the list at the grid point and replace it
 * with either the next ball on the list at that point or a space
 * character if there are none; the ball is not destroyed.
 *
 * RETURNS: nothing.  */

void gridDeleteBall (BALL * pBall, POINT point)
    {
    listRemoveBall (& balls [point.y][point.x], pBall);
    if (pListFirstBall (balls[point.y][point.x]) == 0)
        {
        grid [point.y][point.x] = ' ';
        }
    else
        {
        grid [point.y][point.x] =
            (pListFirstBall (balls[point.y][point.x])->pBallShow)();
        }
    }


/*
 * pGridBall - return the first ball at a given grid point
 *
 * RETURNS: pointer to ball.
 */

BALL * pGridBall (POINT point)
    {
    return pListFirstBall (balls[point.y][point.x]);
    }


/*
 * gridIsBallAt - return non-zero if balls exist at a given point
 *
 * RETURNS: 0 if no balls are at the point, non-zero if there are
 */

BOOL gridIsBallAt (POINT point)
    {
    return pListFirstBall (balls[point.y][point.x]) != 0;
    }


/****************************************************************
 *
 * LIST object functionality
 */

/* 
 * listNew - return an initialized list
 *
 * RETURNS: a LIST structure (see comment in DESCRIPTION section
 *          of main.c regarding passing and returning structures).
 */

LIST listNew (void)
    {
    LIST    list;

    listInit (list);
    return list;
    }


/*
 * listInit - initialize a list
 *
 * RETURNS: none.
 */

void listInit (LIST list)
    {
    list.head = list.last = 0;
    }


/*
 * pListFirstBall - return a pointer to the first ball on a given
 *                  list
 *
 * RETURNS: a pointer to a BALL structure. May be NULL.
 */

BALL * pListFirstBall (LIST list)
    {
    return list.head;
    }               


/*
 * pListLastBall - return a pointer to the last ball on a given 
 *                 list
 *
 * RETURNS: a pointer to a BALL structure. May be NULL.
 */

BALL * pListLastBall (LIST list)
    {
    return list.last;
    }

/*
 * listAddBall - add a given ball to the end of a given list
 *
 * RETURNS: nothing
 */

void listAddBall (LIST * pList, BALL * pBall)
    {
    pBall->pGridNext = NULL;
    pBall->pGridPrev = pList->last;

    if (pList->last)
        {
        pList->last = pBall;
        }
    else
        {
        pList->head = pList->last = pBall;
        }

    }
    
/*
 * listRemoveBall - remove a ball from a list
 *
 * RETURNS: nothing.
 */

void listRemoveBall (LIST * pList, BALL * pBall)
    {

    /*
     * If there is a next ball, set it's "prev" pointer to our
     * back pointer (where "our" is the ball to be removed).
     * If not, then this is the last ball -- set the "last"
     * pointer in the list to our previous ball.
     */

    if (pBall->pGridNext)
        {
        (pBall->pGridNext)->pGridPrev = pBall->pGridPrev;
        }
    else
        {
        pList->last = pBall->pGridPrev;
        }

    /*
     * If there is a previous ball, set it's "next" pointer
     * to our "next" pointer.
     * If not, then our next ball becomes the new head of the
     * list.
     */

    if (pBall->pGridPrev)
        {
        (pBall->pGridPrev)->pGridNext = pBall->pGridNext;
        }
    else
        {
        pList->head = pBall->pGridNext;
        }
}


/****************************************************************
 *
 * Wall Functionality
 *
 * A wall a line of balls with special Bounce() and Show()
 * behaviors. The functions are self-explanatory.
 */
void wallXNew (POINT point)
    {
    BALL *  pBall;

    pBall = ballNew (HARD, wallXBounce, hardCollide, ballMove, wallXShow);
    ballPlaceFixed (pBall, point);
    }

VECTOR wallXBounce (VECTOR vector)
    {
    VECTOR  newVector;

    newVector.dx = -vector.dx;
    newVector.dy = vector.dy;

    return newVector;
    }

char wallXShow (void)
    {
    return '|';
    }

VECTOR wallYBounce (VECTOR vector)
    {
    VECTOR  newVector;

    newVector.dx = vector.dx;
    newVector.dy = -vector.dy;

    return newVector;
    }

/*TODO example for task tag: document function */
char wallYShow (void)
    {
    return '-';
    }

void wallYNew (POINT point)
    {
    BALL *  pBall;

    pBall = ballNew (HARD, wallYBounce, hardCollide, ballMove, wallYShow);
    ballPlaceFixed (pBall, point);
    }
