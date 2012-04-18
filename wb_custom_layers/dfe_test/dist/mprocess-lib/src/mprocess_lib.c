/* mthread.c - Sample Program for Usermode Multi-threaded debugging */

/* Copyright 2006 Wind River Systems, Inc. */


/* 
 * DESCRIPTION
 * A Pthreads program.  Performs thread creation and termination.
 * 
 * This file must be linked against the pthread library.
 * % gcc -o mthread mthread.c -lpthread
 */

#include <stdio.h>
#include <pthread.h>
#include <sys/prctl.h>
#include <sys/types.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include "stack_header.h"

#define DEFAULT_NUM_THREADS	2
INNER_STRUCT_TYPE1  global_s1; 
unsigned int sleep_time = 2;
char counter0 = 0;
short counter1 = 0;
int counter2 = 0;
long counter3 = 0;
long long counter4 = 0;
int verbose = 1;

typedef void * (*VOIDFUNCPTR) (void * arg);

void dummy () {

    simple_stack1();
}

void alone () {
    static int toto = 0;

    prctl(PR_SET_NAME, "yzhang0 alone", 0, 0, 0);
    while (1) {
        toto++;
        dummy ();
        if (verbose) {
            printf ("In alone (pid %d) \n",getpid());
        }
        sleep (sleep_time); 
    }
}

void   printInfo(const char * header) {
    /* pthread_self() returns ???
     * getppid() returns the PID of the manager thread
     * getpid() returns the lightweight PID
     */
    if (verbose) {
        printf("%s :\tThreadID =%8d ParentID = %8d ID_self = %8d\n",
            header, (int) pthread_self(),  getppid(),  getpid());
    }
}

void sharedCode () {
    static int counter;

    counter++; 

    ++counter0; 
      
    ++counter1; 
      
    ++counter2; 
      
    ++counter3; 
      
    ++counter4; 

    /********Complicated structure test*******/

    global_s1.int_var1 += 1;
    global_s1.float_var1 += 1.1;
    global_s1.string_pointer1 = "One";

    global_s1.s2.int_var2 += 2;
    global_s1.s2.float_var2 += 2.2;
    global_s1.s2.string_pointer2 = "Two";

    global_s1.s2.s3.int_var3 += 3;
    global_s1.s2.s3.float_var3 += 3.3;
    global_s1.s2.s3.string_pointer3 = "Three";

    global_s1.s2.s3.s4.int_var4 += 4;
    global_s1.s2.s3.s4.float_var4 += 4.4;
    global_s1.s2.s3.s4.string_pointer4 = "Four";

    global_s1.s2.s3.s4.s5.int_var5 += 5;
    global_s1.s2.s3.s4.s5.float_var5 += 5.5;
    global_s1.s2.s3.s4.s5.string_pointer5 = "Five";

    global_s1.s2.s3.s4.s5.s6.int_var6 += 6;
    global_s1.s2.s3.s4.s5.s6.float_var6 += 6.6;
    global_s1.s2.s3.s4.s5.s6.string_pointer6 = "Six";

    global_s1.s2.s3.s4.s5.s6.s7.int_var7 += 7;
    global_s1.s2.s3.s4.s5.s6.s7.float_var7 += 7.7;
    global_s1.s2.s3.s4.s5.s6.s7.string_pointer7 = "Seven";

    global_s1.s2.s3.s4.s5.s6.s7.s8.int_var8 += 8;
    global_s1.s2.s3.s4.s5.s6.s7.s8.float_var8 += 8.8;
    global_s1.s2.s3.s4.s5.s6.s7.s8.string_pointer8 = "Eight";

    global_s1.s2.s3.s4.s5.s6.s7.s8.s9.int_var9 += 9;
    global_s1.s2.s3.s4.s5.s6.s7.s8.s9.float_var9 += 9.9;
    global_s1.s2.s3.s4.s5.s6.s7.s8.s9.string_pointer9 = "Nine";

    global_s1.s2.s3.s4.s5.s6.s7.s8.s9.s10.int_var10 += 10;
    global_s1.s2.s3.s4.s5.s6.s7.s8.s9.s10.float_var10 += 10.10;
    global_s1.s2.s3.s4.s5.s6.s7.s8.s9.s10.string_pointer10 = "Ten";

    global_s1.s2.s3.s4.s5.s6.s7.s8.s9.s10.s11.int_var11 += 11;
    global_s1.s2.s3.s4.s5.s6.s7.s8.s9.s10.s11.float_var11 += 11.11;
    global_s1.s2.s3.s4.s5.s6.s7.s8.s9.s10.s11.string_pointer11 = "Eleven";

    global_s1.s2.s3.s4.s5.s6.s7.s8.s9.s10.s11.s12.int_var12 += 12;
    global_s1.s2.s3.s4.s5.s6.s7.s8.s9.s10.s11.s12.float_var12 += 12.12;
    global_s1.s2.s3.s4.s5.s6.s7.s8.s9.s10.s11.s12.string_pointer12 = "Twelve";

    global_s1.s2.s3.s4.s5.s6.s7.s8.s9.s10.s11.s12.s13.int_var13 += 13;
    global_s1.s2.s3.s4.s5.s6.s7.s8.s9.s10.s11.s12.s13.float_var13 += 13.13;
    global_s1.s2.s3.s4.s5.s6.s7.s8.s9.s10.s11.s12.s13.string_pointer13 = "Thirteen";

    global_s1.s2.s3.s4.s5.s6.s7.s8.s9.s10.s11.s12.s13.s14.int_var14 += 14;
    global_s1.s2.s3.s4.s5.s6.s7.s8.s9.s10.s11.s12.s13.s14.float_var14 += 14.14;
    global_s1.s2.s3.s4.s5.s6.s7.s8.s9.s10.s11.s12.s13.s14.string_pointer14 = "Fourteen";

    global_s1.s2.s3.s4.s5.s6.s7.s8.s9.s10.s11.s12.s13.s14.s15.int_var15 += 15;
    global_s1.s2.s3.s4.s5.s6.s7.s8.s9.s10.s11.s12.s13.s14.s15.float_var15 += 15.15;
    global_s1.s2.s3.s4.s5.s6.s7.s8.s9.s10.s11.s12.s13.s14.s15.string_pointer15 = "Fifteen";

    global_s1.s2.s3.s4.s5.s6.s7.s8.s9.s10.s11.s12.s13.s14.s15.s16.int_var16 += 16;
    global_s1.s2.s3.s4.s5.s6.s7.s8.s9.s10.s11.s12.s13.s14.s15.s16.float_var16 += 16.16;
    global_s1.s2.s3.s4.s5.s6.s7.s8.s9.s10.s11.s12.s13.s14.s15.s16.string_pointer16 = "Sixteen";

    global_s1.s2.s3.s4.s5.s6.s7.s8.s9.s10.s11.s12.s13.s14.s15.s16.s17.int_var17 += 17;
    global_s1.s2.s3.s4.s5.s6.s7.s8.s9.s10.s11.s12.s13.s14.s15.s16.s17.float_var17 += 17.17;
    global_s1.s2.s3.s4.s5.s6.s7.s8.s9.s10.s11.s12.s13.s14.s15.s16.s17.string_pointer17 = "Seventeen";

    global_s1.s2.s3.s4.s5.s6.s7.s8.s9.s10.s11.s12.s13.s14.s15.s16.s17.s18.int_var18 += 18;
    global_s1.s2.s3.s4.s5.s6.s7.s8.s9.s10.s11.s12.s13.s14.s15.s16.s17.s18.float_var18 += 18.18;
    global_s1.s2.s3.s4.s5.s6.s7.s8.s9.s10.s11.s12.s13.s14.s15.s16.s17.s18.string_pointer18 = "Eighteen";

    global_s1.s2.s3.s4.s5.s6.s7.s8.s9.s10.s11.s12.s13.s14.s15.s16.s17.s18.s19.int_var19 += 19;
    global_s1.s2.s3.s4.s5.s6.s7.s8.s9.s10.s11.s12.s13.s14.s15.s16.s17.s18.s19.float_var19 += 19.19;
    global_s1.s2.s3.s4.s5.s6.s7.s8.s9.s10.s11.s12.s13.s14.s15.s16.s17.s18.s19.string_pointer19 = "Nineteen";

    global_s1.s2.s3.s4.s5.s6.s7.s8.s9.s10.s11.s12.s13.s14.s15.s16.s17.s18.s19.s20.int_var20 += 20;
    global_s1.s2.s3.s4.s5.s6.s7.s8.s9.s10.s11.s12.s13.s14.s15.s16.s17.s18.s19.s20.float_var20 += 20.20;
    global_s1.s2.s3.s4.s5.s6.s7.s8.s9.s10.s11.s12.s13.s14.s15.s16.s17.s18.s19.s20.string_pointer20 = "Twenty";

    global_s1.s2.s3.s4.s5.s6.s7.s8.s9.s10.s11.s12.s13.s14.s15.s16.s17.s18.s19.s20.u1.long_integer += 1;
    global_s1.s2.s3.s4.s5.s6.s7.s8.s9.s10.s11.s12.s13.s14.s15.s16.s17.s18.s19.s20.u1.short_integer += 2;
    global_s1.s2.s3.s4.s5.s6.s7.s8.s9.s10.s11.s12.s13.s14.s15.s16.s17.s18.s19.s20.u1.integer_array[0] += 3;
    global_s1.s2.s3.s4.s5.s6.s7.s8.s9.s10.s11.s12.s13.s14.s15.s16.s17.s18.s19.s20.u1.integer_array[1] += 4;
    global_s1.s2.s3.s4.s5.s6.s7.s8.s9.s10.s11.s12.s13.s14.s15.s16.s17.s18.s19.s20.u1.integer_array[2] += 5;
    global_s1.s2.s3.s4.s5.s6.s7.s8.s9.s10.s11.s12.s13.s14.s15.s16.s17.s18.s19.s20.u1.integer_array[3] += 6;
    global_s1.s2.s3.s4.s5.s6.s7.s8.s9.s10.s11.s12.s13.s14.s15.s16.s17.s18.s19.s20.u1.float_var += 1.1;

}

void printProcessInfo(long i) {

    char threadStr[16] = "Thread ";
    sprintf (&(threadStr[strlen(threadStr)]), "%ld", i);
    printInfo((const char *) threadStr);
    prctl(PR_SET_NAME, "yzhang0 process", 0, 0, 0);

    /* keep thread going forever */
    while (1) {
        if (verbose) {
            printf ("Process %ld is awake (pid %d).\n", i, getpid());
        }
        sleep(sleep_time);
        sharedCode(); 
    }

    /* pthread_exit */
    return;
}

int main(int argc, char *argv[]) {
    pthread_t threads[100];
    int retval;
    int nr_process;
    long t;

    if (argc == 2)
        nr_process = atoi(argv[1]);
    else if (argc == 1)
        nr_process = DEFAULT_NUM_THREADS;
    else {
        printf ("Error Usage: %s num_threads\n", argv[0]);
        exit (1);
    }

    if (nr_process > 100) {
        printf ("Error Usage: %d is greater than 100\n", nr_process);
        exit (1);
    }

    /********Complicated structure test*******/
    global_s1.int_var1 = 1;
    global_s1.float_var1 = 1.1;
    global_s1.string_pointer1 = "One";

    global_s1.s2.int_var2 = 2;
    global_s1.s2.float_var2 = 2.2;
    global_s1.s2.string_pointer2 = "Two";

    global_s1.s2.s3.int_var3 = 3;
    global_s1.s2.s3.float_var3 = 3.3;
    global_s1.s2.s3.string_pointer3 = "Three";

    global_s1.s2.s3.s4.int_var4 = 4;
    global_s1.s2.s3.s4.float_var4 = 4.4;
    global_s1.s2.s3.s4.string_pointer4 = "Four";

    global_s1.s2.s3.s4.s5.int_var5 = 5;
    global_s1.s2.s3.s4.s5.float_var5 = 5.5;
    global_s1.s2.s3.s4.s5.string_pointer5 = "Five";

    global_s1.s2.s3.s4.s5.s6.int_var6 = 6;
    global_s1.s2.s3.s4.s5.s6.float_var6 = 6.6;
    global_s1.s2.s3.s4.s5.s6.string_pointer6 = "Six";

    global_s1.s2.s3.s4.s5.s6.s7.int_var7 = 7;
    global_s1.s2.s3.s4.s5.s6.s7.float_var7 = 7.7;
    global_s1.s2.s3.s4.s5.s6.s7.string_pointer7 = "Seven";

    global_s1.s2.s3.s4.s5.s6.s7.s8.int_var8 = 8;
    global_s1.s2.s3.s4.s5.s6.s7.s8.float_var8 = 8.8;
    global_s1.s2.s3.s4.s5.s6.s7.s8.string_pointer8 = "Eight";

    global_s1.s2.s3.s4.s5.s6.s7.s8.s9.int_var9 = 9;
    global_s1.s2.s3.s4.s5.s6.s7.s8.s9.float_var9 = 9.9;
    global_s1.s2.s3.s4.s5.s6.s7.s8.s9.string_pointer9 = "Nine";

    global_s1.s2.s3.s4.s5.s6.s7.s8.s9.s10.int_var10 = 10;
    global_s1.s2.s3.s4.s5.s6.s7.s8.s9.s10.float_var10 = 10.10;
    global_s1.s2.s3.s4.s5.s6.s7.s8.s9.s10.string_pointer10 = "Ten";

    global_s1.s2.s3.s4.s5.s6.s7.s8.s9.s10.s11.int_var11 = 11;
    global_s1.s2.s3.s4.s5.s6.s7.s8.s9.s10.s11.float_var11 = 11.11;
    global_s1.s2.s3.s4.s5.s6.s7.s8.s9.s10.s11.string_pointer11 = "Eleven";

    global_s1.s2.s3.s4.s5.s6.s7.s8.s9.s10.s11.s12.int_var12 = 12;
    global_s1.s2.s3.s4.s5.s6.s7.s8.s9.s10.s11.s12.float_var12 = 12.12;
    global_s1.s2.s3.s4.s5.s6.s7.s8.s9.s10.s11.s12.string_pointer12 = "Twelve";

    global_s1.s2.s3.s4.s5.s6.s7.s8.s9.s10.s11.s12.s13.int_var13 = 13;
    global_s1.s2.s3.s4.s5.s6.s7.s8.s9.s10.s11.s12.s13.float_var13 = 13.13;
    global_s1.s2.s3.s4.s5.s6.s7.s8.s9.s10.s11.s12.s13.string_pointer13 = "Thirteen";

    global_s1.s2.s3.s4.s5.s6.s7.s8.s9.s10.s11.s12.s13.s14.int_var14 = 14;
    global_s1.s2.s3.s4.s5.s6.s7.s8.s9.s10.s11.s12.s13.s14.float_var14 = 14.14;
    global_s1.s2.s3.s4.s5.s6.s7.s8.s9.s10.s11.s12.s13.s14.string_pointer14 = "Fourteen";

    global_s1.s2.s3.s4.s5.s6.s7.s8.s9.s10.s11.s12.s13.s14.s15.int_var15 = 15;
    global_s1.s2.s3.s4.s5.s6.s7.s8.s9.s10.s11.s12.s13.s14.s15.float_var15 = 15.15;
    global_s1.s2.s3.s4.s5.s6.s7.s8.s9.s10.s11.s12.s13.s14.s15.string_pointer15 = "Fifteen";

    global_s1.s2.s3.s4.s5.s6.s7.s8.s9.s10.s11.s12.s13.s14.s15.s16.int_var16 = 16;
    global_s1.s2.s3.s4.s5.s6.s7.s8.s9.s10.s11.s12.s13.s14.s15.s16.float_var16 = 16.16;
    global_s1.s2.s3.s4.s5.s6.s7.s8.s9.s10.s11.s12.s13.s14.s15.s16.string_pointer16 = "Sixteen";

    global_s1.s2.s3.s4.s5.s6.s7.s8.s9.s10.s11.s12.s13.s14.s15.s16.s17.int_var17 = 17;
    global_s1.s2.s3.s4.s5.s6.s7.s8.s9.s10.s11.s12.s13.s14.s15.s16.s17.float_var17 = 17.17;
    global_s1.s2.s3.s4.s5.s6.s7.s8.s9.s10.s11.s12.s13.s14.s15.s16.s17.string_pointer17 = "Seventeen";

    global_s1.s2.s3.s4.s5.s6.s7.s8.s9.s10.s11.s12.s13.s14.s15.s16.s17.s18.int_var18 = 18;
    global_s1.s2.s3.s4.s5.s6.s7.s8.s9.s10.s11.s12.s13.s14.s15.s16.s17.s18.float_var18 = 18.18;
    global_s1.s2.s3.s4.s5.s6.s7.s8.s9.s10.s11.s12.s13.s14.s15.s16.s17.s18.string_pointer18 = "Eighteen";

    global_s1.s2.s3.s4.s5.s6.s7.s8.s9.s10.s11.s12.s13.s14.s15.s16.s17.s18.s19.int_var19 = 19;
    global_s1.s2.s3.s4.s5.s6.s7.s8.s9.s10.s11.s12.s13.s14.s15.s16.s17.s18.s19.float_var19 = 19.19;
    global_s1.s2.s3.s4.s5.s6.s7.s8.s9.s10.s11.s12.s13.s14.s15.s16.s17.s18.s19.string_pointer19 = "Nineteen";

    global_s1.s2.s3.s4.s5.s6.s7.s8.s9.s10.s11.s12.s13.s14.s15.s16.s17.s18.s19.s20.int_var20 = 20;
    global_s1.s2.s3.s4.s5.s6.s7.s8.s9.s10.s11.s12.s13.s14.s15.s16.s17.s18.s19.s20.float_var20 = 20.20;
    global_s1.s2.s3.s4.s5.s6.s7.s8.s9.s10.s11.s12.s13.s14.s15.s16.s17.s18.s19.s20.string_pointer20 = "Twenty";

    global_s1.s2.s3.s4.s5.s6.s7.s8.s9.s10.s11.s12.s13.s14.s15.s16.s17.s18.s19.s20.u1.long_integer = 100;
    global_s1.s2.s3.s4.s5.s6.s7.s8.s9.s10.s11.s12.s13.s14.s15.s16.s17.s18.s19.s20.u1.short_integer = 200;
    global_s1.s2.s3.s4.s5.s6.s7.s8.s9.s10.s11.s12.s13.s14.s15.s16.s17.s18.s19.s20.u1.integer_array[0] = 300;
    global_s1.s2.s3.s4.s5.s6.s7.s8.s9.s10.s11.s12.s13.s14.s15.s16.s17.s18.s19.s20.u1.integer_array[1] = 400;
    global_s1.s2.s3.s4.s5.s6.s7.s8.s9.s10.s11.s12.s13.s14.s15.s16.s17.s18.s19.s20.u1.integer_array[2] = 500;
    global_s1.s2.s3.s4.s5.s6.s7.s8.s9.s10.s11.s12.s13.s14.s15.s16.s17.s18.s19.s20.u1.integer_array[3] = 600;
    global_s1.s2.s3.s4.s5.s6.s7.s8.s9.s10.s11.s12.s13.s14.s15.s16.s17.s18.s19.s20.u1.float_var = 100.1;


    printInfo("Main Process");

    for(t = 0; t < nr_process; t++) {
        printf("Main process fork one process (%ld)\n",t);
        retval = fork();
		if (0 > retval) {
			printf ("\n fork child process i failed  !" );
			exit(1);
		} else if (0 == retval) {
			printProcessInfo(t);
			exit(1);
		} else {
			continue;
		}
    }

    retval = pthread_create(&threads[nr_process], NULL, (VOIDFUNCPTR) alone, 0);
    nr_process++; 


    prctl(PR_SET_NAME, "yzhang0 main", 0, 0, 0);
    while (1) {
        if (verbose) {
            printf ("In main process (pid %d) \n",getpid());
        }
        mid_stack1();
        sleep (sleep_time); 
    }
    printf ("JOIN is done \n");
    exit(0);
}
