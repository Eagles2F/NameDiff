#include <stdio.h>
#include "namediff.h"



int main()
{
	const char *na = "Tom Cruise";
	const char *nb = "T. Gruise";
	printf("name diff : %d \n",diffweight(na,nb));
	printf("Feng Xi-wei && Feng Xiwei : %d \n",diffweight("Feng Xi-wei","Feng Xiwei"));
	return 0;
}
