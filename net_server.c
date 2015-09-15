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

void clean_up(int cond, int *sock)
{ printf("Exiting now.\n");
  close(*sock); 
  exit(cond);
} 

int main(void)
{ 
  int sock; 
  int connect_sock; 
  struct sockaddr_in serv_name; 
  size_t len; 
  int count;
  
  // Create new socket with AF_INET domain, communication type of stream. Which is 2-ways
  sock = socket(AF_INET, SOCK_STREAM, 0);  
  
  // Check if the socket was created succesfuly
  if (sock < 0)
    { perror ("Error accourd while try to open the channel");
      clean_up(1, &sock); //  In case of failure, clean the socket
    }
  
  // Initialize the serv_name to 0
  bzero(&serv_name, sizeof(serv_name)); 
  serv_name.sin_family = AF_INET; // Set domain as AF_INET                                  
  serv_name.sin_port = htons(PORT); // Set the port

  // Bind the addres to the socket
  if (bind(sock, (struct sockaddr *)&serv_name, sizeof(serv_name)) < 0)
    { perror ("Error accourd while naming the channel");
      clean_up(1, &sock); // If failure accourd clean the socket.
    }
      
  printf("Server is alive and waiting for socket connection from client.\n");
  listen(sock, 1); // Listen for connections on the socket

  len = sizeof(serv_name);
  connect_sock = accept(sock, (struct sockaddr *)&serv_name, &len); // Extract the first connection on the queue.

  // Writing dara 10 times. Each time we write 4 bytes.
  for (count = 1; count <= SIM_LENGTH; count++)
    { write(connect_sock, &count, 4);
      printf("Server has written %d to socket.\n", count);
    }

  close(connect_sock); // Close the socket
  clean_up(0, &sock); 

}
