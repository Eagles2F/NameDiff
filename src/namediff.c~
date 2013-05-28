#include "namediff.h"
#include <string.h>
#include <stdio.h>

#define isch(x) ((x>='a'&&x<='z')||(x>='A'&&x<='Z'))

typedef struct 
{
	int len; // number of name part
	char part[5][20]; // name part
	char abf[5]; // Abbreviation flag
}NameInfo;


NameInfo nameparse(const char *str)
{
	NameInfo ni;
	int len = strlen(str);
	int j = 0;
	int i = 0;
	ni.len = 0;
	
	for(i=0;i<len;i++)
	{
		if(isch(str[i]))
		{
			ni[len][j] = str[i];
			j++;
		}else if(j>0)
		{
			ni.part[len][j] = '\0';
			ni.abf[len] = j == 1 ;
			ni.len++;
			j = 0;
		}
		
	}
	if(j>0)
	{
		ni.part[len][j] = '\0';
		ni.abf[len] = j == 1 ;
		ni.len++;
		j = 0;
	}
	return ni;
}


int diffweight(const char *na,const char *nb)
{
	NameInfo nia = nameparse(na);
	NameInfo nib = nameparse(nb);
	
	for(int i=0;i<nia.len;i++)
		printf("[%s][%d]\n",nia.part[i],nia.abf[i]);
	
	printf("\n");
	for(int i=0;i<nia.len;i++)
		printf("[%s][%d]\n",nib.part[i],nib.abf[i]);	
	
	
	return 0;
}
