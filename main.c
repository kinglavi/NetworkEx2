
#include <stdio.h>

int code[4] = {-1,-1,-1,-1};
char resultArray[4] = {'_','_','_','_'};
int attempts = 0;
int bul = 0;
int pgiha = 0;

void checkGuess(int *guess);
void generateCode();
void initializeCodeArray();
void print4DigitArray(char *arr);
void initializeResultArray();
void initializeCodeArray();
int isOriginalNumber(int digit);
int isWinner(int *guess);
int * fromNumberToArray(int number);

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

void print4DigitArray(char *arr)
{
	printf("%c %c %c %c\n",arr[0],arr[1],arr[2],arr[3]);
}

void printBulPgiha()
{
	printf("bul: %d pgiha:%d \n",bul,pgiha);
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


int main()
{
	int currGuess;

	generateCode();

	printf("please enter a 4 digits number\n");

	scanf("%d",&currGuess);
	int* guess = fromNumberToArray(currGuess);
	while(!isWinner(guess))
	{
		checkGuess(guess);
		print4DigitArray(resultArray);
		printBulPgiha();
		printf("attempts: %d\n",attempts);

		printf("please enter a 4 digits number\n");
		scanf("%d",&currGuess);
		guess = fromNumberToArray(currGuess);
	}

	printf("winner!");

	return 0;
}


