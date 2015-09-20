#include <sys/types.h>
#include <unistd.h>          
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <signal.h>     
#include <sys/socket.h>
#include <sys/un.h>
#include <netinet/in.h>


int main()
{
	int x;
	x =3;
	char str[512];
	sprintf(str,"somethinmg %d bla",x);
	printf("%s",str);
	return 0;
}
