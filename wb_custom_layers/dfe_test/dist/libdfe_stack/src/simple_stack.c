/*
 * =====================================================================================
 *
 *       Filename:  simple_stack.c
 *
 *    Description:  simple stack test
 *
 *        Version:  1.0
 *        Created:  06/03/2011 10:21:25 AM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Yanchao Zhang (yzhang0), yanchao.zhang@windriver.com
 *        Company:  WindRiver System
 *
 * =====================================================================================
 */

extern void simple_stack1 ();
extern void simple_stack2 (char a, int b, float c, long d);
extern void simple_stack3 (char a, int b, float c, long d);
extern void simple_stack4 (char a, int b, float c, long d);
extern void simple_stack5 (char a, int b, float c, long d);

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  simple_stack1
 *  Description:  show simple local and argument values
 * =====================================================================================
 */
void simple_stack1 () {

    /*-----------------------------------------------------------------------------
     *  delcar local value
     *-----------------------------------------------------------------------------*/
    int i;
    char a1  = 3;
    int  b1  = 34;
    float c1 = 550.13;
    long d1  = 3113;

	char char_array1[5];
	char char_array2[5];
	char char_array3[5];
	char char_array4[5];
	char char_array5[5];

	int int_array1[5];
	int int_array2[5];
	int int_array3[5];
	int int_array4[5];
	int int_array5[5];

	float float_array1[5];
	float float_array2[5];
	float float_array3[5];
	float float_array4[5];
	float float_array5[5];

	long long_array1[5];
	long long_array2[5];
	long long_array3[5];
	long long_array4[5];
	long long_array5[5];


    /*-----------------------------------------------------------------------------
     *  init value
     *-----------------------------------------------------------------------------*/
	for (i = 0; i < 5; i++) {

		char_array1[i] = 1;
		char_array2[i] = 2;
		char_array3[i] = 3;
		char_array4[i] = 47;
		char_array5[i] = 35;
		
		int_array1[i] = 10;
		int_array2[i] = 20;
		int_array3[i] = 30;
		int_array4[i] = 407;
		int_array5[i] = 505;
		
		float_array1[i] = 0.001;
		float_array2[i] = 0.002;
		float_array3[i] = 0.003;
		float_array4[i] = 0.604;
		float_array5[i] = 0.305;
		
		long_array1[i] = 100000;
		long_array2[i] = 200000;
		long_array3[i] = 300000;
		long_array4[i] = 406780;
		long_array5[i] = 500330000;
	}

    /*-----------------------------------------------------------------------------
     *  do regular work
     *-----------------------------------------------------------------------------*/
	for (i = 0; i < 5; i++) {

		char_array1[i] = a1 + 3;
		char_array2[i] = a1 - 2;
		char_array3[i] = a1 * 3;
		char_array4[i] = a1 / 4;
		char_array5[i] = a1 % 3;

		int_array1[i] = b1 + 3;
		int_array2[i] = b1 - 2;
		int_array3[i] = b1 * 3;
		int_array4[i] = b1 / 4;
		int_array5[i] = b1 % 5;
		
		float_array1[i] = c1 + 0.1;
		float_array2[i] = c1 - 0.2;
		float_array3[i] = c1 * 0.3;
		float_array4[i] = c1 / 0.4;
		float_array5[i] = c1 + 0.5;
		
		long_array1[i] = d1 + 1000;
		long_array2[i] = d1 - 2000;
		long_array3[i] = d1 * 3000;
		long_array4[i] = d1 / 4000;
		long_array5[i] = d1 % 5555;
	}

/**** ***construct stack frame******/
    simple_stack2 (a1,b1,c1,d1);
}

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  simple_stack2
 *  Description:  show simple local and argument values
 * =====================================================================================
 */
void simple_stack2 (char a, int b, float c, long d) {

    /*-----------------------------------------------------------------------------
     *  delcar local value
     *-----------------------------------------------------------------------------*/
    int i;
    char a2  = 3;
    int  b2  = 34;
    float c2 = 3450.13;
    long d2  = 3113;

	char char_array1[5];
	char char_array2[5];
	char char_array3[5];
	char char_array4[5];
	char char_array5[5];

	int int_array1[5];
	int int_array2[5];
	int int_array3[5];
	int int_array4[5];
	int int_array5[5];

	float float_array1[5];
	float float_array2[5];
	float float_array3[5];
	float float_array4[5];
	float float_array5[5];

	long long_array1[5];
	long long_array2[5];
	long long_array3[5];
	long long_array4[5];
	long long_array5[5];


    /*-----------------------------------------------------------------------------
     *  init value
     *-----------------------------------------------------------------------------*/
	for (i = 0; i < 5; i++) {

		char_array1[i] = 1;
		char_array2[i] = 2;
		char_array3[i] = 3;
		char_array4[i] = 47;
		char_array5[i] = 35;
		
		int_array1[i] = 10;
		int_array2[i] = 20;
		int_array3[i] = 30;
		int_array4[i] = 407;
		int_array5[i] = 505;
		
		float_array1[i] = 0.001;
		float_array2[i] = 0.002;
		float_array3[i] = 0.003;
		float_array4[i] = 0.604;
		float_array5[i] = 0.305;
		
		long_array1[i] = 100000;
		long_array2[i] = 200000;
		long_array3[i] = 300000;
		long_array4[i] = 406780;
		long_array5[i] = 500330000;
	}

    /*-----------------------------------------------------------------------------
     *  do regular work
     *-----------------------------------------------------------------------------*/
	for (i = 0; i < 5; i++) {

		char_array1[i] = a + 3;
		char_array2[i] = a - 2;
		char_array3[i] = a * 3;
		char_array4[i] = a / 4;
		char_array5[i] = a % 3;

		int_array1[i] = b + 3;
		int_array2[i] = b - 2;
		int_array3[i] = b * 3;
		int_array4[i] = b / 4;
		int_array5[i] = b % 5;
		
		float_array1[i] = c + 0.1;
		float_array2[i] = c - 0.2;
		float_array3[i] = c * 0.3;
		float_array4[i] = c / 0.4;
		float_array5[i] = c - 50;
		
		long_array1[i] = d + 1000;
		long_array2[i] = d - 2000;
		long_array3[i] = d * 3000;
		long_array4[i] = d / 4000;
		long_array5[i] = d * 5555;
	}

/**** ***construct stack frame******/
    simple_stack3 (a2,b2,c2,d2);
}

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  simple_stack3
 *  Description:  show simple local and argument values
 * =====================================================================================
 */
void simple_stack3 (char a, int b, float c, long d) {
    /*-----------------------------------------------------------------------------
     *  delcar local value
     *-----------------------------------------------------------------------------*/
    int i;
    char a3  = 3;
    int  b3  = 34;
    float c3 = 0.13;
    long d3  = 3113;

	char char_array1[5];
	char char_array2[5];
	char char_array3[5];
	char char_array4[5];
	char char_array5[5];

	int int_array1[5];
	int int_array2[5];
	int int_array3[5];
	int int_array4[5];
	int int_array5[5];

	float float_array1[5];
	float float_array2[5];
	float float_array3[5];
	float float_array4[5];
	float float_array5[5];

	long long_array1[5];
	long long_array2[5];
	long long_array3[5];
	long long_array4[5];
	long long_array5[5];


    /*-----------------------------------------------------------------------------
     *  init value
     *-----------------------------------------------------------------------------*/
	for (i = 0; i < 5; i++) {

		char_array1[i] = 1;
		char_array2[i] = 2;
		char_array3[i] = 3;
		char_array4[i] = 47;
		char_array5[i] = 35;
		
		int_array1[i] = 10;
		int_array2[i] = 20;
		int_array3[i] = 30;
		int_array4[i] = 407;
		int_array5[i] = 505;
		
		float_array1[i] = 0.001;
		float_array2[i] = 0.002;
		float_array3[i] = 0.003;
		float_array4[i] = 0.604;
		float_array5[i] = 0.305;
		
		long_array1[i] = 100000;
		long_array2[i] = 200000;
		long_array3[i] = 300000;
		long_array4[i] = 406780;
		long_array5[i] = 500330000;
	}

    /*-----------------------------------------------------------------------------
     *  do regular work
     *-----------------------------------------------------------------------------*/
	for (i = 0; i < 5; i++) {

		char_array1[i] = a + 3;
		char_array2[i] = a - 2;
		char_array3[i] = a * 3;
		char_array4[i] = a / 4;
		char_array5[i] = a % 3;

		int_array1[i] = b + 3;
		int_array2[i] = b - 2;
		int_array3[i] = b * 3;
		int_array4[i] = b / 4;
		int_array5[i] = b % 5;
		
		float_array1[i] = c + 0.1;
		float_array2[i] = c - 0.2;
		float_array3[i] = c * 0.3;
		float_array4[i] = c / 0.4;
		float_array5[i] = c * 3.34;
		
		long_array1[i] = d + 1000;
		long_array2[i] = d - 2000;
		long_array3[i] = d * 3000;
		long_array4[i] = d / 4000;
		long_array5[i] = d % 5555;
	}

/**** ***construct stack frame******/
    simple_stack4 (a3,b3,c3,d3);
}

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  simple_stack4
 *  Description:  show simple local and argument values
 * =====================================================================================
 */
void simple_stack4 (char a, int b, float c, long d) {
    /*-----------------------------------------------------------------------------
     *  delcar local value
     *-----------------------------------------------------------------------------*/
    int i;
    char a4  = 3;
    int  b4  = 34;
    float c4 = 0.13;
    long d4  = 3113;

	char char_array1[5];
	char char_array2[5];
	char char_array3[5];
	char char_array4[5];
	char char_array5[5];

	int int_array1[5];
	int int_array2[5];
	int int_array3[5];
	int int_array4[5];
	int int_array5[5];

	float float_array1[5];
	float float_array2[5];
	float float_array3[5];
	float float_array4[5];
	float float_array5[5];

	long long_array1[5];
	long long_array2[5];
	long long_array3[5];
	long long_array4[5];
	long long_array5[5];


    /*-----------------------------------------------------------------------------
     *  init value
     *-----------------------------------------------------------------------------*/
	for (i = 0; i < 5; i++) {

		char_array1[i] = 1;
		char_array2[i] = 2;
		char_array3[i] = 3;
		char_array4[i] = 47;
		char_array5[i] = 35;
		
		int_array1[i] = 10;
		int_array2[i] = 20;
		int_array3[i] = 30;
		int_array4[i] = 407;
		int_array5[i] = 505;
		
		float_array1[i] = 0.001;
		float_array2[i] = 0.002;
		float_array3[i] = 0.003;
		float_array4[i] = 0.604;
		float_array5[i] = 0.305;
		
		long_array1[i] = 100000;
		long_array2[i] = 200000;
		long_array3[i] = 300000;
		long_array4[i] = 406780;
		long_array5[i] = 500330000;
	}

    /*-----------------------------------------------------------------------------
     *  do regular work
     *-----------------------------------------------------------------------------*/
	for (i = 0; i < 5; i++) {

		char_array1[i] = a + 3;
		char_array2[i] = a - 2;
		char_array3[i] = a * 3;
		char_array4[i] = a / 4;
		char_array5[i] = a % 3;

		int_array1[i] = b + 3;
		int_array2[i] = b - 2;
		int_array3[i] = b * 3;
		int_array4[i] = b / 4;
		int_array5[i] = b % 5;
		
		float_array1[i] = c + 0.1;
		float_array2[i] = c - 0.2;
		float_array3[i] = c * 0.3;
		float_array4[i] = c / 0.4;
		float_array5[i] = c * 7.00003;
		
		long_array1[i] = d + 1000;
		long_array2[i] = d - 2000;
		long_array3[i] = d * 3000;
		long_array4[i] = d / 4000;
		long_array5[i] = d % 5555;
	}

/**** ***construct stack frame******/
    simple_stack5 (a4,b4,c4,d4);
}

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  simple_stack5
 *  Description:  show simple local and argument values
 * =====================================================================================
 */
void simple_stack5 (char a, int b, float c, long d) {
    /*-----------------------------------------------------------------------------
     *  delcar local value
     *-----------------------------------------------------------------------------*/
    int i;
	char char_array1[5];
	char char_array2[5];
	char char_array3[5];
	char char_array4[5];
	char char_array5[5];

	int int_array1[5];
	int int_array2[5];
	int int_array3[5];
	int int_array4[5];
	int int_array5[5];

	float float_array1[5];
	float float_array2[5];
	float float_array3[5];
	float float_array4[5];
	float float_array5[5];

	long long_array1[5];
	long long_array2[5];
	long long_array3[5];
	long long_array4[5];
	long long_array5[5];


    /*-----------------------------------------------------------------------------
     *  init value
     *-----------------------------------------------------------------------------*/
	for (i = 0; i < 5; i++) {

		char_array1[i] = 1;
		char_array2[i] = 2;
		char_array3[i] = 3;
		char_array4[i] = 47;
		char_array5[i] = 35;
		
		int_array1[i] = 10;
		int_array2[i] = 20;
		int_array3[i] = 30;
		int_array4[i] = 407;
		int_array5[i] = 505;
		
		float_array1[i] = 0.001;
		float_array2[i] = 0.002;
		float_array3[i] = 0.003;
		float_array4[i] = 0.604;
		float_array5[i] = 0.305;
		
		long_array1[i] = 100000;
		long_array2[i] = 200000;
		long_array3[i] = 300000;
		long_array4[i] = 406780;
		long_array5[i] = 500330000;
	}

    /*-----------------------------------------------------------------------------
     *  do regular work
     *-----------------------------------------------------------------------------*/
	for (i = 0; i < 5; i++) {

		char_array1[i] = a + 3;
		char_array2[i] = a - 2;
		char_array3[i] = a * 3;
		char_array4[i] = a / 4;
		char_array5[i] = a % 3;

		int_array1[i] = b + 3;
		int_array2[i] = b - 2;
		int_array3[i] = b * 3;
		int_array4[i] = b / 4;
		int_array5[i] = b % 5;
		
		float_array1[i] = c + 0.1;
		float_array2[i] = c - 0.2;
		float_array3[i] = c * 0.3;
		float_array4[i] = c / 0.4;
		float_array5[i] = c * 3000.02;
		
		long_array1[i] = d + 1000;
		long_array2[i] = d - 2000;
		long_array3[i] = d * 3000;
		long_array4[i] = d / 4000;
		long_array5[i] = d % 5555;
	}
}

