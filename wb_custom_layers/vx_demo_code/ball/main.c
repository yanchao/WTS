/* main.c - Wind River Bouncing Balls C Sample Program */

/*
DESCRIPTION

This sample is part of a tutorial that describes how to get
started with Wind River Workbench, including how to create,
build, and execute projects. There is a similar C++ program.

The program implements a box (called the grid) in which a number
of "balls" interact with each other and the walls.  Different
kinds of balls display different behaviors. (The "physics" of
this 2D world are imaginary; a realistic simulation is not
intended.)

See the readme.txt file in the project directory for a brief
description of how to build and run the sample. To watch the
balls move, place a break as directed in main() below, display
global array grid in the Watch window, and execute.

Modules:
  - main.c: This module. Includes main() and BALL, POINT, and
            VECTOR object functions.
  - grid.c: Functions to manage the grid and walls, and to
            operate on the list of balls at each grid point.

Note on Passing and Returning Structures:
    This code passes, returns, and assigns small structures for
    points, vectors, and lists. While this does not conform to
    the Wind River coding conventions, it is used for cleaner
    code and to be similar to the C++ version.

INCLUDE FILES
  - ball.h: definitions for all types (BALL, LIST, POINT, VECTOR)
            and all function prototypes.  */

/* includes */

#include <stdlib.h>
#include <stdio.h>
#include "ball.h"

/* globals */

BALL * pMovableBalls; /* list of all movable balls on the grid */
                        
BOOL finished = 0;      /* continuing running until this global */
                        /* is non-zero */

int num_hard = 2;       /* number of each kind of ball; use */
int num_soft = 2;       /* variables to permit experimenting */
int num_spin = 2;       /* without recompiling */


int ballMain (void)
    {
    int     i;
    BALL *  p;
    int     seed = 3;
    
    gridInit ();

    srand(seed);        /* stop here and set seed for different */
                        /* initial ball arrangements */

    for (i = 0; i < num_hard; i++)
        {
        hardNew ();
        }

    for (i = 0; i < num_soft; i++)
        {
        softNew ();
        }
    
    for (i = 0; i < num_spin; i++)
        {
        spinNew ();
        }

    /*
     * To watch the balls move, display global "grid" in the
     * Memory window, and place a breakpoint with property 
     * "Continue on Break" enabled at either the next statement
     * or at the "for" statement for a more detailed view.
     */

    while (! finished)
        {
        for (p = pMovableBalls; p != NULL ; p = p->pNextMovable)
            {
            p->pBallMove (p);
            }
        }

    return 0;
    }


/****************************************************************
 *
 * Hard Ball Unique Characteristics
 * 
 *    - hardBounce:     bounce incoming ball by 180% (back where
 *                      it came from)
 *    - hardCollide:    TRUE -- balls collide with hard balls
 *    - hardShow:       '*'
 */

BALL *  hardNew (void)
    {
    BALL *  p;

    p = ballNew (HARD, hardBounce, hardCollide, ballMove, hardShow);
    ballPlaceMovable (p);
    
    return p;
    }

/*
 * hardBounce - bounce incoming ball by 180% (back where it
 *              came from 
 *
 * Given the change vector of an incoming ball, return a change
 * vector that is 180% opposite to it. That is, when a ball
 * collides with a hard ball, the hard ball's hardBounce
 * function reverses that change vector.
 *
 * RETURNS: new direction vector after bounce.
 */

VECTOR hardBounce
    (
    VECTOR vector   /* incoming change vector */
    )
    {
    VECTOR  newVector;

    newVector.dx = -vector.dx;
    newVector.dy = -vector.dy;
    return newVector;
    }

BOOL hardCollide (void)
    {
    return TRUE;
    }

char hardShow (void)
    {
    return '*';
    }


/****************************************************************
 *
 * Soft Ball Unique Characteristics
 * 
 *    - softShow:       'O'
 *
 * Rely on the BALL base functions for "collide" and "bounce".
 * Thus, other balls do not "collide" with soft balls and soft
 * soft balls impart no "bounce".
 */

BALL *  softNew (void)
    {
    BALL *  p;

    p = ballNew (SOFT, ballBounce, ballCollide, ballMove, softShow);
    ballPlaceMovable (p);
    
    return p;
    }

char softShow (void)
{
    return 'O';
}


/****************************************************************
 *
 * Spin Ball Unique Characteristics
 * 
 *    - spinBounce:     bounce incoming ball by 90% clockwise
 *    - spinCollide:    TRUE -- balls collide with spin balls
 *    - spinShow:       '@'
 */

BALL *  spinNew (void)
    {
    BALL *  p;

    p = ballNew (SPIN, spinBounce, spinCollide, ballMove, spinShow);
    ballPlaceMovable (p);
    
    return p;
    }

/*
 * spinBounce - bounce incoming ball by 90% clockwise
 *
 * Given the change vector of an incoming ball, return a change
 * vector that is 90% clockwise to it. That is, when a ball
 * collides with a spin ball, the spin ball's  spinBounce
 * function turns the ball 90% clockwise.
 *
 * RETURNS: new direction vector after bounce.
 */

VECTOR spinBounce
    (
    VECTOR vector   /* incoming change vector */
    )
    {
    VECTOR  newVector;

    newVector.dx = -vector.dx;
    newVector.dy = -vector.dy;
    return newVector;
    }

BOOL spinCollide (void)
    {
    return TRUE;
    }

char spinShow (void)
    {
    return '@';
    }


/****************************************************************
 *
 * General Ball Functionality
 */

/*
 * ballNew - create new BALL object with default behaviors (but
 *           do not put it on the grid)
 *
 * RETURNS: pointer to the BALL object.
 */

BALL * ballNew
    (
    BALL_KIND   ballKind,   /* HARD, SOFT, or SPIN for */
                            /* debugging purposes */
    /* 
     * The next four parameters are the "bounce", "collide",
     * "move", and "show" functions for the ball. Pass the
     * general functions ballBounce() and so forth for default
     * behavior (that of a soft ball with 0,0 initial change
     * vector).
     */
    VECTOR  (*pBounce)(VECTOR),
    BOOL    (*pCollide)(void),
    void    (*pMove)(struct ball *),
    char    (*pShow)(void)
    )
    {
    BALL *  pBall;

    pBall = (BALL *) malloc (sizeof (BALL));

    pBall->ballKind     = ballKind;
    pBall->pNextMovable = NULL;
    pBall->pGridNext    = pBall->pGridPrev = NULL;

    pBall->position     = pointNew  (0, 0);
    pBall->change       = vectorNew (0, 0);
    
    pBall->pBallBounce  = pBounce;
    pBall->pBallCollide = pCollide;
    pBall->pBallMove    = pMove;
    pBall->pBallShow    = pShow;
    
    return pBall;
    }

/*
 * ballPlaceFixed - place unmovable ball at fixed point
 * 
 * Used to create hard balls for walls.
 * 
 * RETURNS: nothing.
 */

void ballPlaceFixed (BALL * pBall, POINT point)
    {
    gridAddBall (pBall, point);
    }
    
/*
 * ballPlaceMovable - place movable ball at random grid point
 * 
 * Add the ball to the "pMovableBalls" list and set:
 *    - Ball position to a random point in the interior of
 *      the grid (not on a wall)
 *    - "change" vector to a random value in the set of
 *      integer points from -1,-1 through 1,1 inclusive
 * 
 * RETURNS: nothing.
 */

void ballPlaceMovable (BALL * pBall)
    {
    pBall->pNextMovable = pMovableBalls;
    pMovableBalls       = pBall;
        
    pBall->position.x = randRange (1, WIDTH - 1);
    pBall->position.y = randRange (1, HEIGHT - 1);
    pBall->change.dx  = randRange (-1, 2);
    pBall->change.dy  = randRange (-1, 2);
        
    gridAddBall (pBall, pBall->position );
    }


/*
 * ballBounce - default Bounce() behavior
 *
 * RETURNS: change vector of 0,0 (regardless of input).
 */

VECTOR ballBounce (VECTOR vector)
    {
    VECTOR  newVector;

    newVector.dx = 0;
    newVector.dy = 0;
    return newVector;
    }

/*
 * ballCollide - default Collide() behavior - no collision
 *
 * RETURNS: FALSE.
 */

BOOL ballCollide (void)
    {
    return FALSE;
    }


/*
 * ballMove - move the ball to a new position based on its change
 *            vector and whether it encounters another ball
 *
 * RETURNS: nothing.
 * 
 * DESCRIPTION:
 * 
 * Briefly, move the ball to the next position in the same
 * direction (it's current position plus its change vector).  If
 * it encounters a hard or spin ball (or a wall which is just a
 * line of hard balls), that is if it encounters a ball with a
 * TRUE "collide" function at the new position, the ball at the
 * new position "bounces" the incoming ball by changing that
 * ball's change vector. Then, in this function, move the ball
 * again to the next new position and leave it there (regardless
 * of whether some other ball or wall is there).
 * 
 * In more detail:
 *  Calculate a tentative new position for this ball by adding
 *  its current change vector to its current position. (The
 *  initial "change" vector is a random value in the set of
 *  integer pairs from -1,-1 through 1,1 inclusive established
 *  in ballPlaceMovable().)
 * 
 *  If there is a ball at the new position and that ball
 *  "collides" with other balls (hard and spin balls collide,
 *  soft balls don't):
 * 
 *      "Bounce" this ball by resetting it's change vector based
 *      on ball at the new position.
 * 
 *      Re-calculate this ball's new position by adding the
 *      updated change vector to the tentative new position.
 * 
 *  Delete this ball from it's current position and add it at the
 *  new position. This displays the ball and places it at the end
 *  of the list of balls at the new position. (Note: a ball 
 *  collides with the first ball in the list at a position.)
 */

void ballMove (BALL * pBall)
    {
    POINT       newPosition;

    newPosition = pointPlus (pBall->position, pBall->change);

    if (gridIsBallAt (newPosition ) )
        {
        if ((pGridBall (newPosition)->pBallCollide)() )
            {
            pBall->change = 
                (pGridBall(newPosition)->pBallBounce)(pBall->change);
            newPosition = pointPlus (newPosition, pBall->change);
            }
        }

    /*
     * Remove ball from list at old position and add to list at new.
     */

    gridDeleteBall (pBall, pBall->position );
    pBall->position = newPosition;
    gridAddBall (pBall, pBall->position );
    } 


/*
 * ballShow - default Show() behavior - soft ball
 */

char ballShow (void)
    {
    return 'O';
    }


/****************************************************************
 *
 * POINT Object Functionality
 */

/* 
 * pointNew - return an initialized point
 *
 * RETURNS: a POINT structure with x and y components set as
 *          given (see comment in DESCRIPTION section of main.c 
 *          regarding passing and returning structures).
 */

POINT pointNew (int x, int y )
    {
    POINT   point;

    point.x = x;
    point.y = y;

    return point;
    }

/*
 * pointPlus - return point equal to the sum of a point and
 *             a change vector
 *
 * Ensures that the result is within the grid, *including* the
 * grid walls. If either x or y of the result would be outside a
 * grid wall, set it back to that wall.
 *
 * RETURNS: 
 */

POINT pointPlus
    (
    POINT point,    /* initial point */
    VECTOR vector   /* change vector */
    )
    {
    POINT   newPoint;
    
    newPoint.x = point.x + vector.dx;
    newPoint.y = point.y + vector.dy;

    if (newPoint.x < 0)
        {
        newPoint.x = 0;
        }
    else if (newPoint.x >= WIDTH)
        {
        newPoint.x = WIDTH - 1;
        }
        
    if (newPoint.y < 0)
        {
        newPoint.y = 0;
        }
    else if (newPoint.y >= HEIGHT)
        {
        newPoint.y = HEIGHT - 1;
        }
        
    return newPoint;
    }


/****************************************************************
 *
 * VECTOR Object Functionality
 *
 * Functions to implement a sort of "change vector" for balls,
 * consisting only of dx and dy components. The start point of
 * the vector is the current location of the ball, and the end
 * point is the start point offset by the dx,dy components.
 */

/* 
 * vectorNew - return an initialized vector
 *
 * RETURNS: a VECTOR structure with dx and dy components set as
 *          given (see comment in DESCRIPTION section of main.c 
 *          regarding passing and returning structures).
 */

VECTOR vectorNew (int dx, int dy )
    {
    VECTOR  vector;

    vector.dx = dx;
    vector.dy = dy;

    return vector;
    }


/*
 * randRange - return a random number in a range
 * 
 * RETURNS: a random number in the range [min, max) (that is,
 *          including min but 1 less than max).
 *
 * Uses the high bits from rand() for better randomness.  See
 * www.thinkage.ca/english/gcos/expl/c/lib/rand.html and
 * www.die.net/doc/linux/man/man3/rand.3.html. Use method from
 * the first page to avoid floating point. Can be seeded by
 * calling srand() as usual for rand().
 */
 
int randRange
    (
    int min,    /* low end of range */
    int max     /* high end of range */
    )
    {
    int     val;
    
    /* Generate a value between 0 and max-1. */
    
    while (max <= (val = rand() / (RAND_MAX/max)));
    
    /* Return above value offset by minimum. */
    
    return val + min;
    }
