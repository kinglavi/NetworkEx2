#include <sys/types.h>
#include <unistd.h>          
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <signal.h>     
#include <sys/socket.h>
#include <sys/un.h>
#include <netinet/in.h>
#define SIM_LENGTH 10 
#define PORT 1337

int code[4] = {-1,-1,-1,-1};
char resultArray[4] = {'_','_','_','_'};
int attempts = 0;
int bul = 0;
int pgiha = 0;

void checkGuess(int *guess);
void generateCode();
void initializeCodeArray();
char* print4DigitArray(char *arr);
void initializeResultArray();
void initializeCodeArray();
int isOriginalNumber(int digit);
int isWinner(int *guess);
int * fromNumberToArray(int number);
void clean_up(int cond, int *sock);
int create_socket();

void clean_up(int cond, int *sock)
/* This function close the connection
*/
{ printf("Exiting now.\n");
  close(*sock); 
  exit(cond);
} 

// This function receives a guess array and updates the bul and pgihas
// variables accordingly
void checkGuess(int *guess)
{
	// accumulate another attempt
	attempts++;

	// initialize bul and pgiha vars
	bul = 0;
	pgiha = 0;

	// Init result array
	initializeResultArray();

	// Check how many bul's and pgiha's we have, to check how many pgiha's
	// we need an embedded loop
	int i;
	for(i=0;i<4;i++)
	{
		if(guess[i] == code[i])
		{
			bul++;
			resultArray[i] = guess[i]+'0';
		}

		int j;
		for(j=0;j<4;j++)
		{
			if(i!=j && guess[i] == code[j])
			{
				pgiha++;
			}
		}
	}
}

// Function receives a guesses array and returns true (1) or false(0) if the guesses array
// is a winner array or not
int isWinner(int *guesses) {
	int i;
	for (i = 0; i < 4; i++)
	{
		if (code[i] != guesses[i])
		{
			return 0;
		}
	}

	return 1;
}

// This function generates a a new code array
void generateCode()
{
	// Initialize guesses
	initializeCodeArray();

	// Randomize the first digit of the code
	code[0] = rand() % 9;

	int i;
	// Randomize the remaining digits
	for(i = 1;i<4;i++)
	{
		int nextCodeNum = rand() % 9 ;
		while(!isOriginalNumber(nextCodeNum)){
			nextCodeNum = rand() % 9 ;
		}

		code[i] = nextCodeNum;
	}
}

// This function checks if a digit already exists in the code array
int isOriginalNumber(int digit)
{
	int i;
	for(i = 0;i<4;i++)
	{
		if(code[i] == digit)
		{
			return 0;
		}
	}

	return 1;
}

// This function initializes the code array
void initializeCodeArray()
{
	int i;
	for(i=0;i<4;i++)
	{
		code[i]=-1;
	}
}

// This function initializes the result array
void initializeResultArray()
{
	int i;
	for(i=0;i<4;i++)
	{
		resultArray[i]='_';
	}
}

char* print4DigitArray(char *arr)
{
	// Ain li kaoh lihtuv nahun...
	char *str = malloc(512/4);
	sprintf(str,"%c %c %c %c\n",arr[0],arr[1],arr[2],arr[3]);
	return str;
}

char* printBulPgiha()
{
	char* str = malloc(512/4);
	sprintf(str,"bul: %d pgiha:%d \n",bul,pgiha);
	return str;
}

int * fromNumberToArray(int number)
{
	static int arrDigits[4] = {-1,-1,-1,-1};

	arrDigits[0] = number / 1000;
	arrDigits[1] = (number / 100) % 10;
	arrDigits[2] = (number / 10) % 10;
	arrDigits[3] = number % 10;

	return arrDigits;
}

int create_socket()
/*
	Function that create the server socket and wait for connection from the client.
*/
{
	int sock; 
	int connect_sock; 
	struct sockaddr_in serv_name; 
	size_t len; 
	int count;

	sock = socket(AF_INET, SOCK_STREAM, 0);  
	if (sock < 0)
	{ perror ("Error opening channel");
	  clean_up(1, &sock);
	}

	bzero(&serv_name, sizeof(serv_name)); 
	serv_name.sin_family = AF_INET;                                   
	serv_name.sin_port = htons(PORT); 

	if (bind(sock, (struct sockaddr *)&serv_name, sizeof(serv_name)) < 0)
	{ perror ("Error naming channel");
	  clean_up(1, &sock);
	}
	  
	printf("Server is alive and waiting for socket connection from client.\n");
	listen(sock, 1); 

	len = sizeof(serv_name);
 	connect_sock = accept(sock, (struct sockaddr *)&serv_name, &len); // Extract the first connection on the queue.
	return connect_sock;
}

int main()
{
	int connect_sock = create_socket();
	char buffer[512];
	int currGuess;

	generateCode();
	char enterDigits[] = "please enter a 4 digits number\n";
	// Send a command to the client.
	write(connect_sock,&enterDigits, sizeof(enterDigits));
	
	// Read the guess number from the client
	read(connect_sock,&currGuess,sizeof(int));
	
	int* guess = fromNumberToArray(currGuess);
	while(!isWinner(guess))
	{
		buffer[0] = '\0';
		checkGuess(guess);
		
		// Concatenate the string result.
		char *resultArrayString = print4DigitArray(resultArray);
		char *bulPgiha = printBulPgiha();
		char attemptsString[512/4];
		sprintf(attemptsString,"attempts: %d\n",attempts);

		strcpy(buffer, resultArrayString);
		strcat(buffer, bulPgiha);
		strcat(buffer, attemptsString);

		// Write the turnResult to the client.
		printf("%s",buffer);
		write(connect_sock,&buffer, sizeof(buffer));

		// Start a new turn....
		char enterDigits[] = "please enter a 4 digits number\n";
		// Send a command to the client.
		write(connect_sock,&enterDigits, sizeof(buffer));
		// Read the guess number from the client
		read(connect_sock,&currGuess,sizeof(int));

		int* guess = fromNumberToArray(currGuess);
	}

	buffer[0] = '\0';
	strcpy(buffer, "winner!");

	write(connect_sock,buffer,sizeof(buffer));

	// Close the socket
	close(connect_sock);
	//clean_up(0,&sock);

	return 0;
}


