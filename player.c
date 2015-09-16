
#include <sys/types.h>
#include <unistd.h>          
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <signal.h>    
#include <sys/socket.h>
#include <sys/un.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#define SIM_LENGTH 10 
#define PORT 1337 


int main(int argc, char* argv[])
{ 
  int sock; 
  struct sockaddr_in cli_name; 
  struct addrinfo* server_info;
  char* hostaddr;
  int count;
  int value; 

  printf("Client is alive and establishing socket connection.\n");
  
  // Get the information of the server by his hostname. Save result to server_info.
  if (0 != getaddrinfo(argv[1], NULL, NULL, &server_info)) {
	// Exit if error accourd.
    fprintf(stderr, "Error in resolving hostname %s\n", argv[1]);
    exit(1);
  }
  
  sock = socket(AF_INET, SOCK_STREAM, 0); // Create new socket with AF_INET domain, communication type of stream. Which is 2-ways
  if (sock < 0) // Check if the socket was created succesfuly
    { perror ("Error opening channel");
      close(sock);
      exit(2);
    }
      
  // Get the address of the host
  hostaddr = inet_ntoa(((struct sockaddr_in*)server_info->ai_addr)->sin_addr);
	  
  bzero(&cli_name, sizeof(cli_name)); // Initialize the cli_name to null (0)
  cli_name.sin_family = AF_INET; // Initialize domain as AF_INET
  cli_name.sin_addr.s_addr = inet_addr(hostaddr); // Initialize the ip address of the server
  cli_name.sin_port = htons(PORT); // Initialize the port


   // Try to open connection
  if (connect(sock, (struct sockaddr *)&cli_name, sizeof(cli_name)) < 0)
    { perror ("Error accourd while tring to establishe communications");
      close(sock);
      exit(3);
    }

  char buffer[512];
  int currGuess;
  //char *server_answer;

  // Print the command to the user from the server
  read(sock,buffer,sizeof(buffer));
  printf("%s",buffer);

  // Read the guess input from the user.
  scanf("%d",&currGuess);
  // Send the user input to the server.
  write(sock,&currGuess,sizeof(buffer));

  //while (read(sock,buffer,sizeof(buffer)))


  // Try to read the socket 10 time. each time 4 bytes.
  for (count = 1; count <= SIM_LENGTH; count++)
    { read(sock, &value, 4);
      printf("Client has received %d from socket.\n", value);
    }

  printf("Exiting now.\n");

  close(sock); 
  exit(0); 

} 
