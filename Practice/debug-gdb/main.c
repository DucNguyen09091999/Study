#include <stdio.h>

int main(void)
{
	int total;
	for (int i =1; i <= 100;i++)
	{
		total += i;
	}
	printf("Tong cac  so tu 1 den 100 la %d\n",total);
	return 0;
}
