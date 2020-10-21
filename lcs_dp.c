#include <stdio.h>

int max(int a, int b) {   
	return (a > b)? a : b;
}

int LCSubStr(char *X, char *Y, int m, int n) {
    int LCSuff[m+1][n+1];
    int i,j,result = 0;
    for ( i=0; i<=m; i++) {
        for ( j=0; j<=n; j++) {
            if (i == 0 || j == 0)
                LCSuff[i][j] = 0;
            else if (X[i-1] == Y[j-1]) {
                LCSuff[i][j] = LCSuff[i-1][j-1] + 1;
                result = max(result, LCSuff[i][j]);
            }
            else LCSuff[i][j] = 0;
        }
    }
    return result;
}

int main() {
	char a[100];
	char b[100];
	printf("Enter string 1 : ");
	gets(a);
	printf("Enter string 2 : ");
	gets(b);
	int m=strlen(a);
	int n=strlen(b);
    printf("Length of Longest Common Substring is %d",LCSubStr(a,b,m,n));
    return 0;
}
