#include "namediff.h"
#include <string.h>
#include <stdio.h>

#define _MIN(x,y,z) ((x<y)?((x<z)?x:z):((y<z)?y:z))
#define MAX2(a,b) (a>b?a:b)
#define MIN2(a,b) (a>b?b:a)

#define isch(x) ((x>='a'&&x<='z')||(x>='A'&&x<='Z'))

#define lowcase(x) ((x>='A'&&x<='Z')?x-32:x)

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
		if(isch(str[i])||(str[i] =='-'))
		{
			if(str[i]=='-') continue;
			ni.part[ni.len][j] = lowcase(str[i]);
			j++;
		}else if(j>0)
		{
			ni.part[ni.len][j] = '\0';
			ni.abf[ni.len] = (j == 1);
			ni.len++;
			j = 0;
		}
		
	}
	if(j>0)
	{
		ni.part[ni.len][j] = '\0';
		ni.abf[ni.len] = j == 1 ;
		ni.len++;
		j = 0;
	}
	return ni;
}

int editDistanceT(const char *cs1,int s1len,const char * cs2,int s2len,int threshold) 
{
	if (s1len-s2len>threshold||s2len-s1len>threshold) return -1;
	int st, en;
	int flag;
	int d[s1len+1][s2len+1];
	char s1[s1len+1];
	char s2[s2len+1];
	memset(d,0,sizeof(int)*((s1len+1)*(s2len+1)));
	for(int i=0;i<s1len;i++) 
		s1[i] = (cs1[i] >='A' && cs1[i] <= 'Z') ? (cs1[i] + 32) : cs1[i];
	for(int i=0;i<s2len;i++) 
		s2[i] = (cs2[i] >='A' && cs2[i] <= 'Z') ? (cs2[i] + 32) : cs2[i];
	for (int i = 1; i <= s1len; i++) 
	{
		st = MAX2(1, i - threshold);
		en = MIN2(s2len, i + threshold);
		for (int j = st; j <= en; j++) 
		{
			d[i][j] = 500;
			if (j - i + 1 <= threshold && d[i - 1][j] + 1 < d[i][j]) 
				d[i][j] = d[i - 1][j] + 1;
			if (i - j + 1 <= threshold && d[i][j - 1] + 1 < d[i][j]) 
				d[i][j] = d[i][j - 1] + 1;
			d[i][j] = MIN2(d[i][j], d[i - 1][j - 1] + (int)(s1[i - 1] != s2[j - 1]));
			if (d[i][j] <= threshold) flag = 0;
		}
		if (flag && i > threshold) return -1;
	}
	if (d[s1len][s2len] > threshold) return -1;
	else return d[s1len][s2len];
}


int nifsame(NameInfo a,int offa,NameInfo b,int offb)
{
	if(a.abf[offa] || b.abf[offb]) return a.part[offa][0] == b.part[offb][0];
	char *s1 = a.part[offa];
	char *s2 = b.part[offb];
	int la = strlen(s1);
	int lb = strlen(s2);
	int th = (la > lb ? lb : la) * 0.2;
	int ed ;
	if((ed = editDistanceT(s1,la,s2,lb,th)) == -1) return 0;
	else if(ed == 0 ) return 1; // absolutely same
	else return 2; // some difference
}


int diffweight(const char *na,const char *nb)
{
	NameInfo nia = nameparse(na);
	NameInfo nib = nameparse(nb);
	int a = nia.len+1; // length of source + 1 for j in 0,1,2...m
	int b = nib.len+1; // length of dest + 1 for i in 0,1,2...n
	int matrix[a][b];// distance matrix
	int i;
	int j;
	int ed;
	for(i = 0 ; i < a ; i ++)
	{
		memset(matrix[i],0,b*sizeof(int));
		matrix[i][0] = i;
	}
	for(i = 0 ; i < b;i++) matrix[0][i] = i;
	for(i = 1 ; i < a ; i ++ )
	{
		for(j = 1 ; j < b ; j ++)
		{
			//if(t[i-1] == s[j-1])
			if((ed = nifsame(nia,i-1,nib,j-1)) > 0)
			{
				matrix[i][j] = matrix[i-1][j-1] + (ed == 2);
			}else{
				matrix[i][j] = _MIN(matrix[i][j-1] + 2, // len of a
					matrix[i-1][j] + 2, // len of b 
					matrix[i-1][j-1] + 2); // substitute cost
			}
		}
	}
	return matrix[a-1][b-1];
}
