#include <stdio.h>
#include <string.h>
#include <math.h>
#include <stdlib.h>

int main() {
	char a[100];
	char b[100];
	char c1[100];
	printf("Enter string 1 : ");
	gets(a);
	printf("Enter string 2 : ");
	gets(b);
	int m=strlen(a);
	int n=strlen(b);
	int l,max=0,c=0,i,j,k,d;
	for(i=1;i<=m;++i) {
		for(j=0;j<=m-i;++j) {
			c=0;
			d=j;
			for(k=0;k<n;++k) {
				if(b[k]==a[d]) {
					d++;
					c++;
					if(c==i)						
						break;
				}
				else {
					c=0;
					d=j;
				}
			}
			if(c==i&&max<i) {
				c1[0]='\0';
				for(l=j;l<j+i;++l)
					c1[l-j]=a[l];
				max=i;
			}
		}
	}
	printf("Longest Common Substring in %s and %s is: %s , of length %d",a,b,c1,max);
	return 0;
}

