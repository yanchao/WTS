/*
 * =====================================================================================
 *
 *       Filename:  stackTest.h
 *
 *    Description:  for debug purpose
 *
 *        Version:  1.0
 *        Created:  06/03/2011
 *       Revision:  none
 *       Compiler:  
 *
 *         Author:  Yanchao Zhang (yzhang0), yanchao.zhang@windriver.com
 *        Company:  WindRiver System
 *
 * =====================================================================================
 */
#ifndef _STACK_HEADER_H
#define _STACK_HEADER_H

typedef enum {sunday, monday, tuesday, wednesday, thursday, friday, saturday} day;

typedef struct {        
	  long long_integer;
	  short short_integer;
	  int integer_array[4];
	  char *string_pointer;
      float float_var;
    } STRUCT_TYPE1;

typedef struct {
	  short   a;
	  char    b[4];
	  long    c;
	  day d1;
	} STRUCT_TYPE2;
	
typedef union {        
	  long long_integer;
	  short short_integer;
	  int integer_array[4];
	  char *string_pointer;
	  float float_var;
	} UNION_TYPE1;
	

typedef struct {
	  int 	int_var20;
	  float float_var20;
	  char *string_pointer20;
  
	  UNION_TYPE1 u1;
	} INNER_STRUCT_TYPE20;

typedef struct {
	  int 	int_var19;
	  float float_var19;
	  char *string_pointer19;
	  INNER_STRUCT_TYPE20 s20; 
	} INNER_STRUCT_TYPE19;

typedef struct {
	  int 	int_var18;
	  float float_var18;
	  char *string_pointer18;
	  INNER_STRUCT_TYPE19 s19; 
	} INNER_STRUCT_TYPE18;

typedef struct {
	  int 	int_var17;
	  float float_var17;
	  char *string_pointer17;
	  INNER_STRUCT_TYPE18 s18; 
	} INNER_STRUCT_TYPE17;

typedef struct {
	  int 	int_var16;
	  float float_var16;
	  char *string_pointer16;
	  INNER_STRUCT_TYPE17 s17; 
	} INNER_STRUCT_TYPE16;

typedef struct {
	  int 	int_var15;
	  float float_var15;
	  char *string_pointer15;
	  INNER_STRUCT_TYPE16 s16; 
	} INNER_STRUCT_TYPE15;

typedef struct {
	  int 	int_var14;
	  float float_var14;
	  char *string_pointer14;
	  INNER_STRUCT_TYPE15 s15; 
	} INNER_STRUCT_TYPE14;

typedef struct {
	  int 	int_var13;
	  float float_var13;
	  char *string_pointer13;
	  INNER_STRUCT_TYPE14 s14; 
	} INNER_STRUCT_TYPE13;

typedef struct {
	  int 	int_var12;
	  float float_var12;
	  char *string_pointer12;
	  INNER_STRUCT_TYPE13 s13; 
	} INNER_STRUCT_TYPE12;

typedef struct {
	  int 	int_var11;
	  float float_var11;
	  char *string_pointer11;
	  INNER_STRUCT_TYPE12 s12; 
	} INNER_STRUCT_TYPE11;

typedef struct {
	  int 	int_var10;
	  float float_var10;
	  char *string_pointer10;
	  INNER_STRUCT_TYPE11 s11; 
	} INNER_STRUCT_TYPE10;

typedef struct {
	  int 	int_var9;
	  float float_var9;
	  char *string_pointer9;
	  INNER_STRUCT_TYPE10 s10; 
	} INNER_STRUCT_TYPE9;

typedef struct {
	  int 	int_var8;
	  float float_var8;
	  char *string_pointer8;
	  INNER_STRUCT_TYPE9 s9; 
	} INNER_STRUCT_TYPE8;

typedef struct {
	  int 	int_var7;
	  float float_var7;
	  char *string_pointer7;
	  INNER_STRUCT_TYPE8 s8; 
	} INNER_STRUCT_TYPE7;

typedef struct {
	  int 	int_var6;
	  float float_var6;
	  char *string_pointer6;
	  INNER_STRUCT_TYPE7 s7; 
	} INNER_STRUCT_TYPE6;

typedef struct {
	  int 	int_var5;
	  float float_var5;
	  char *string_pointer5;
	  INNER_STRUCT_TYPE6 s6; 
	} INNER_STRUCT_TYPE5;

typedef struct {
	  int 	int_var4;
	  float float_var4;
	  char *string_pointer4;
	  INNER_STRUCT_TYPE5 s5; 
	} INNER_STRUCT_TYPE4;

typedef struct {
	  int 	int_var3;
	  float float_var3;
	  char *string_pointer3;
	  INNER_STRUCT_TYPE4 s4; 
	} INNER_STRUCT_TYPE3;

typedef struct {
	  int 	int_var2;
	  float float_var2;
	  char *string_pointer2;
	  INNER_STRUCT_TYPE3 s3; 
	} INNER_STRUCT_TYPE2;

typedef struct {
	  int 	int_var1;
	  float float_var1;
	  char *string_pointer1;
	  INNER_STRUCT_TYPE2 s2; 
	} INNER_STRUCT_TYPE1;


/*-----------------------------------------------------------------------------
 *  copy from c_demo
 *-----------------------------------------------------------------------------*/
typedef enum {red, yellow, blue} color_type;

typedef struct {
      short   a;
      char    b[16];
      int     c;
      color_type color;
    } REC_TYPE1;

typedef struct {        
      int long_integer;
      short short_integer;
      int integer_array[4];
      char *string_pointer;
    } REC_TYPE2;

#endif

