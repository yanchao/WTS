/* math.c - implements math demonstration functions */

/* Copyright 2004 Wind River Systems, Inc. */

/*
modification history
--------------------
01a,07oct04,f_b   added
*/

/*
 *  General Definitions
 */
#define MAX_FACTORIAL  10
#define MAX_FIBONACCI  10

/*
 *  General Global Declarations
 */
int   fact_table[MAX_FACTORIAL];       

int   num_fibs;                 /* table of fibonacci numbers */   
int   fib_table[MAX_FIBONACCI];

/*
 *  Forward References
 */
int factorial( int );
int fibonacci( int );


/************************************************************************
* Function:    factorialDemo()
* Description: Use a recursive function to generates a table of factorials.
*************************************************************************/
void factorialDemo( void )
{
    int   sum;          /* will be the sum of factorials from 0 to 9 */
    int   clear_index;  /* general local variables */
    int   loopvar;

    loopvar = 0;
    
   /*
    *  Start by zero'ing out the factorial array table. This table
    *  will be used to store a calculated factorial sequence.
    */
    for (clear_index = 0; clear_index < MAX_FACTORIAL; ++clear_index) 
    {
       fact_table[clear_index] = 0;
    }

   /*
    *  Calculate and store the factorial sequence.
    */
    for (sum=0, loopvar=0; loopvar < MAX_FACTORIAL; ++loopvar) 
    {
        fact_table[ loopvar ] = factorial( loopvar );

       /*
        *  Keep a running sum of the calculated factorials. 
        */ 
        sum += fact_table[loopvar];
    }

} /* end of factorialDemo() */


/************************************************************************
* Function:    fibonacciDemo()
* Description: Use a recursive function to generates the fibonacci sequence.
*               Each fibonacci number is the sum of the, that is,
*               ( 0,1,1,2,3,5,8,13,....)
*               The algorithm is given by:
*               F(0) = 0
*               F(1) = 1
*               F(i) = F(i-1) + F(i-2) ,    i >= 2
************************************************************************/
void fibonacciDemo( void )
{
    int   sum;          /* will be the sum of the fibonacci series */
    int   clear_index;  /* general local variables */
    int   loopvar;
    volatile int temp;

    loopvar = 0;
    sum = 0;
    
   /*
    *  Let's start by zero'ing out the fibonacci array table. This table
    *  will be used to store a calculated fibonacci sequence.
    */

    for (clear_index = 0; clear_index < MAX_FIBONACCI; ++clear_index) 
    {
       fib_table[clear_index] = 0;
    }

   /*
    *  Calculate and store the fibonacci sequence.
    */
    for (loopvar = 0; loopvar < MAX_FIBONACCI; loopvar++) 
    {
        temp = fibonacci( loopvar );
        
        fib_table[ loopvar ] =  temp;

       /*
        *  Keep a running sum of the calculated fibonacci sequence. 
        */ 
        sum += fib_table[loopvar];
    }

}       /* end of fibonacciDemo() */


/************************************************************************
*  Function: Compute the factorial of a number 
*************************************************************************/
int factorial( int  num)
{
    if ( num < 2 )
      return(1);

    num *= factorial( num-1 );

    return (num);
}

/************************************************************************
*  Function:    fibonacci()
*  Description: Computes the fibonacci of a number 
************************************************************************/
int fibonacci( int  num)
{
    if ( num >= 2 )
    {
      num = fibonacci( num-1 ) + fibonacci( num-2 );
    }

    return num;
}

/************************************************************************
*  Function:    abs( )
*  Description: Computes the absolute value of the passed in int.
************************************************************************/
int abs( int  num)
{
  return ( num >= 0 ) ? num : (num *= -1);
}


