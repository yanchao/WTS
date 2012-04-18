/*
 * =====================================================================================
 *
 *       Filename:  mid_stack.c
 *
 *    Description:  mid level complex stack
 *
 *        Version:  1.0
 *        Created:  06/03/2011 10:52:48 AM
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Yanchao Zhang (yzhang0), yanchao.zhang@windriver.com
 *        Company:  WindRiver System
 *
 * =====================================================================================
 */

#include "stack_header.h"
#include <lib/string.c>

REC_TYPE1 q;
REC_TYPE2 rectest;
INNER_STRUCT_TYPE1 s1;

/*-----------------------------------------------------------------------------
 *  forward references/prototypes
 *-----------------------------------------------------------------------------*/
void mid_stack1();
void mid_stack2(REC_TYPE1 *, REC_TYPE2 *, INNER_STRUCT_TYPE1 *);
void mid_stack3(REC_TYPE1 *, REC_TYPE2 *, INNER_STRUCT_TYPE1 *);
void mid_stack4(REC_TYPE1 *, REC_TYPE2 *, INNER_STRUCT_TYPE1 *);
void mid_stack5(REC_TYPE1 *, REC_TYPE2 *, INNER_STRUCT_TYPE1 *);

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  mic_stack1
 *  Description:  show simple local and argument values
 * =====================================================================================
 */
void mid_stack1 ( ) {

    int i;
    UNION_TYPE1 union_array[5];
    day enum_array[5];
    STRUCT_TYPE1 struct_array1[5];
    STRUCT_TYPE2 struct_array2[5];

    REC_TYPE1 local_q;
    REC_TYPE2 local_rectest;
    INNER_STRUCT_TYPE1 local_s1;

    local_q.a = 55;
    strcpy(local_q.b,"December");
    local_q.c = 12345678L;
    local_q.color = red;        

    /* Initialize the rectest structure */  
    local_rectest.long_integer = 0xFFFFEEEE;
    local_rectest.short_integer = 5555;
    local_rectest.integer_array[0] = 0;
    local_rectest.integer_array[1] = 10;
    local_rectest.integer_array[2] = 20;
    local_rectest.integer_array[3] = 30;
    local_rectest.string_pointer = "Wind River's Tools Product Family";

    for (i = 0; i < 5; i++)
    {
        union_array[i].long_integer = i;
        union_array[i].short_integer = i + 10;
        union_array[i].integer_array[0] = i + 100;
        union_array[i].integer_array[1] = i + 200;
        union_array[i].integer_array[2] = i + 300;
        union_array[i].integer_array[3] = i + 400;
        union_array[i].string_pointer = "Wind River's WindPower Product Family";
        union_array[i].float_var = i + 0.10;

        enum_array[i] = i;

        struct_array1[i].long_integer = i + 1000;
        struct_array1[i].short_integer = i + 2000;
        struct_array1[i].integer_array[0] = i + 10;
        struct_array1[i].integer_array[1] = i + 10;
        struct_array1[i].integer_array[2] = i + 10;
        struct_array1[i].integer_array[3] = i + 10;
        struct_array1[i].string_pointer = "Wind River's WindPower Product Family";
        struct_array1[i].float_var = i + 10.10;

        struct_array2[i].a = i + 10;
        struct_array2[i].b[0] = 'W';
        struct_array2[i].b[1] = 'R';
        struct_array2[i].b[2] = 'S';
        struct_array2[i].b[3] = '\0';
        struct_array2[i].c = i + 100;
        struct_array2[i].d1 = sunday;

    }

    /**** ***construct stack frame******/
    mid_stack2 (&local_q,&local_rectest,&local_s1);
    /********basic data type test*******/
}

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  mic_stack2
 *  Description:  show simple local and argument values
 * =====================================================================================
 */
void mid_stack2(REC_TYPE1 *p_q, REC_TYPE2 *p_rectest, INNER_STRUCT_TYPE1 *p_s1) {

    int i;
    UNION_TYPE1 union_array[5];
    day enum_array[5];
    STRUCT_TYPE1 struct_array1[5];
    STRUCT_TYPE2 struct_array2[5];

    REC_TYPE1 local_q;
    REC_TYPE2 local_rectest;
    INNER_STRUCT_TYPE1 local_s1;

    p_q->a = 33;
    strcpy(p_q->b,"November");
    p_q->c = 32165478L;
    p_q->color = blue;        

    /* Initialize the rectest structure */  
    p_rectest->long_integer = 0xFFFFE421;
    p_rectest->short_integer = 3333;
    p_rectest->integer_array[0] = -4;
    p_rectest->integer_array[1] = 14;
    p_rectest->integer_array[2] = 24;
    p_rectest->integer_array[3] = 34;
    p_rectest->string_pointer = "Wind River's Tools Product Family";

    for (i = 0; i < 5; i++)
    {
        union_array[i].long_integer = i;
        union_array[i].short_integer = i + 10;
        union_array[i].integer_array[0] = i + 100;
        union_array[i].integer_array[1] = i + 200;
        union_array[i].integer_array[2] = i + 300;
        union_array[i].integer_array[3] = i + 400;
        union_array[i].string_pointer = "Wind River's WindPower Product Family";
        union_array[i].float_var = i + 0.10;

        enum_array[i] = i;

        struct_array1[i].long_integer = i + 1000;
        struct_array1[i].short_integer = i + 2000;
        struct_array1[i].integer_array[0] = i + 10;
        struct_array1[i].integer_array[1] = i + 10;
        struct_array1[i].integer_array[2] = i + 10;
        struct_array1[i].integer_array[3] = i + 10;
        struct_array1[i].string_pointer = "Wind River's WindPower Product Family";
        struct_array1[i].float_var = i + 10.10;

        struct_array2[i].a = i + 10;
        struct_array2[i].b[0] = 'W';
        struct_array2[i].b[1] = 'R';
        struct_array2[i].b[2] = 'S';
        struct_array2[i].b[3] = '\0';
        struct_array2[i].c = i + 100;
        ++struct_array2[i].d1 ;
    }

    p_s1->int_var1 = 1;
    p_s1->float_var1 = 1.11;
    p_s1->string_pointer1 = "One";

    p_s1->s2.int_var2 = 2;
    p_s1->s2.float_var2 = 2.22;
    p_s1->s2.string_pointer2 = "Two";

    p_s1->s2.s3.int_var3 = 3;
    p_s1->s2.s3.float_var3 = 3.3;
    p_s1->s2.s3.string_pointer3 = "Three";

    p_s1->s2.s3.s4.int_var4 = 4;
    p_s1->s2.s3.s4.float_var4 = 4.4;
    p_s1->s2.s3.s4.string_pointer4 = "Four";

    p_s1->s2.s3.s4.s5.int_var5 = 5;
    p_s1->s2.s3.s4.s5.float_var5 = 5.5;
    p_s1->s2.s3.s4.s5.string_pointer5 = "Five";

    p_s1->s2.s3.s4.s5.s6.int_var6 = 6;
    p_s1->s2.s3.s4.s5.s6.float_var6 = 6.6;
    p_s1->s2.s3.s4.s5.s6.string_pointer6 = "Six";

    p_s1->s2.s3.s4.s5.s6.s7.int_var7 = 7;
    p_s1->s2.s3.s4.s5.s6.s7.float_var7 = 7.7;
    p_s1->s2.s3.s4.s5.s6.s7.string_pointer7 = "Seven";

    p_s1->s2.s3.s4.s5.s6.s7.s8.int_var8 = 8;
    p_s1->s2.s3.s4.s5.s6.s7.s8.float_var8 = 8.8;
    p_s1->s2.s3.s4.s5.s6.s7.s8.string_pointer8 = "Eight";

    p_s1->s2.s3.s4.s5.s6.s7.s8.s9.int_var9 = 9;
    p_s1->s2.s3.s4.s5.s6.s7.s8.s9.float_var9 = 9.9;
    p_s1->s2.s3.s4.s5.s6.s7.s8.s9.string_pointer9 = "Nine";

    p_s1->s2.s3.s4.s5.s6.s7.s8.s9.s10.int_var10 = 10;
    p_s1->s2.s3.s4.s5.s6.s7.s8.s9.s10.float_var10 = 10.10;
    p_s1->s2.s3.s4.s5.s6.s7.s8.s9.s10.string_pointer10 = "Ten";

    p_s1->s2.s3.s4.s5.s6.s7.s8.s9.s10.s11.int_var11 = 11;
    p_s1->s2.s3.s4.s5.s6.s7.s8.s9.s10.s11.float_var11 = 11.11;
    p_s1->s2.s3.s4.s5.s6.s7.s8.s9.s10.s11.string_pointer11 = "Eleven";

    p_s1->s2.s3.s4.s5.s6.s7.s8.s9.s10.s11.s12.int_var12 = 12;
    p_s1->s2.s3.s4.s5.s6.s7.s8.s9.s10.s11.s12.float_var12 = 12.12;
    p_s1->s2.s3.s4.s5.s6.s7.s8.s9.s10.s11.s12.string_pointer12 = "Twelve";

    p_s1->s2.s3.s4.s5.s6.s7.s8.s9.s10.s11.s12.s13.int_var13 = 13;
    p_s1->s2.s3.s4.s5.s6.s7.s8.s9.s10.s11.s12.s13.float_var13 = 13.13;
    p_s1->s2.s3.s4.s5.s6.s7.s8.s9.s10.s11.s12.s13.string_pointer13 = "Thirteen";

    p_s1->s2.s3.s4.s5.s6.s7.s8.s9.s10.s11.s12.s13.s14.int_var14 = 14;
    p_s1->s2.s3.s4.s5.s6.s7.s8.s9.s10.s11.s12.s13.s14.float_var14 = 14.14;
    p_s1->s2.s3.s4.s5.s6.s7.s8.s9.s10.s11.s12.s13.s14.string_pointer14 = "Fourteen";

    p_s1->s2.s3.s4.s5.s6.s7.s8.s9.s10.s11.s12.s13.s14.s15.int_var15 = 15;
    p_s1->s2.s3.s4.s5.s6.s7.s8.s9.s10.s11.s12.s13.s14.s15.float_var15 = 15.15;
    p_s1->s2.s3.s4.s5.s6.s7.s8.s9.s10.s11.s12.s13.s14.s15.string_pointer15 = "Fifteen";

    p_s1->s2.s3.s4.s5.s6.s7.s8.s9.s10.s11.s12.s13.s14.s15.s16.int_var16 = 16;
    p_s1->s2.s3.s4.s5.s6.s7.s8.s9.s10.s11.s12.s13.s14.s15.s16.float_var16 = 16.16;
    p_s1->s2.s3.s4.s5.s6.s7.s8.s9.s10.s11.s12.s13.s14.s15.s16.string_pointer16 = "Sixteen";

    p_s1->s2.s3.s4.s5.s6.s7.s8.s9.s10.s11.s12.s13.s14.s15.s16.s17.int_var17 = 17;
    p_s1->s2.s3.s4.s5.s6.s7.s8.s9.s10.s11.s12.s13.s14.s15.s16.s17.float_var17 = 17.17;
    p_s1->s2.s3.s4.s5.s6.s7.s8.s9.s10.s11.s12.s13.s14.s15.s16.s17.string_pointer17 = "Seventeen";

    p_s1->s2.s3.s4.s5.s6.s7.s8.s9.s10.s11.s12.s13.s14.s15.s16.s17.s18.int_var18 = 18;
    p_s1->s2.s3.s4.s5.s6.s7.s8.s9.s10.s11.s12.s13.s14.s15.s16.s17.s18.float_var18 = 18.18;
    p_s1->s2.s3.s4.s5.s6.s7.s8.s9.s10.s11.s12.s13.s14.s15.s16.s17.s18.string_pointer18 = "Eighteen";

    p_s1->s2.s3.s4.s5.s6.s7.s8.s9.s10.s11.s12.s13.s14.s15.s16.s17.s18.s19.int_var19 = 19;
    p_s1->s2.s3.s4.s5.s6.s7.s8.s9.s10.s11.s12.s13.s14.s15.s16.s17.s18.s19.float_var19 = 19.19;
    p_s1->s2.s3.s4.s5.s6.s7.s8.s9.s10.s11.s12.s13.s14.s15.s16.s17.s18.s19.string_pointer19 = "Nineteen";

    p_s1->s2.s3.s4.s5.s6.s7.s8.s9.s10.s11.s12.s13.s14.s15.s16.s17.s18.s19.s20.int_var20 = 20;
    p_s1->s2.s3.s4.s5.s6.s7.s8.s9.s10.s11.s12.s13.s14.s15.s16.s17.s18.s19.s20.float_var20 = 20.20;
    p_s1->s2.s3.s4.s5.s6.s7.s8.s9.s10.s11.s12.s13.s14.s15.s16.s17.s18.s19.s20.string_pointer20 = "Twenty";

    p_s1->s2.s3.s4.s5.s6.s7.s8.s9.s10.s11.s12.s13.s14.s15.s16.s17.s18.s19.s20.u1.long_integer = 100;
    p_s1->s2.s3.s4.s5.s6.s7.s8.s9.s10.s11.s12.s13.s14.s15.s16.s17.s18.s19.s20.u1.short_integer = 200;
    p_s1->s2.s3.s4.s5.s6.s7.s8.s9.s10.s11.s12.s13.s14.s15.s16.s17.s18.s19.s20.u1.integer_array[0] = 300;
    p_s1->s2.s3.s4.s5.s6.s7.s8.s9.s10.s11.s12.s13.s14.s15.s16.s17.s18.s19.s20.u1.integer_array[1] = 400;
    p_s1->s2.s3.s4.s5.s6.s7.s8.s9.s10.s11.s12.s13.s14.s15.s16.s17.s18.s19.s20.u1.integer_array[2] = 500;
    p_s1->s2.s3.s4.s5.s6.s7.s8.s9.s10.s11.s12.s13.s14.s15.s16.s17.s18.s19.s20.u1.integer_array[3] = 600;
    p_s1->s2.s3.s4.s5.s6.s7.s8.s9.s10.s11.s12.s13.s14.s15.s16.s17.s18.s19.s20.u1.float_var = 100.1;

    /********Complicated structure test*******/

    local_s1.int_var1 = 1;
    local_s1.float_var1 = 1.1;
    local_s1.string_pointer1 = "One";

    local_s1.s2.int_var2 = 2;
    local_s1.s2.float_var2 = 2.2;
    local_s1.s2.string_pointer2 = "Two";

    local_s1.s2.s3.int_var3 = 3;
    local_s1.s2.s3.float_var3 = 3.3;
    local_s1.s2.s3.string_pointer3 = "Three";

    local_s1.s2.s3.s4.int_var4 = 4;
    local_s1.s2.s3.s4.float_var4 = 4.4;
    local_s1.s2.s3.s4.string_pointer4 = "Four";

    local_s1.s2.s3.s4.s5.int_var5 = 5;
    local_s1.s2.s3.s4.s5.float_var5 = 5.5;
    local_s1.s2.s3.s4.s5.string_pointer5 = "Five";

    local_s1.s2.s3.s4.s5.s6.int_var6 = 6;
    local_s1.s2.s3.s4.s5.s6.float_var6 = 6.6;
    local_s1.s2.s3.s4.s5.s6.string_pointer6 = "Six";

    local_s1.s2.s3.s4.s5.s6.s7.int_var7 = 7;
    local_s1.s2.s3.s4.s5.s6.s7.float_var7 = 7.7;
    local_s1.s2.s3.s4.s5.s6.s7.string_pointer7 = "Seven";

    local_s1.s2.s3.s4.s5.s6.s7.s8.int_var8 = 8;
    local_s1.s2.s3.s4.s5.s6.s7.s8.float_var8 = 8.8;
    local_s1.s2.s3.s4.s5.s6.s7.s8.string_pointer8 = "Eight";

    local_s1.s2.s3.s4.s5.s6.s7.s8.s9.int_var9 = 9;
    local_s1.s2.s3.s4.s5.s6.s7.s8.s9.float_var9 = 9.9;
    local_s1.s2.s3.s4.s5.s6.s7.s8.s9.string_pointer9 = "Nine";

    local_s1.s2.s3.s4.s5.s6.s7.s8.s9.s10.int_var10 = 10;
    local_s1.s2.s3.s4.s5.s6.s7.s8.s9.s10.float_var10 = 10.10;
    local_s1.s2.s3.s4.s5.s6.s7.s8.s9.s10.string_pointer10 = "Ten";

    local_s1.s2.s3.s4.s5.s6.s7.s8.s9.s10.s11.int_var11 = 11;
    local_s1.s2.s3.s4.s5.s6.s7.s8.s9.s10.s11.float_var11 = 11.11;
    local_s1.s2.s3.s4.s5.s6.s7.s8.s9.s10.s11.string_pointer11 = "Eleven";

    local_s1.s2.s3.s4.s5.s6.s7.s8.s9.s10.s11.s12.int_var12 = 12;
    local_s1.s2.s3.s4.s5.s6.s7.s8.s9.s10.s11.s12.float_var12 = 12.12;
    local_s1.s2.s3.s4.s5.s6.s7.s8.s9.s10.s11.s12.string_pointer12 = "Twelve";

    local_s1.s2.s3.s4.s5.s6.s7.s8.s9.s10.s11.s12.s13.int_var13 = 13;
    local_s1.s2.s3.s4.s5.s6.s7.s8.s9.s10.s11.s12.s13.float_var13 = 13.13;
    local_s1.s2.s3.s4.s5.s6.s7.s8.s9.s10.s11.s12.s13.string_pointer13 = "Thirteen";

    local_s1.s2.s3.s4.s5.s6.s7.s8.s9.s10.s11.s12.s13.s14.int_var14 = 14;
    local_s1.s2.s3.s4.s5.s6.s7.s8.s9.s10.s11.s12.s13.s14.float_var14 = 14.14;
    local_s1.s2.s3.s4.s5.s6.s7.s8.s9.s10.s11.s12.s13.s14.string_pointer14 = "Fourteen";

    local_s1.s2.s3.s4.s5.s6.s7.s8.s9.s10.s11.s12.s13.s14.s15.int_var15 = 15;
    local_s1.s2.s3.s4.s5.s6.s7.s8.s9.s10.s11.s12.s13.s14.s15.float_var15 = 15.15;
    local_s1.s2.s3.s4.s5.s6.s7.s8.s9.s10.s11.s12.s13.s14.s15.string_pointer15 = "Fifteen";

    local_s1.s2.s3.s4.s5.s6.s7.s8.s9.s10.s11.s12.s13.s14.s15.s16.int_var16 = 16;
    local_s1.s2.s3.s4.s5.s6.s7.s8.s9.s10.s11.s12.s13.s14.s15.s16.float_var16 = 16.16;
    local_s1.s2.s3.s4.s5.s6.s7.s8.s9.s10.s11.s12.s13.s14.s15.s16.string_pointer16 = "Sixteen";

    local_s1.s2.s3.s4.s5.s6.s7.s8.s9.s10.s11.s12.s13.s14.s15.s16.s17.int_var17 = 17;
    local_s1.s2.s3.s4.s5.s6.s7.s8.s9.s10.s11.s12.s13.s14.s15.s16.s17.float_var17 = 17.37;
    local_s1.s2.s3.s4.s5.s6.s7.s8.s9.s10.s11.s12.s13.s14.s15.s16.s17.string_pointer17 = "Seventeen";

    local_s1.s2.s3.s4.s5.s6.s7.s8.s9.s10.s11.s12.s13.s14.s15.s16.s17.s18.int_var18 = 18;
    local_s1.s2.s3.s4.s5.s6.s7.s8.s9.s10.s11.s12.s13.s14.s15.s16.s17.s18.float_var18 = 18.38;
    local_s1.s2.s3.s4.s5.s6.s7.s8.s9.s10.s11.s12.s13.s14.s15.s16.s17.s18.string_pointer18 = "Eighteen";

    local_s1.s2.s3.s4.s5.s6.s7.s8.s9.s10.s11.s12.s13.s14.s15.s16.s17.s18.s19.int_var19 = 19;
    local_s1.s2.s3.s4.s5.s6.s7.s8.s9.s10.s11.s12.s13.s14.s15.s16.s17.s18.s19.float_var19 = 19.39;
    local_s1.s2.s3.s4.s5.s6.s7.s8.s9.s10.s11.s12.s13.s14.s15.s16.s17.s18.s19.string_pointer19 = "Nineteen";

    local_s1.s2.s3.s4.s5.s6.s7.s8.s9.s10.s11.s12.s13.s14.s15.s16.s17.s18.s19.s20.int_var20 = 20;
    local_s1.s2.s3.s4.s5.s6.s7.s8.s9.s10.s11.s12.s13.s14.s15.s16.s17.s18.s19.s20.float_var20 = 20.23;
    local_s1.s2.s3.s4.s5.s6.s7.s8.s9.s10.s11.s12.s13.s14.s15.s16.s17.s18.s19.s20.string_pointer20 = "Twenty";

    local_s1.s2.s3.s4.s5.s6.s7.s8.s9.s10.s11.s12.s13.s14.s15.s16.s17.s18.s19.s20.u1.long_integer = 100;
    local_s1.s2.s3.s4.s5.s6.s7.s8.s9.s10.s11.s12.s13.s14.s15.s16.s17.s18.s19.s20.u1.short_integer = 200;
    local_s1.s2.s3.s4.s5.s6.s7.s8.s9.s10.s11.s12.s13.s14.s15.s16.s17.s18.s19.s20.u1.integer_array[0] = 300;
    local_s1.s2.s3.s4.s5.s6.s7.s8.s9.s10.s11.s12.s13.s14.s15.s16.s17.s18.s19.s20.u1.integer_array[1] = 400;
    local_s1.s2.s3.s4.s5.s6.s7.s8.s9.s10.s11.s12.s13.s14.s15.s16.s17.s18.s19.s20.u1.integer_array[2] = 500;
    local_s1.s2.s3.s4.s5.s6.s7.s8.s9.s10.s11.s12.s13.s14.s15.s16.s17.s18.s19.s20.u1.integer_array[3] = 600;
    local_s1.s2.s3.s4.s5.s6.s7.s8.s9.s10.s11.s12.s13.s14.s15.s16.s17.s18.s19.s20.u1.float_var = 100.1;

    /**** ***construct stack frame******/
    mid_stack3 (&local_q,&local_rectest,&local_s1);
    /********basic data type test*******/
}

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  mic_stack3
 *  Description:  show simple local and argument values
 * =====================================================================================
 */
void mid_stack3(REC_TYPE1 *p_q, REC_TYPE2 *p_rectest, INNER_STRUCT_TYPE1 *p_s1) {

    int i;
    UNION_TYPE1 union_array[5];
    day enum_array[5];
    STRUCT_TYPE1 struct_array1[5];
    STRUCT_TYPE2 struct_array2[5];

    p_q->a = 33;
    strcpy(p_q->b,"November");
    p_q->c = 32165478L;
    p_q->color = blue;        

    /* Initialize the rectest structure */  
    p_rectest->long_integer = 0xFFFFE421;
    p_rectest->short_integer = 3333;
    p_rectest->integer_array[0] = -4;
    p_rectest->integer_array[1] = 14;
    p_rectest->integer_array[2] = 24;
    p_rectest->integer_array[3] = 34;
    p_rectest->string_pointer = "Wind River's Tools Product Family";


    q.a += 55;
    strcpy(q.b,"April");
    q.c += 12345678L;
    q.color = blue;        

    /* Initialize the rectest structure */  
    rectest.long_integer = 0xFFFFEEEE;
    rectest.short_integer = 5555;
    rectest.integer_array[0] = 0;
    rectest.integer_array[1] = 10;
    rectest.integer_array[2] = 20;
    rectest.integer_array[3] = 30;
    rectest.string_pointer = "Wind River's Tools Product Family";

    for (i = 0; i < 5; i++)
    {
        union_array[i].long_integer = i;
        union_array[i].short_integer = i + 10;
        union_array[i].integer_array[0] = i + 100;
        union_array[i].integer_array[1] = i + 200;
        union_array[i].integer_array[2] = i + 300;
        union_array[i].integer_array[3] = i + 400;
        union_array[i].string_pointer = "Wind River's WindPower Product Family";
        union_array[i].float_var = i + 0.10;

        enum_array[i] = i;

        struct_array1[i].long_integer = i + 1000;
        struct_array1[i].short_integer = i + 2000;
        struct_array1[i].integer_array[0] = i + 10;
        struct_array1[i].integer_array[1] = i + 10;
        struct_array1[i].integer_array[2] = i + 10;
        struct_array1[i].integer_array[3] = i + 10;
        struct_array1[i].string_pointer = "Wind River's WindPower Product Family";
        struct_array1[i].float_var = i + 10.10;

        struct_array2[i].a = i + 10;
        struct_array2[i].b[0] = 'W';
        struct_array2[i].b[1] = 'R';
        struct_array2[i].b[2] = 'S';
        struct_array2[i].b[3] = '\0';
        struct_array2[i].c = i + 100;
        ++struct_array2[i].d1 ;
    }

    p_s1->int_var1 = 1;
    p_s1->float_var1 = 1.11;
    p_s1->string_pointer1 = "One";

    p_s1->s2.int_var2 = 2;
    p_s1->s2.float_var2 = 2.22;
    p_s1->s2.string_pointer2 = "Two";

    p_s1->s2.s3.int_var3 = 3;
    p_s1->s2.s3.float_var3 = 3.3;
    p_s1->s2.s3.string_pointer3 = "Three";

    p_s1->s2.s3.s4.int_var4 = 4;
    p_s1->s2.s3.s4.float_var4 = 4.4;
    p_s1->s2.s3.s4.string_pointer4 = "Four";

    p_s1->s2.s3.s4.s5.int_var5 = 5;
    p_s1->s2.s3.s4.s5.float_var5 = 5.5;
    p_s1->s2.s3.s4.s5.string_pointer5 = "Five";

    p_s1->s2.s3.s4.s5.s6.int_var6 = 6;
    p_s1->s2.s3.s4.s5.s6.float_var6 = 6.6;
    p_s1->s2.s3.s4.s5.s6.string_pointer6 = "Six";

    p_s1->s2.s3.s4.s5.s6.s7.int_var7 = 7;
    p_s1->s2.s3.s4.s5.s6.s7.float_var7 = 7.7;
    p_s1->s2.s3.s4.s5.s6.s7.string_pointer7 = "Seven";

    p_s1->s2.s3.s4.s5.s6.s7.s8.int_var8 = 8;
    p_s1->s2.s3.s4.s5.s6.s7.s8.float_var8 = 8.8;
    p_s1->s2.s3.s4.s5.s6.s7.s8.string_pointer8 = "Eight";

    p_s1->s2.s3.s4.s5.s6.s7.s8.s9.int_var9 = 9;
    p_s1->s2.s3.s4.s5.s6.s7.s8.s9.float_var9 = 9.9;
    p_s1->s2.s3.s4.s5.s6.s7.s8.s9.string_pointer9 = "Nine";

    p_s1->s2.s3.s4.s5.s6.s7.s8.s9.s10.int_var10 = 10;
    p_s1->s2.s3.s4.s5.s6.s7.s8.s9.s10.float_var10 = 10.10;
    p_s1->s2.s3.s4.s5.s6.s7.s8.s9.s10.string_pointer10 = "Ten";

    p_s1->s2.s3.s4.s5.s6.s7.s8.s9.s10.s11.int_var11 = 11;
    p_s1->s2.s3.s4.s5.s6.s7.s8.s9.s10.s11.float_var11 = 11.11;
    p_s1->s2.s3.s4.s5.s6.s7.s8.s9.s10.s11.string_pointer11 = "Eleven";

    p_s1->s2.s3.s4.s5.s6.s7.s8.s9.s10.s11.s12.int_var12 = 12;
    p_s1->s2.s3.s4.s5.s6.s7.s8.s9.s10.s11.s12.float_var12 = 12.12;
    p_s1->s2.s3.s4.s5.s6.s7.s8.s9.s10.s11.s12.string_pointer12 = "Twelve";

    p_s1->s2.s3.s4.s5.s6.s7.s8.s9.s10.s11.s12.s13.int_var13 = 13;
    p_s1->s2.s3.s4.s5.s6.s7.s8.s9.s10.s11.s12.s13.float_var13 = 13.13;
    p_s1->s2.s3.s4.s5.s6.s7.s8.s9.s10.s11.s12.s13.string_pointer13 = "Thirteen";

    p_s1->s2.s3.s4.s5.s6.s7.s8.s9.s10.s11.s12.s13.s14.int_var14 = 14;
    p_s1->s2.s3.s4.s5.s6.s7.s8.s9.s10.s11.s12.s13.s14.float_var14 = 14.14;
    p_s1->s2.s3.s4.s5.s6.s7.s8.s9.s10.s11.s12.s13.s14.string_pointer14 = "Fourteen";

    p_s1->s2.s3.s4.s5.s6.s7.s8.s9.s10.s11.s12.s13.s14.s15.int_var15 = 15;
    p_s1->s2.s3.s4.s5.s6.s7.s8.s9.s10.s11.s12.s13.s14.s15.float_var15 = 15.15;
    p_s1->s2.s3.s4.s5.s6.s7.s8.s9.s10.s11.s12.s13.s14.s15.string_pointer15 = "Fifteen";

    p_s1->s2.s3.s4.s5.s6.s7.s8.s9.s10.s11.s12.s13.s14.s15.s16.int_var16 = 16;
    p_s1->s2.s3.s4.s5.s6.s7.s8.s9.s10.s11.s12.s13.s14.s15.s16.float_var16 = 16.16;
    p_s1->s2.s3.s4.s5.s6.s7.s8.s9.s10.s11.s12.s13.s14.s15.s16.string_pointer16 = "Sixteen";

    p_s1->s2.s3.s4.s5.s6.s7.s8.s9.s10.s11.s12.s13.s14.s15.s16.s17.int_var17 = 17;
    p_s1->s2.s3.s4.s5.s6.s7.s8.s9.s10.s11.s12.s13.s14.s15.s16.s17.float_var17 = 17.17;
    p_s1->s2.s3.s4.s5.s6.s7.s8.s9.s10.s11.s12.s13.s14.s15.s16.s17.string_pointer17 = "Seventeen";

    p_s1->s2.s3.s4.s5.s6.s7.s8.s9.s10.s11.s12.s13.s14.s15.s16.s17.s18.int_var18 = 18;
    p_s1->s2.s3.s4.s5.s6.s7.s8.s9.s10.s11.s12.s13.s14.s15.s16.s17.s18.float_var18 = 18.18;
    p_s1->s2.s3.s4.s5.s6.s7.s8.s9.s10.s11.s12.s13.s14.s15.s16.s17.s18.string_pointer18 = "Eighteen";

    p_s1->s2.s3.s4.s5.s6.s7.s8.s9.s10.s11.s12.s13.s14.s15.s16.s17.s18.s19.int_var19 = 19;
    p_s1->s2.s3.s4.s5.s6.s7.s8.s9.s10.s11.s12.s13.s14.s15.s16.s17.s18.s19.float_var19 = 19.19;
    p_s1->s2.s3.s4.s5.s6.s7.s8.s9.s10.s11.s12.s13.s14.s15.s16.s17.s18.s19.string_pointer19 = "Nineteen";

    p_s1->s2.s3.s4.s5.s6.s7.s8.s9.s10.s11.s12.s13.s14.s15.s16.s17.s18.s19.s20.int_var20 = 20;
    p_s1->s2.s3.s4.s5.s6.s7.s8.s9.s10.s11.s12.s13.s14.s15.s16.s17.s18.s19.s20.float_var20 = 20.20;
    p_s1->s2.s3.s4.s5.s6.s7.s8.s9.s10.s11.s12.s13.s14.s15.s16.s17.s18.s19.s20.string_pointer20 = "Twenty";

    p_s1->s2.s3.s4.s5.s6.s7.s8.s9.s10.s11.s12.s13.s14.s15.s16.s17.s18.s19.s20.u1.long_integer = 100;
    p_s1->s2.s3.s4.s5.s6.s7.s8.s9.s10.s11.s12.s13.s14.s15.s16.s17.s18.s19.s20.u1.short_integer = 200;
    p_s1->s2.s3.s4.s5.s6.s7.s8.s9.s10.s11.s12.s13.s14.s15.s16.s17.s18.s19.s20.u1.integer_array[0] = 300;
    p_s1->s2.s3.s4.s5.s6.s7.s8.s9.s10.s11.s12.s13.s14.s15.s16.s17.s18.s19.s20.u1.integer_array[1] = 400;
    p_s1->s2.s3.s4.s5.s6.s7.s8.s9.s10.s11.s12.s13.s14.s15.s16.s17.s18.s19.s20.u1.integer_array[2] = 500;
    p_s1->s2.s3.s4.s5.s6.s7.s8.s9.s10.s11.s12.s13.s14.s15.s16.s17.s18.s19.s20.u1.integer_array[3] = 600;
    p_s1->s2.s3.s4.s5.s6.s7.s8.s9.s10.s11.s12.s13.s14.s15.s16.s17.s18.s19.s20.u1.float_var = 100.1;

    /**** ***construct stack frame******/
    mid_stack4 (p_q,p_rectest,p_s1);
    /********basic data type test*******/
}

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  mic_stack4
 *  Description:  show simple local and argument values
 * =====================================================================================
 */
void mid_stack4(REC_TYPE1 *p_q, REC_TYPE2 *p_rectest, INNER_STRUCT_TYPE1 *p_s1) {

    int i;
    UNION_TYPE1 union_array[5];
    day enum_array[5];
    STRUCT_TYPE1 struct_array1[5];
    STRUCT_TYPE2 struct_array2[5];

    q.a += 55;
    strcpy(q.b,"April");
    q.c += 12345678L;
    q.color = blue;        

    /* Initialize the rectest structure */  
    rectest.long_integer = 0xFFFFEEEE;
    rectest.short_integer = 5555;
    rectest.integer_array[0] = 0;
    rectest.integer_array[1] = 10;
    rectest.integer_array[2] = 20;
    rectest.integer_array[3] = 30;
    rectest.string_pointer = "Wind River's Tools Product Family";

    p_q->a = 33;
    strcpy(p_q->b,"November");
    p_q->c = 32165478L;
    p_q->color = blue;        

    /* Initialize the rectest structure */  
    p_rectest->long_integer = 0xFFFFE421;
    p_rectest->short_integer = 3333;
    p_rectest->integer_array[0] = -4;
    p_rectest->integer_array[1] = 14;
    p_rectest->integer_array[2] = 24;
    p_rectest->integer_array[3] = 34;
    p_rectest->string_pointer = "Wind River's Tools Product Family";


    for (i = 0; i < 5; i++)
    {
        union_array[i].long_integer = i;
        union_array[i].short_integer = i + 10;
        union_array[i].integer_array[0] = i + 100;
        union_array[i].integer_array[1] = i + 200;
        union_array[i].integer_array[2] = i + 300;
        union_array[i].integer_array[3] = i + 400;
        union_array[i].string_pointer = "Wind River's WindPower Product Family";
        union_array[i].float_var = i + 0.10;

        enum_array[i] = i;

        struct_array1[i].long_integer = i + 1000;
        struct_array1[i].short_integer = i + 2000;
        struct_array1[i].integer_array[0] = i + 10;
        struct_array1[i].integer_array[1] = i + 10;
        struct_array1[i].integer_array[2] = i + 10;
        struct_array1[i].integer_array[3] = i + 10;
        struct_array1[i].string_pointer = "Wind River's WindPower Product Family";
        struct_array1[i].float_var = i + 10.10;

        struct_array2[i].a = i + 10;
        struct_array2[i].b[0] = 'W';
        struct_array2[i].b[1] = 'R';
        struct_array2[i].b[2] = 'S';
        struct_array2[i].b[3] = '\0';
        struct_array2[i].c = i + 100;
        struct_array2[i].d1 = sunday;
    }

    p_s1->int_var1 = 1;
    p_s1->float_var1 = 1.11;
    p_s1->string_pointer1 = "One";

    p_s1->s2.int_var2 = 2;
    p_s1->s2.float_var2 = 2.22;
    p_s1->s2.string_pointer2 = "Two";

    p_s1->s2.s3.int_var3 = 3;
    p_s1->s2.s3.float_var3 = 3.3;
    p_s1->s2.s3.string_pointer3 = "Three";

    p_s1->s2.s3.s4.int_var4 = 4;
    p_s1->s2.s3.s4.float_var4 = 4.4;
    p_s1->s2.s3.s4.string_pointer4 = "Four";

    p_s1->s2.s3.s4.s5.int_var5 = 5;
    p_s1->s2.s3.s4.s5.float_var5 = 5.5;
    p_s1->s2.s3.s4.s5.string_pointer5 = "Five";

    p_s1->s2.s3.s4.s5.s6.int_var6 = 6;
    p_s1->s2.s3.s4.s5.s6.float_var6 = 6.6;
    p_s1->s2.s3.s4.s5.s6.string_pointer6 = "Six";

    p_s1->s2.s3.s4.s5.s6.s7.int_var7 = 7;
    p_s1->s2.s3.s4.s5.s6.s7.float_var7 = 7.7;
    p_s1->s2.s3.s4.s5.s6.s7.string_pointer7 = "Seven";

    p_s1->s2.s3.s4.s5.s6.s7.s8.int_var8 = 8;
    p_s1->s2.s3.s4.s5.s6.s7.s8.float_var8 = 8.8;
    p_s1->s2.s3.s4.s5.s6.s7.s8.string_pointer8 = "Eight";

    p_s1->s2.s3.s4.s5.s6.s7.s8.s9.int_var9 = 9;
    p_s1->s2.s3.s4.s5.s6.s7.s8.s9.float_var9 = 9.9;
    p_s1->s2.s3.s4.s5.s6.s7.s8.s9.string_pointer9 = "Nine";

    p_s1->s2.s3.s4.s5.s6.s7.s8.s9.s10.int_var10 = 10;
    p_s1->s2.s3.s4.s5.s6.s7.s8.s9.s10.float_var10 = 10.10;
    p_s1->s2.s3.s4.s5.s6.s7.s8.s9.s10.string_pointer10 = "Ten";

    p_s1->s2.s3.s4.s5.s6.s7.s8.s9.s10.s11.int_var11 = 11;
    p_s1->s2.s3.s4.s5.s6.s7.s8.s9.s10.s11.float_var11 = 11.11;
    p_s1->s2.s3.s4.s5.s6.s7.s8.s9.s10.s11.string_pointer11 = "Eleven";

    p_s1->s2.s3.s4.s5.s6.s7.s8.s9.s10.s11.s12.int_var12 = 12;
    p_s1->s2.s3.s4.s5.s6.s7.s8.s9.s10.s11.s12.float_var12 = 12.12;
    p_s1->s2.s3.s4.s5.s6.s7.s8.s9.s10.s11.s12.string_pointer12 = "Twelve";

    p_s1->s2.s3.s4.s5.s6.s7.s8.s9.s10.s11.s12.s13.int_var13 = 13;
    p_s1->s2.s3.s4.s5.s6.s7.s8.s9.s10.s11.s12.s13.float_var13 = 13.13;
    p_s1->s2.s3.s4.s5.s6.s7.s8.s9.s10.s11.s12.s13.string_pointer13 = "Thirteen";

    p_s1->s2.s3.s4.s5.s6.s7.s8.s9.s10.s11.s12.s13.s14.int_var14 = 14;
    p_s1->s2.s3.s4.s5.s6.s7.s8.s9.s10.s11.s12.s13.s14.float_var14 = 14.14;
    p_s1->s2.s3.s4.s5.s6.s7.s8.s9.s10.s11.s12.s13.s14.string_pointer14 = "Fourteen";

    p_s1->s2.s3.s4.s5.s6.s7.s8.s9.s10.s11.s12.s13.s14.s15.int_var15 = 15;
    p_s1->s2.s3.s4.s5.s6.s7.s8.s9.s10.s11.s12.s13.s14.s15.float_var15 = 15.15;
    p_s1->s2.s3.s4.s5.s6.s7.s8.s9.s10.s11.s12.s13.s14.s15.string_pointer15 = "Fifteen";

    p_s1->s2.s3.s4.s5.s6.s7.s8.s9.s10.s11.s12.s13.s14.s15.s16.int_var16 = 16;
    p_s1->s2.s3.s4.s5.s6.s7.s8.s9.s10.s11.s12.s13.s14.s15.s16.float_var16 = 16.16;
    p_s1->s2.s3.s4.s5.s6.s7.s8.s9.s10.s11.s12.s13.s14.s15.s16.string_pointer16 = "Sixteen";

    p_s1->s2.s3.s4.s5.s6.s7.s8.s9.s10.s11.s12.s13.s14.s15.s16.s17.int_var17 = 17;
    p_s1->s2.s3.s4.s5.s6.s7.s8.s9.s10.s11.s12.s13.s14.s15.s16.s17.float_var17 = 17.17;
    p_s1->s2.s3.s4.s5.s6.s7.s8.s9.s10.s11.s12.s13.s14.s15.s16.s17.string_pointer17 = "Seventeen";

    p_s1->s2.s3.s4.s5.s6.s7.s8.s9.s10.s11.s12.s13.s14.s15.s16.s17.s18.int_var18 = 18;
    p_s1->s2.s3.s4.s5.s6.s7.s8.s9.s10.s11.s12.s13.s14.s15.s16.s17.s18.float_var18 = 18.18;
    p_s1->s2.s3.s4.s5.s6.s7.s8.s9.s10.s11.s12.s13.s14.s15.s16.s17.s18.string_pointer18 = "Eighteen";

    p_s1->s2.s3.s4.s5.s6.s7.s8.s9.s10.s11.s12.s13.s14.s15.s16.s17.s18.s19.int_var19 = 19;
    p_s1->s2.s3.s4.s5.s6.s7.s8.s9.s10.s11.s12.s13.s14.s15.s16.s17.s18.s19.float_var19 = 19.19;
    p_s1->s2.s3.s4.s5.s6.s7.s8.s9.s10.s11.s12.s13.s14.s15.s16.s17.s18.s19.string_pointer19 = "Nineteen";

    p_s1->s2.s3.s4.s5.s6.s7.s8.s9.s10.s11.s12.s13.s14.s15.s16.s17.s18.s19.s20.int_var20 = 20;
    p_s1->s2.s3.s4.s5.s6.s7.s8.s9.s10.s11.s12.s13.s14.s15.s16.s17.s18.s19.s20.float_var20 = 20.20;
    p_s1->s2.s3.s4.s5.s6.s7.s8.s9.s10.s11.s12.s13.s14.s15.s16.s17.s18.s19.s20.string_pointer20 = "Twenty";

    p_s1->s2.s3.s4.s5.s6.s7.s8.s9.s10.s11.s12.s13.s14.s15.s16.s17.s18.s19.s20.u1.long_integer = 100;
    p_s1->s2.s3.s4.s5.s6.s7.s8.s9.s10.s11.s12.s13.s14.s15.s16.s17.s18.s19.s20.u1.short_integer = 200;
    p_s1->s2.s3.s4.s5.s6.s7.s8.s9.s10.s11.s12.s13.s14.s15.s16.s17.s18.s19.s20.u1.integer_array[0] = 300;
    p_s1->s2.s3.s4.s5.s6.s7.s8.s9.s10.s11.s12.s13.s14.s15.s16.s17.s18.s19.s20.u1.integer_array[1] = 400;
    p_s1->s2.s3.s4.s5.s6.s7.s8.s9.s10.s11.s12.s13.s14.s15.s16.s17.s18.s19.s20.u1.integer_array[2] = 500;
    p_s1->s2.s3.s4.s5.s6.s7.s8.s9.s10.s11.s12.s13.s14.s15.s16.s17.s18.s19.s20.u1.integer_array[3] = 600;
    p_s1->s2.s3.s4.s5.s6.s7.s8.s9.s10.s11.s12.s13.s14.s15.s16.s17.s18.s19.s20.u1.float_var = 100.1;


    /**** ***construct stack frame******/
    mid_stack5 (&q,&rectest,&s1);
    /********basic data type test*******/
}

/* 
 * ===  FUNCTION  ======================================================================
 *         Name:  mic_stack5
 *  Description:  show simple local and argument values
 * =====================================================================================
 */
void mid_stack5 (REC_TYPE1 *p_q, REC_TYPE2 *p_rectest, INNER_STRUCT_TYPE1 *p_s1) {

    int i;
    day enum_array[5];
    STRUCT_TYPE1 struct_array1[5];
    STRUCT_TYPE2 struct_array2[5];

    p_q->a += 55;
    strcpy(p_q->b,"April");
    p_q->c += 32451678L;
    p_q->color = blue;        

    /* Initialize the rectest structure */  
    p_rectest->long_integer = 0xFFFFEEEE;
    p_rectest->short_integer = 5555;
    p_rectest->integer_array[0] = 0;
    p_rectest->integer_array[1] = 10;
    p_rectest->integer_array[2] = 20;
    p_rectest->integer_array[3] = 30;
    p_rectest->string_pointer = "Wind River's Tools Product Family";

    for (i = 0; i < 5; i++)
    {
        enum_array[i] = i;

        struct_array1[i].long_integer = i + 1000;
        struct_array1[i].short_integer = i + 2000;
        struct_array1[i].integer_array[0] = i + 10;
        struct_array1[i].integer_array[1] = i + 10;
        struct_array1[i].integer_array[2] = i + 10;
        struct_array1[i].integer_array[3] = i + 10;
        struct_array1[i].string_pointer = "Wind River's WindPower Product Family";
        struct_array1[i].float_var = i + 10.10;

        struct_array2[i].a = i + 10;
        struct_array2[i].b[0] = 'W';
        struct_array2[i].b[1] = 'R';
        struct_array2[i].b[2] = 'S';
        struct_array2[i].b[3] = '\0';
        struct_array2[i].c = i + 100;
        struct_array2[i].d1 = sunday;
    }

    p_s1->int_var1 = 1;
    p_s1->float_var1 = 1.1;
    p_s1->string_pointer1 = "One";

    p_s1->s2.int_var2 = 2;
    p_s1->s2.float_var2 = 2.2;
    p_s1->s2.string_pointer2 = "Two";

    p_s1->s2.s3.int_var3 = 3;
    p_s1->s2.s3.float_var3 = 3.3;
    p_s1->s2.s3.string_pointer3 = "Three";

    p_s1->s2.s3.s4.int_var4 = 4;
    p_s1->s2.s3.s4.float_var4 = 4.4;
    p_s1->s2.s3.s4.string_pointer4 = "Four";

    p_s1->s2.s3.s4.s5.int_var5 = 5;
    p_s1->s2.s3.s4.s5.float_var5 = 5.5;
    p_s1->s2.s3.s4.s5.string_pointer5 = "Five";

    p_s1->s2.s3.s4.s5.s6.int_var6 = 6;
    p_s1->s2.s3.s4.s5.s6.float_var6 = 6.6;
    p_s1->s2.s3.s4.s5.s6.string_pointer6 = "Six";

    p_s1->s2.s3.s4.s5.s6.s7.int_var7 = 7;
    p_s1->s2.s3.s4.s5.s6.s7.float_var7 = 7.7;
    p_s1->s2.s3.s4.s5.s6.s7.string_pointer7 = "Seven";

    p_s1->s2.s3.s4.s5.s6.s7.s8.int_var8 = 8;
    p_s1->s2.s3.s4.s5.s6.s7.s8.float_var8 = 8.8;
    p_s1->s2.s3.s4.s5.s6.s7.s8.string_pointer8 = "Eight";

    p_s1->s2.s3.s4.s5.s6.s7.s8.s9.int_var9 = 9;
    p_s1->s2.s3.s4.s5.s6.s7.s8.s9.float_var9 = 9.9;
    p_s1->s2.s3.s4.s5.s6.s7.s8.s9.string_pointer9 = "Nine";

    p_s1->s2.s3.s4.s5.s6.s7.s8.s9.s10.int_var10 = 10;
    p_s1->s2.s3.s4.s5.s6.s7.s8.s9.s10.float_var10 = 10.10;
    p_s1->s2.s3.s4.s5.s6.s7.s8.s9.s10.string_pointer10 = "Ten";

    p_s1->s2.s3.s4.s5.s6.s7.s8.s9.s10.s11.int_var11 = 11;
    p_s1->s2.s3.s4.s5.s6.s7.s8.s9.s10.s11.float_var11 = 11.11;
    p_s1->s2.s3.s4.s5.s6.s7.s8.s9.s10.s11.string_pointer11 = "Eleven";

    p_s1->s2.s3.s4.s5.s6.s7.s8.s9.s10.s11.s12.int_var12 = 12;
    p_s1->s2.s3.s4.s5.s6.s7.s8.s9.s10.s11.s12.float_var12 = 12.12;
    p_s1->s2.s3.s4.s5.s6.s7.s8.s9.s10.s11.s12.string_pointer12 = "Twelve";

    p_s1->s2.s3.s4.s5.s6.s7.s8.s9.s10.s11.s12.s13.int_var13 = 13;
    p_s1->s2.s3.s4.s5.s6.s7.s8.s9.s10.s11.s12.s13.float_var13 = 13.13;
    p_s1->s2.s3.s4.s5.s6.s7.s8.s9.s10.s11.s12.s13.string_pointer13 = "Thirteen";

    p_s1->s2.s3.s4.s5.s6.s7.s8.s9.s10.s11.s12.s13.s14.int_var14 = 14;
    p_s1->s2.s3.s4.s5.s6.s7.s8.s9.s10.s11.s12.s13.s14.float_var14 = 14.14;
    p_s1->s2.s3.s4.s5.s6.s7.s8.s9.s10.s11.s12.s13.s14.string_pointer14 = "Fourteen";

    p_s1->s2.s3.s4.s5.s6.s7.s8.s9.s10.s11.s12.s13.s14.s15.int_var15 = 15;
    p_s1->s2.s3.s4.s5.s6.s7.s8.s9.s10.s11.s12.s13.s14.s15.float_var15 = 15.15;
    p_s1->s2.s3.s4.s5.s6.s7.s8.s9.s10.s11.s12.s13.s14.s15.string_pointer15 = "Fifteen";

    p_s1->s2.s3.s4.s5.s6.s7.s8.s9.s10.s11.s12.s13.s14.s15.s16.int_var16 = 16;
    p_s1->s2.s3.s4.s5.s6.s7.s8.s9.s10.s11.s12.s13.s14.s15.s16.float_var16 = 16.16;
    p_s1->s2.s3.s4.s5.s6.s7.s8.s9.s10.s11.s12.s13.s14.s15.s16.string_pointer16 = "Sixteen";

    p_s1->s2.s3.s4.s5.s6.s7.s8.s9.s10.s11.s12.s13.s14.s15.s16.s17.int_var17 = 17;
    p_s1->s2.s3.s4.s5.s6.s7.s8.s9.s10.s11.s12.s13.s14.s15.s16.s17.float_var17 = 17.17;
    p_s1->s2.s3.s4.s5.s6.s7.s8.s9.s10.s11.s12.s13.s14.s15.s16.s17.string_pointer17 = "Seventeen";

    p_s1->s2.s3.s4.s5.s6.s7.s8.s9.s10.s11.s12.s13.s14.s15.s16.s17.s18.int_var18 = 18;
    p_s1->s2.s3.s4.s5.s6.s7.s8.s9.s10.s11.s12.s13.s14.s15.s16.s17.s18.float_var18 = 18.18;
    p_s1->s2.s3.s4.s5.s6.s7.s8.s9.s10.s11.s12.s13.s14.s15.s16.s17.s18.string_pointer18 = "Eighteen";

    p_s1->s2.s3.s4.s5.s6.s7.s8.s9.s10.s11.s12.s13.s14.s15.s16.s17.s18.s19.int_var19 = 19;
    p_s1->s2.s3.s4.s5.s6.s7.s8.s9.s10.s11.s12.s13.s14.s15.s16.s17.s18.s19.float_var19 = 19.19;
    p_s1->s2.s3.s4.s5.s6.s7.s8.s9.s10.s11.s12.s13.s14.s15.s16.s17.s18.s19.string_pointer19 = "Nineteen";

    p_s1->s2.s3.s4.s5.s6.s7.s8.s9.s10.s11.s12.s13.s14.s15.s16.s17.s18.s19.s20.int_var20 = 20;
    p_s1->s2.s3.s4.s5.s6.s7.s8.s9.s10.s11.s12.s13.s14.s15.s16.s17.s18.s19.s20.float_var20 = 20.20;
    p_s1->s2.s3.s4.s5.s6.s7.s8.s9.s10.s11.s12.s13.s14.s15.s16.s17.s18.s19.s20.string_pointer20 = "Twenty";

    p_s1->s2.s3.s4.s5.s6.s7.s8.s9.s10.s11.s12.s13.s14.s15.s16.s17.s18.s19.s20.u1.long_integer = 100;
    p_s1->s2.s3.s4.s5.s6.s7.s8.s9.s10.s11.s12.s13.s14.s15.s16.s17.s18.s19.s20.u1.short_integer = 200;
    p_s1->s2.s3.s4.s5.s6.s7.s8.s9.s10.s11.s12.s13.s14.s15.s16.s17.s18.s19.s20.u1.integer_array[0] = 300;
    p_s1->s2.s3.s4.s5.s6.s7.s8.s9.s10.s11.s12.s13.s14.s15.s16.s17.s18.s19.s20.u1.integer_array[1] = 400;
    p_s1->s2.s3.s4.s5.s6.s7.s8.s9.s10.s11.s12.s13.s14.s15.s16.s17.s18.s19.s20.u1.integer_array[2] = 500;
    p_s1->s2.s3.s4.s5.s6.s7.s8.s9.s10.s11.s12.s13.s14.s15.s16.s17.s18.s19.s20.u1.integer_array[3] = 600;
    p_s1->s2.s3.s4.s5.s6.s7.s8.s9.s10.s11.s12.s13.s14.s15.s16.s17.s18.s19.s20.u1.float_var = 100.1;

}
