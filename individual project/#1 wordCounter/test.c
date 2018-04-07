#include <stdio.h>

int main() {
	FILE* fp = fopen("sample.txt","r");
	
	char buffer[100];

	while(fgets(buffer,100,fp)!=NULL)
		printf("%s\n",buffer);

	return 0;
}
