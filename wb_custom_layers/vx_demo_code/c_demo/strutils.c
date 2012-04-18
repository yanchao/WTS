/* strutils.c - implements string manipulating utilities */

/* Copyright 2004 Wind River Systems, Inc. */

/*
modification history
--------------------
01a,07oct04,f_b   added
*/

/************************************************************************
* Function:    strcpy()
* Description: String copy utility
* Notes:       Copy *s2 to *s1
*/
char *strcpy(char *s1, const char *s2)
{
    char *result;
    char *ps1;
    char *ps2;

    ps1 = s1;
    ps2 = (char*)s2;
    result = s1;

    while ((*ps1++ = *ps2++) != '\0')
    ;
 
    return (result);
} 

/************************************************************************
* Function:    strcpm()
* Description: String compare utility
* Notes:       
*/
int strcmp(register const char *s1, register const char *s2)
{
    while ((*s1 == *s2) && *s1) {
    s1++;
    s2++;
    }

  return (*s1 - *s2);
} 
