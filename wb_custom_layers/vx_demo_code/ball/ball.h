/* ball.h - Wind River Bouncing Balls C Sample Program */

#ifndef __INCballh
#define __INCballh

/* Basic type extensions */

#ifndef NULL
#define NULL    0
#endif

#ifndef TRUE
typedef int     BOOL;
#define TRUE    1
#define FALSE   0
#endif

/****************************************************************
 *
 * VECTOR Type
 */

typedef struct vector   /* delta x,y change that can be added */
    {                   /* to a point */
    int     dx;
    int     dy;
    } VECTOR;

VECTOR vectorNew (int dx, int dy);

/****************************************************************
 *
 * POINT Types and Prototypes
 */

typedef struct point    /* an x,y position in the grid */
    {
    int     x;
    int     y;
    } POINT;

POINT pointNew (int x, int y);
POINT pointPlus (POINT point, VECTOR vector);


/****************************************************************
 *
 * BALL Type and Prototypes
 *
 * A generic ball. Each ball has a position and a change vector
 * for the next move.
 *
 * Each ball is also on two linked lists threaded through fields
 * in the BALL object:
 *    - A singly-linked list of all *movable* balls threaded
 *      through the pNextMovable pointer in each BALL object. The 
 *      head this list is pMovableBalls in main.c.
 *    - A doubly-linked list of all ball objects at the same grid 
 *      position threaded through the pGridNext and pGridPrev
 *      pointers in each BALL object. The head of each of these
 *      lists is in the grid array in grid.c.
 *
 * Each ball is represented by a character returned by its Show()
 * function, can Move(), can determine collisions of incoming
 * balls (Collide()), and can affect the change vector of
 * incoming balls (Bounce()). The BALL object contains function
 * pointers for these functions so different kinds of balls
 * (hard, soft, and spin) can have different behavior.
 *
 * A hard ball that is part of a wall is on the grid and is
 * included in the doubly-linked list at that point, but is not
 * on the pMovableBalls list (thus its pNexMovable pointer is
 * always NULL).
 *
 * The ballKind field and BALL_KIND enumeration are present only
 * for debugging: they make it easy to identify the kind of ball
 * when looking at a BALL structure, and show symbolic display of
 * enumerations.
 */

typedef enum {HARD, SOFT, SPIN} BALL_KIND;

typedef struct ball
    {
    BALL_KIND       ballKind;       /* HARD, SOFT, or SPIN */
    struct ball *   pNextMovable;   /* link to next ball on the */
                                    /* list of movable balls */
    struct ball *   pGridNext;      /* to next ball at same grid point */
    struct ball *   pGridPrev;      /* to previous ball at same grid point */
    POINT           position;       /* position of the ball on the grid */
    VECTOR          change;         /* change vector of the ball */

    /*
     * Function pointers to support different behaviors for
     * different kinds of balls:
     */

    VECTOR  (*pBallBounce)(VECTOR vector);  /* change the incoming vector */
    BOOL    (*pBallCollide)(void);          /* does collision affect */
                                            /* incoming change vector? */
    void    (*pBallMove)(struct ball * pBall);  /* move the ball */
    char    (*pBallShow)(void);             /* return character to */
                                            /* show the ball */
    } BALL;

BALL * ballNew
    (
    BALL_KIND   ballKind,
    VECTOR      (*pBounce)(VECTOR),
    BOOL        (*pCollide)(void),
    void        (*pMove)(struct ball *),
    char        (*pShow)(void)
    );
void    ballPlaceFixed (BALL * pBall, POINT point);
void    ballPlaceMovable (BALL * pBall);
VECTOR  ballBounce (VECTOR vector);
BOOL    ballCollide (void);
void    ballMove (BALL * pBall);
char    ballShow (void);

BALL *  hardNew (void);
VECTOR  hardBounce (VECTOR vector);
BOOL    hardCollide (void);
char    hardShow (void);

BALL *  softNew (void);
char    softShow (void);

BALL *  spinNew (void);
VECTOR  spinBounce (VECTOR vector);
BOOL    spinCollide (void);
char    spinShow (void);


/****************************************************************
 *
 * LIST Type and Prototypes
 *
 * A LIST object represents a doubly-linked list of the balls at 
 * each position of the grid. (It is not used for the list of all 
 * balls.)
 *
 * The list structure below consists only of pointers to the ball
 * at the head and end of the list. The list is threaded through
 * the pGridNext and pGridPrev fields in each BALL object.
 */

typedef struct list
    {
    BALL *  head;
    BALL *  last;
    } LIST;

void   listInit (LIST list);
void   listAddBall (LIST * pList, BALL * pBall);
void   listRemoveBall (LIST * pList, BALL * pBall);
BALL * pListFirstBall (LIST list);
BALL * pListLastBall (LIST list);


/****************************************************************
 *
 * Grid Dimensions and Prototypes
 *
 * The grid represents the collection of all balls which are the
 * walls or are moving. See grid.c for the elements of the grid
 * and their descriptions.
 */

#define HEIGHT 10   /* grid height */
#define WIDTH 16    /* grid width */

void   gridAddBall(BALL * pBall, POINT point);
void   gridDeleteBall (BALL * pBall, POINT point);
void   gridInit (void);
BOOL   gridIsBallAt (POINT point);
BALL * pGridBall (POINT point);


/****************************************************************
 *
 * Wall Prototypes
 *
 * A wall is a line of hard balls with some behavioral 
 * modifications.
 */

void   wallXNew (POINT point);      /* For floor and ceiling */
VECTOR wallXBounce (VECTOR vector);
char   wallXShow ();

void   wallYNew (POINT point);      /* For side walls */
VECTOR wallYBounce (VECTOR vector);
char   wallYShow ();

/*
 * randRange Prototype
 */
 
int randRange (int min, int max);

#endif /* __INCballh */
