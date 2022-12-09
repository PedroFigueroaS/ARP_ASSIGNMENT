/* itoa example */
#include <stdio.h>
#include <stdlib.h>

int main ()
{
float var =0.5;
char int_str[20];

sprintf(int_str, "%f", var);
printf("Var: %s\n", int_str);
return 0;
}

